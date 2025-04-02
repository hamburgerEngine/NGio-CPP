#pragma once
#include <vector>
#include <string>

namespace io {
namespace newgrounds {
namespace crypto {

class RC4 {
public:
    RC4(const std::vector<uint8_t>& key);
    std::vector<uint8_t> crypt(const std::vector<uint8_t>& data);

private:
    void ksa(const std::vector<uint8_t>& key);
    void swap(uint8_t& a, uint8_t& b);
    
    std::vector<uint8_t> S;
    int i = 0, j = 0;
};

}}} // namespace io::newgrounds::crypto 