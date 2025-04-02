#include "io/newgrounds/crypto/Cipher.hpp"

namespace io {
namespace newgrounds {
namespace crypto {

RC4::RC4(const std::vector<uint8_t>& key) : S(256) {
    ksa(key);
}

std::vector<uint8_t> RC4::encrypt(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> output(data.size());

    for (size_t n = 0; n < data.size(); n++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(S[i], S[j]);
        uint8_t k = S[(S[i] + S[j]) % 256];
        output[n] = data[n] ^ k;
    }

    return output;
}

void RC4::ksa(const std::vector<uint8_t>& key) {
    for (int i = 0; i < 256; i++) {
        S[i] = i;
    }

    int j = 0;
    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + key[i % key.size()]) % 256;
        swap(S[i], S[j]);
    }

    i = j = 0;
}

void RC4::swap(uint8_t& a, uint8_t& b) {
    uint8_t temp = a;
    a = b;
    b = temp;
}

}}} // namespace io::newgrounds::crypto 