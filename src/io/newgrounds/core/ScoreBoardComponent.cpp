#include "io/newgrounds/core/ScoreBoardComponent.hpp"
#include "io/newgrounds/NGLite.hpp"
#include <nlohmann/json.hpp>

namespace io {
namespace newgrounds {
namespace core {

ScoreBoardComponent::ScoreBoardComponent(NGLite* core)
    : core_(core) {
}

void ScoreBoardComponent::getBoards(BoardsCallback callback) {
    core_->queueCall("ScoreBoard.getBoards", nlohmann::json::object(),
        [this, callback](const nlohmann::json& response) {
            std::vector<ScoreBoard> boards;
            bool success = false;

            if (response.contains("data")) {
                const auto& data = response["data"];
                if (data.contains("success") && data["success"].get<bool>() &&
                    data.contains("scoreboards") && data["scoreboards"].is_array()) {
                    success = true;
                    for (const auto& board : data["scoreboards"]) {
                        if (board.contains("id") && board.contains("name")) {
                            boards.emplace_back(core_, 
                                             board["id"].get<int>(),
                                             board["name"].get<std::string>());
                        }
                    }
                }
            }

            if (callback) {
                callback(success, boards);
            }
        });
}

void ScoreBoardComponent::getScores(int id,
                                   int limit,
                                   int skip,
                                   Period period,
                                   bool social,
                                   const std::string& tag,
                                   const std::string& user,
                                   ScoreCallback callback) {
    nlohmann::json params = {
        {"id", id},
        {"limit", limit},
        {"skip", skip},
        {"social", social}
    };

    std::string periodStr;
    switch (period) {
        case Period::DAY: periodStr = "D"; break;
        case Period::WEEK: periodStr = "W"; break;
        case Period::MONTH: periodStr = "M"; break;
        case Period::YEAR: periodStr = "Y"; break;
        case Period::ALL: periodStr = "A"; break;
    }
    params["period"] = periodStr;

    if (!tag.empty()) {
        params["tag"] = tag;
    }
    if (!user.empty()) {
        params["user"] = user;
    }

    core_->queueCall("ScoreBoard.getScores", params,
        [callback](const nlohmann::json& response) {
            bool success = false;
            if (response.contains("data")) {
                const auto& data = response["data"];
                success = data.contains("success") && data["success"].get<bool>();
            }
            if (callback) {
                callback(success);
            }
        });
}

void ScoreBoardComponent::postScore(int id,
                                   int value,
                                   const std::string& tag,
                                   ScoreCallback callback) {
    nlohmann::json params = {
        {"id", id},
        {"value", value}
    };

    if (!tag.empty()) {
        params["tag"] = tag;
    }

    core_->queueCall("ScoreBoard.postScore", params,
        [callback](const nlohmann::json& response) {
            bool success = false;
            if (response.contains("data")) {
                const auto& data = response["data"];
                success = data.contains("success") && data["success"].get<bool>();
            }
            if (callback) {
                callback(success);
            }
        });
}

}}} // namespace io::newgrounds::core