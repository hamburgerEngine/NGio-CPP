#include "io/newgrounds/core/Medal.hpp"
#include "io/newgrounds/NG.hpp"
#include <iostream>

namespace io {
namespace newgrounds {
namespace core {

Medal::Medal(int id, const std::string& name, const std::string& description, 
             int value, bool unlocked)
    : id(id), name(name), description(description), value(value), unlocked(unlocked) {}
    
void Medal::sendUnlock(std::function<void(bool)> callback) {
    if (unlocked) {
        if (callback) callback(true);
        return;
    }

    nlohmann::json params = {
        {"id", id}
    };

    nlohmann::json response;
    if (!NG::core->sendRequest("Medal.unlock", params, response)) {
        std::cout << "Failed to send medal unlock request" << std::endl;
        if (callback) callback(false);
        return;
    }

    try {
        if (response.contains("success") && response["success"].get<bool>()) {
            unlocked = true;
            std::cout << "Medal '" << name << "' unlocked!" << std::endl;
            if (callback) callback(true);
        } else {
            if (callback) callback(false);
        }
    } catch (const nlohmann::json::exception&) {
        if (callback) callback(false);
    }
}

}}} // namespace io::newgrounds::core 