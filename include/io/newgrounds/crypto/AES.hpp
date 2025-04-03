#pragma once
#include <vector>
#include <array>
#include <cstdint>
#include <cstddef>

namespace io {
namespace newgrounds {
namespace crypto {

class AESImpl {
public:
    static constexpr size_t BLOCK_SIZE = 16;
    static constexpr size_t KEY_SIZE = 16;
    static constexpr size_t NR = 10;

    AESImpl(const std::vector<uint8_t>& key);
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data);

private:
    void keyExpansion();
    void addRoundKey(uint8_t state[4][4], const uint32_t* roundKey);
    void subBytes(uint8_t state[4][4]);
    void shiftRows(uint8_t state[4][4]);
    void mixColumns(uint8_t state[4][4]);
    void encryptBlock(uint8_t in[16], uint8_t out[16]);

    std::array<uint32_t, 4 * (NR + 1)> roundKey;
    std::array<uint8_t, KEY_SIZE> key;
};

}}} // namespace io::newgrounds::crypto 