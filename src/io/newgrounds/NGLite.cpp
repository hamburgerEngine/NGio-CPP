#include "io/newgrounds/NGLite.hpp"
#include "io/newgrounds/core/ComponentList.hpp"

namespace io {
namespace newgrounds {
namespace core {

ComponentList& NGLite::getCalls() {
    return *calls_;
}

void NGLite::setCalls(ComponentList* calls) {
    calls_ = calls;
}

}}} // namespace io::newgrounds::core 