#include "io/newgrounds/core/Score.hpp"
#include "io/newgrounds/core/User.hpp"

namespace io {
namespace newgrounds {
namespace core {

Score::Score(int value, const std::string& formattedValue, const std::string& tag, User* user)
    : value_(value)
    , formattedValue_(formattedValue)
    , tag_(tag)
    , user_(user) {
}

}}} // namespace io::newgrounds::core 