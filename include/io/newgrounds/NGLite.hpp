#pragma once

#include <string>
#include <functional>
#include <nlohmann/json.hpp>

namespace io {
namespace newgrounds {
namespace core {

class ComponentList;

class NGLite {
public:
    virtual ~NGLite() = default;

    virtual void queueCall(const std::string& component, 
                         const nlohmann::json& parameters,
                         std::function<void(const nlohmann::json&)> callback) = 0;

    ComponentList& getCalls();
    void setCalls(ComponentList* calls);

protected:
    ComponentList* calls_{nullptr};
};

}

using NGLite = core::NGLite;

}} // namespace io::newgrounds 