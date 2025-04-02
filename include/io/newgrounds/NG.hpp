#pragma once

#include <string>
#include <functional>
#include <memory>
#include <thread>
#include <chrono>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "io/newgrounds/core/MedalList.hpp"

namespace io {
namespace newgrounds {

struct User {
    int id;
    std::string name;
    bool supporter;
    std::string url;
};

enum class LoginState {
    SUCCESS,
    FAIL,
    CANCELLED
};

enum class SessionStatus {
    SESSION_EXPIRED,
    REQUEST_LOGIN,
    USER_LOADED
};

enum class CipherType {
    NONE,
    AES_128,
    RC4
};

enum class EncryptionFormat {
    BASE_64,
    HEX
};

class NG {
public:
    static void create(const std::string& app_id, const std::string& session_id = "");
    static NG* core;

    NG(const std::string& app_id, const std::string& session_id = "");
    ~NG();

    void requestLogin(std::function<void(LoginState)> callback = nullptr);
    bool isLoggedIn() const { return logged_in; }
    const User* getUser() const { return user.get(); }
    void setupEncryption(const std::string& key, CipherType cipher = CipherType::AES_128, 
                        EncryptionFormat format = EncryptionFormat::BASE_64);
    core::MedalList medals;
    bool medalsInitialized{false};
    bool sendRequest(const std::string& component, const nlohmann::json& params, nlohmann::json& response);

    void unlockMedal(int id);

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
    void checkSession(const std::string& session_id, std::function<void(LoginState)> callback);
    void checkSessionStatus(std::function<void(LoginState)> callback);
    void openPassportUrl(const std::string& url);

    std::string app_id;
    std::string session_id;
    bool logged_in{false};
    bool attempting_login{false};
    bool login_cancelled{false};
    std::unique_ptr<User> user;
    CURL* curl;
    std::function<std::string(const std::string&)> encryption_handler;
};

}} // namespace io::newgrounds