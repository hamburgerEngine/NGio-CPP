#pragma once

#include <string>

namespace io {
namespace newgrounds {
namespace core {

class User;

class Score {
public:
    Score(int value, const std::string& formattedValue, const std::string& tag = "", User* user = nullptr);

    int getValue() const { return value_; }

    const std::string& getFormattedValue() const { return formattedValue_; }

    const std::string& getTag() const { return tag_; }

    User* getUser() const { return user_; }

private:
    int value_;
    std::string formattedValue_;
    std::string tag_;
    User* user_;
};

}}} // namespace io::newgrounds::core 