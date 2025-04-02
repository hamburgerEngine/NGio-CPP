#pragma once
#include <string>
#include <vector>

namespace io {
namespace newgrounds {
namespace crypto {

class Base64 {
public:
    static std::string encode(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> decode(const std::string& input);
};

class Hex {
public:
    static std::string encode(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> decode(const std::string& input);
};

}}} // namespace io::newgrounds::crypto 