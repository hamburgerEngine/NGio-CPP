#pragma once

#include "io/newgrounds/core/ScoreBoardComponent.hpp"

namespace io {
namespace newgrounds {

class NGLite;

class ComponentList {
public:
    explicit ComponentList(NGLite* core) : scoreBoard(core) {}

    ScoreBoardComponent scoreBoard;
};

}} // namespace io::newgrounds 