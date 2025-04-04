#include "io/newgrounds/NG.hpp"
#include "io/newgrounds/crypto/Cipher.hpp"
#include "io/newgrounds/crypto/Encoding.hpp"

namespace io {
namespace newgrounds {

NG* NG::core = nullptr;

void NG::create(const std::string& app_id, const std::string& session_id) {
    if (core == nullptr) {
        core = new NG(app_id, session_id);
    }
}

NG::NG(const std::string& app_id, const std::string& session_id) 
    : app_id(app_id), session_id(session_id) {
    
    curl = curl_easy_init();
    if (!curl) {
        throw std::runtime_error("Failed to initialize CURL");
    }

    calls_ = std::make_unique<ComponentList>(this);
    setCalls(calls_.get());

    if (!session_id.empty()) {
        checkSession(session_id, nullptr);
    }
}

NG::~NG() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

size_t NG::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

bool NG::sendRequest(const std::string& component, const nlohmann::json& params, nlohmann::json& response) {
    std::cout << "Sending request for component: " << component << std::endl;
    std::cout << "Encryption handler " << (encryption_handler ? "is set" : "is not set") << std::endl;
    
    nlohmann::json call = {
        {"component", component},
        {"parameters", params}
    };
    
    nlohmann::json request = {
        {"app_id", app_id},
        {"session_id", session_id}
    };

    if (encryption_handler) {
        std::cout << "Raw data to encrypt: " << call.dump() << std::endl;
        std::string secure_data = encryption_handler(call.dump());
        request["call"] = {{"secure", secure_data}};
        std::cout << "Sending encrypted request: " << request.dump() << std::endl;
    } else {
        request["call"] = call;
        std::cout << "Sending unencrypted request: " << request.dump() << std::endl;
    }

    std::string post_data = request.dump();
    std::string response_string;

    curl_easy_setopt(curl, CURLOPT_URL, "https://newgrounds.io/gateway_v3.php");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cout << "CURL request failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    try {
        response = nlohmann::json::parse(response_string);
        std::cout << "Response received: " << response.dump() << std::endl;
        return true;
    } catch (...) {
        std::cout << "Failed to parse response: " << response_string << std::endl;
        return false;
    }
}

void NG::checkSession(const std::string& session_id, std::function<void(LoginState)> callback) {
    nlohmann::json params = {};
    nlohmann::json response;

    if (!sendRequest("App.checkSession", params, response)) {
        if (callback) callback(LoginState::FAIL);
        return;
    }

    try {
        if (response.contains("success") && response["success"].get<bool>() && 
            response.contains("result") && response["result"].contains("success") && 
            response["result"]["success"].get<bool>()) {

            if (response["result"].contains("data") && 
                response["result"]["data"].contains("session") && 
                response["result"]["data"]["session"].contains("user")) {

                auto& userData = response["result"]["data"]["session"]["user"];
                user = std::make_unique<User>();
                
                if (userData.contains("id"))
                    user->id = userData["id"].get<int>();
                if (userData.contains("name"))
                    user->name = userData["name"].get<std::string>();
                if (userData.contains("supporter"))
                    user->supporter = userData["supporter"].get<bool>();
                if (userData.contains("url"))
                    user->url = userData["url"].get<std::string>();

                logged_in = true;
                if (callback) callback(LoginState::SUCCESS);
            } else {
                if (callback) callback(LoginState::FAIL);
            }
        } else {
            if (callback) callback(LoginState::FAIL);
        }
    } catch (const nlohmann::json::exception& e) {
        if (callback) callback(LoginState::FAIL);
    }
}

void NG::requestLogin(std::function<void(LoginState)> callback) {
    if (logged_in) {
        if (callback) callback(LoginState::SUCCESS);
        return;
    }

    if (attempting_login) {
        if (callback) callback(LoginState::FAIL);
        return;
    }

    attempting_login = true;
    login_cancelled = false;

    nlohmann::json params = {
        {"force", true}
    };
    nlohmann::json response;

    if (!sendRequest("App.startSession", params, response)) {
        attempting_login = false;
        if (callback) callback(LoginState::FAIL);
        return;
    }

    try {
        if (response.contains("success") && response["success"].get<bool>() && 
            response.contains("result") && response["result"].contains("data")) {
            
            auto& data = response["result"]["data"];
            if (data.contains("session")) {
                auto& session = data["session"];
                
                if (session.contains("passport_url")) {
                    std::string passport_url = session["passport_url"].get<std::string>();
                    std::cout << "Opening passport URL: " << passport_url << std::endl;
                    openPassportUrl(passport_url);
                } else {
                    std::cout << "No passport URL found in response" << std::endl;
                }

                if (session.contains("id")) {
                    session_id = session["id"].get<std::string>();
                    checkSessionStatus(callback);
                }
            }
        }
    } catch (const nlohmann::json::exception&) {
        attempting_login = false;
        if (callback) callback(LoginState::FAIL);
    }
}

void NG::checkSessionStatus(std::function<void(LoginState)> callback) {
    if (login_cancelled) {
        attempting_login = false;
        if (callback) callback(LoginState::CANCELLED);
        return;
    }

    nlohmann::json params = {};
    nlohmann::json response;

    if (!sendRequest("App.checkSession", params, response)) {
        attempting_login = false;
        if (callback) callback(LoginState::FAIL);
        return;
    }

    try {
        if (response.contains("success") && response["success"].get<bool>() && 
            response.contains("result") && response["result"].contains("data")) {
            
            auto& data = response["result"]["data"];
            if (data.contains("session") && data["session"].contains("user")) {
                auto& userData = data["session"]["user"];
                if (userData.is_null()) {
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    checkSessionStatus(callback);
                    return;
                }

                user = std::make_unique<User>();
                
                if (userData.contains("id"))
                    user->id = userData["id"].get<int>();
                if (userData.contains("name"))
                    user->name = userData["name"].get<std::string>();
                if (userData.contains("supporter"))
                    user->supporter = userData["supporter"].get<bool>();
                if (userData.contains("url"))
                    user->url = userData["url"].get<std::string>();

                if (!user->name.empty()) {
                    logged_in = true;
                    attempting_login = false;
                    if (callback) callback(LoginState::SUCCESS);
                    return;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(3));
        checkSessionStatus(callback);
    } catch (const nlohmann::json::exception&) {
        attempting_login = false;
        if (callback) callback(LoginState::FAIL);
    }
}

void NG::openPassportUrl(const std::string& url) {
    #ifdef _WIN32
        system(("start " + url).c_str());
    #elif __APPLE__
        system(("open " + url).c_str());
    #else
        system(("xdg-open " + url).c_str());
    #endif
}

void NG::setupEncryption(const std::string& key, CipherType cipher, EncryptionFormat format) {
    encryption_handler = nullptr;
    std::cout << "Setting up encryption..." << std::endl;

    std::vector<uint8_t> decoded_key;
    if (format == EncryptionFormat::BASE_64) {
        decoded_key = crypto::Base64::decode(key);
        std::cout << "Decoded key length: " << decoded_key.size() << " bytes" << std::endl;
    } else {
        decoded_key = crypto::Hex::decode(key);
        std::cout << "Decoded key length: " << decoded_key.size() << " bytes" << std::endl;
    }

    auto cipher_impl = std::shared_ptr<crypto::ICipher>(crypto::createCipher(cipher, decoded_key).release());
    if (!cipher_impl) {
        std::cout << "Failed to create cipher implementation" << std::endl;
        return;
    }

    encryption_handler = [key, cipher, format]
    (const std::string& data) -> std::string {
        std::vector<uint8_t> decoded_key;
        if (format == EncryptionFormat::BASE_64) {
            decoded_key = crypto::Base64::decode(key);
        } else {
            decoded_key = crypto::Hex::decode(key);
        }
        
        auto fresh_cipher = std::shared_ptr<crypto::ICipher>(crypto::createCipher(cipher, decoded_key).release());
        std::vector<uint8_t> input(data.begin(), data.end());
        auto encrypted = fresh_cipher->encrypt(input);
        
        if (format == EncryptionFormat::BASE_64) {
            return crypto::Base64::encode(encrypted);
        } else {
            return crypto::Hex::encode(encrypted);
        }
    };

    /*
    testing purposes
    const std::string test_string = R"({"component":"Medal.getList","parameters":null})";
    std::cout << "Encryption test:" << std::endl;
    std::cout << "Input: '" << test_string << "'" << std::endl;
    std::cout << "Output: '" << encryption_handler(test_string) << "'" << std::endl;
    std::cout << "Encryption setup complete" << std::endl;
    */
}

void NG::unlockMedal(int id) {
    try {
        if (core != nullptr && logged_in && medalsInitialized) {
            std::cout << "Attempting to unlock medal " << id << std::endl;
            auto medal = medals.get(id);
            if (medal != nullptr && !medal->unlocked) {
                std::cout << "Sending unlock for medal " << id << std::endl;
                medal->sendUnlock();
            } else {
                std::cout << "Medal " << id << " already unlocked or not found" << std::endl;
            }
        } else {
            std::cout << "Cannot unlock medal " << id 
                     << " - not logged in or medals not initialized (loggedIn: " 
                     << logged_in << ", initialized: " << medalsInitialized << ")" << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Error unlocking medal " << id << ": " << e.what() << std::endl;
    }
}

void NG::queueCall(const std::string& component, 
                   const nlohmann::json& parameters,
                   std::function<void(const nlohmann::json&)> callback) {
    nlohmann::json response;
    if (sendRequest(component, parameters, response)) {
        if (callback) {
            if (response.contains("result")) {
                callback(response["result"]);
            } else {
                callback(response);
            }
        }
    } else if (callback) {
        callback(nlohmann::json::object());
    }
}

void NG::requestScoreBoards(std::function<void(bool success, const std::vector<ScoreBoard>& boards)> callback) {
    calls_->scoreBoard.getBoards(callback);
}

}} // namespace io::newgrounds 