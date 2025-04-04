#pragma once
#include <string>
#include <vector>
#include <memory>
#include "io/newgrounds/NG.hpp"

namespace io {
namespace newgrounds {
namespace crypto {

class ICipher {
public:
    virtual ~ICipher() = default;
    virtual std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) = 0;
protected:
    std::vector<uint8_t> crypt(const std::vector<uint8_t>& data);
};

class RC4 : public ICipher {
public:
    RC4(const std::vector<uint8_t>& key);
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data) override;

private:
    void ksa(const std::vector<uint8_t>& key);
    void swap(uint8_t& a, uint8_t& b);
    
    std::vector<uint8_t> S;
    int i = 0, j = 0;
};

std::unique_ptr<ICipher> createCipher(CipherType type, const std::vector<uint8_t>& key);

}}} // namespace io::newgrounds::crypto 