#pragma once

#include <string>
#include <vector>
#include <functional>
#include "Score.hpp"

namespace io {
namespace newgrounds {
namespace core {

class NGLite;
class ComponentList;

enum class Period {
    DAY,
    WEEK,
    MONTH,
    YEAR,
    ALL
};

class ScoreBoard {
public:
    using ScoreCallback = std::function<void(bool success)>;

    ScoreBoard(NGLite* core, int id, const std::string& name);

    int getId() const { return id_; }

    const std::string& getName() const { return name_; }

    const std::vector<Score>& getScores() const { return scores_; }

    void postScore(int value, const std::string& tag = "", ScoreCallback callback = nullptr);

    void requestScores(int limit = 10, 
                      int skip = 0,
                      Period period = Period::ALL,
                      bool social = false,
                      const std::string& tag = "",
                      const std::string& user = "",
                      ScoreCallback callback = nullptr);

private:
    NGLite* core_;
    int id_;
    std::string name_;
    std::vector<Score> scores_;
};

}}} // namespace io::newgrounds::core 