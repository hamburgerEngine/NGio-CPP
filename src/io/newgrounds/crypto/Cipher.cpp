#include "io/newgrounds/crypto/Cipher.hpp"

namespace io {
namespace newgrounds {
namespace crypto {

std::unique_ptr<ICipher> createCipher(CipherType type, const std::vector<uint8_t>& key) {
    switch (type) {
        case CipherType::RC4:
            return std::make_unique<RC4>(key);
        case CipherType::AES_128:
            return std::make_unique<AES128>(key);
        default:
            return nullptr;
    }
}

}}} // namespace io::newgrounds::crypto