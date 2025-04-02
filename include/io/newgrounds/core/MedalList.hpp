#pragma once

#include "Medal.hpp"
#include <unordered_map>
#include <memory>
#include <functional>

namespace io {
namespace newgrounds {
namespace core {

class MedalList {
public:
    void loadList(std::function<void(bool)> callback = nullptr);
    Medal* get(int id);
    const std::unordered_map<int, std::unique_ptr<Medal>>& getAll() const { return medals; }

private:
    std::unordered_map<int, std::unique_ptr<Medal>> medals;
};

}}} // namespace io::newgrounds::core 