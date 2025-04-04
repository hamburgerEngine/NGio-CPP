#pragma once

#include <string>

namespace io {
namespace newgrounds {

class User {
public:
    int id;
    std::string name;
    bool supporter;
    std::string url;
};

}} // namespace io::newgrounds 