#include "io/newgrounds/core/MedalList.hpp"
#include "io/newgrounds/NG.hpp"
#include <iostream>

namespace io {
namespace newgrounds {
namespace core {

void MedalList::loadList(std::function<void(bool)> callback) {
    nlohmann::json params = {};
    nlohmann::json response;

    if (!NG::core->sendRequest("Medal.getList", params, response)) {
        std::cout << "Failed to load medal list" << std::endl;
        if (callback) callback(false);
        return;
    }

    try {
        if (response.contains("success") && response["success"].get<bool>() && 
            response.contains("result") && response["result"].contains("data") &&
            response["result"]["data"].contains("medals")) {

            auto& medalArray = response["result"]["data"]["medals"];
            for (const auto& medalData : medalArray) {
                int id = medalData["id"].get<int>();
                medals[id] = std::make_unique<Medal>(
                    id,
                    medalData["name"].get<std::string>(),
                    medalData["description"].get<std::string>(),
                    medalData["value"].get<int>(),
                    medalData["unlocked"].get<bool>()
                );
            }

            NG::core->medalsInitialized = true;
            if (callback) callback(true);
        } else {
            if (callback) callback(false);
        }
    } catch (const nlohmann::json::exception&) {
        if (callback) callback(false);
    }
}

Medal* MedalList::get(int id) {
    auto it = medals.find(id);
    return it != medals.end() ? it->second.get() : nullptr;
}

}}} // namespace io::newgrounds::core 