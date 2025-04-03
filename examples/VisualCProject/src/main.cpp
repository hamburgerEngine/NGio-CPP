#include "io/newgrounds/NG.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    io::newgrounds::NG::create("59994:owOYs5dr"); // replace with ur app id

    std::cout << "Checking login status..." << std::endl;
    bool login_complete = false;

    if (io::newgrounds::NG::core->isLoggedIn()) {
        const auto* user = io::newgrounds::NG::core->getUser();
        if (user) {
            std::cout << "Already logged in as: " << user->name << std::endl;
        } else {
            std::cout << "User object is null despite being logged in" << std::endl;
        }
        login_complete = true;
    } else {
        std::cout << "Not logged in, requesting login..." << std::endl;
        io::newgrounds::NG::core->requestLogin([&login_complete](io::newgrounds::LoginState state) {
            if (state == io::newgrounds::LoginState::SUCCESS) {
                login_complete = true;
            }
        });

        while (!login_complete) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    io::newgrounds::NG::core->setupEncryption("CtZ3tHRcA9kTtf9W9xk6aA==", // replace with ur key lol
                                            io::newgrounds::CipherType::RC4,
                                            io::newgrounds::EncryptionFormat::BASE_64);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    bool medal_complete = false;
    io::newgrounds::NG::core->medals.loadList([&medal_complete](bool success) {
        if (success) {
            io::newgrounds::NG::core->unlockMedal(83703);
            medal_complete = true;
        } else {
            medal_complete = true;
        }
    });

    while (!medal_complete) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Login process complete, exiting..." << std::endl;
    return 0;
}