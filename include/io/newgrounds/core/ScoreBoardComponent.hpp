#pragma once

#include <string>
#include <functional>
#include <vector>
#include "ScoreBoard.hpp"

namespace io {
namespace newgrounds {
namespace core {

class NGLite;

class ScoreBoardComponent {
public:
    using BoardsCallback = std::function<void(bool success, const std::vector<ScoreBoard>& boards)>;
    using ScoreCallback = std::function<void(bool success)>;

    explicit ScoreBoardComponent(NGLite* core);

    void getBoards(BoardsCallback callback);

    void getScores(int id,
                   int limit,
                   int skip,
                   Period period,
                   bool social,
                   const std::string& tag,
                   const std::string& user,
                   ScoreCallback callback);

    void postScore(int id,
                   int value,
                   const std::string& tag,
                   ScoreCallback callback);

private:
    NGLite* core_;
};

}}} // namespace io::newgrounds::core 