#pragma once

#include <string>
#include <functional>

namespace io {
namespace newgrounds {
namespace core {

class Medal {
public:
    Medal(int id, const std::string& name, const std::string& description, 
          int value, bool unlocked = false);

    void sendUnlock(std::function<void(bool)> callback = nullptr);

    int id;
    std::string name;
    std::string description;
    int value;
    bool unlocked;
};

}}} // namespace io::newgrounds::core 