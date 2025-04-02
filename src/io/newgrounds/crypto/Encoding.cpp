#include "io/newgrounds/crypto/Encoding.hpp"

namespace io {
namespace newgrounds {
namespace crypto {

static const std::string base64_chars = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string Base64::encode(const std::vector<uint8_t>& data) {
    std::string ret;
    int i = 0;
    int j = 0;
    uint8_t char_array_3[3];
    uint8_t char_array_4[4];

    for (uint8_t byte : data) {
        char_array_3[i++] = byte;
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; i < 4; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; j < i + 1; j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';
    }

    return ret;
}

std::vector<uint8_t> Base64::decode(const std::string& input) {
    std::vector<uint8_t> ret;
    int i = 0;
    int j = 0;
    int in_ = 0;
    uint8_t char_array_4[4], char_array_3[3];

    for (char c : input) {
        if (c == '=') break;
        if (base64_chars.find(c) == std::string::npos) continue;

        char_array_4[i++] = c;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; i < 3; i++)
                ret.push_back(char_array_3[i]);
            i = 0;
        }
    }

    if (i) {
        for (j = 0; j < i; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

        for (j = 0; j < i - 1; j++)
            ret.push_back(char_array_3[j]);
    }

    return ret;
}

std::string Hex::encode(const std::vector<uint8_t>& data) {
    static const char hex_chars[] = "0123456789ABCDEF";
    std::string output;
    output.reserve(data.size() * 2);

    for (uint8_t byte : data) {
        output.push_back(hex_chars[byte >> 4]);
        output.push_back(hex_chars[byte & 0xF]);
    }

    return output;
}

std::vector<uint8_t> Hex::decode(const std::string& input) {
    std::vector<uint8_t> output;
    output.reserve(input.length() / 2);

    for (size_t i = 0; i < input.length(); i += 2) {
        char high = std::toupper(input[i]);
        char low = std::toupper(input[i + 1]);
        
        uint8_t byte = ((high >= 'A' ? high - 'A' + 10 : high - '0') << 4) |
                       (low >= 'A' ? low - 'A' + 10 : low - '0');
        output.push_back(byte);
    }

    return output;
}

}}} // namespace io::newgrounds::crypto 