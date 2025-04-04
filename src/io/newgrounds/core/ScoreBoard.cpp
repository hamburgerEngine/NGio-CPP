#include "io/newgrounds/core/ScoreBoard.hpp"
#include "io/newgrounds/core/ScoreBoardComponent.hpp"
#include "io/newgrounds/NGLite.hpp"
#include "io/newgrounds/core/ComponentList.hpp"

namespace io {
namespace newgrounds {
namespace core {

ScoreBoard::ScoreBoard(NGLite* core, int id, const std::string& name)
    : core_(core)
    , id_(id)
    , name_(name) {
}

void ScoreBoard::postScore(int value, const std::string& tag, ScoreCallback callback) {
    core_->getCalls().scoreBoard.postScore(id_, value, tag, callback);
}

void ScoreBoard::requestScores(int limit, 
                             int skip,
                             Period period,
                             bool social,
                             const std::string& tag,
                             const std::string& user,
                             ScoreCallback callback) {
    core_->getCalls().scoreBoard.getScores(id_, limit, skip, period, social, tag, user, 
        [this, callback](bool success) {
            if (callback) {
                callback(success);
            }
        });
}

}}} // namespace io::newgrounds::core