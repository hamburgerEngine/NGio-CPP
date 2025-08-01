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

    io::newgrounds::NG::core->setupEncryption("b2OXmviYngsv0Da1dZ+TVg==", // replace with ur key lol
                                            io::newgrounds::CipherType::AES_128,
                                            io::newgrounds::EncryptionFormat::BASE_64);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // gits scorenords
    bool scoreboards_complete = false;
    io::newgrounds::NG::core->requestScoreBoards(
        [&scoreboards_complete](bool success, const std::vector<io::newgrounds::ScoreBoard>& boards) {
            if (success) {
                std::cout << "Found " << boards.size() << " scoreboards:" << std::endl;
                for (const auto& board : boards) {
                    std::cout << "- " << board.getName() << " (ID: " << board.getId() << ")" << std::endl;
                }
                scoreboards_complete = true;
            } else {
                std::cout << "Failed to get scoreboards!" << std::endl;
                scoreboards_complete = true;
            }
        }
    );

    while (!scoreboards_complete) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // post score to highscore id: 14743 (replace wit ur id lel)
    bool score_post_complete = false;
    io::newgrounds::NG::core->getCalls().scoreBoard.postScore(14743, 9001, "example_tag",
        [&score_post_complete](bool success) {
            if (success) {
                std::cout << "Score posted successfully!" << std::endl;
            } else {
                std::cout << "Failed to post score!" << std::endl;
            }
            score_post_complete = true;
        }
    );

    while (!score_post_complete) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // git scores from scorbor id: 14743 (replace wit ur id lel)
    bool scores_complete = false;
    io::newgrounds::NG::core->getCalls().scoreBoard.getScores(14743, 10, 0, io::newgrounds::Period::ALL, false, "", "",
        [&scores_complete](bool success) {
            if (success) {
                std::cout << "Scores retrieved successfully!" << std::endl;
            } else {
                std::cout << "Failed to get scores!" << std::endl;
            }
            scores_complete = true;
        }
    );

    while (!scores_complete) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

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