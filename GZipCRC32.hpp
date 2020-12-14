#pragma once

#include <cstdint>
#include <vector>

using namespace std;

class GZipCRC32
{
public:
    uint32_t getValue();
    void update(const uint8_t);
    void update(const vector<uint8_t> &);
    // precompute a table of CRC values
    static const array<uint32_t, 256> CRC32Table;
private:
    uint32_t currentValue{0xffffffff};
};

/**
 * @brief Include the provided byte into the CRC value
 *
 * @param byte 8-bit message to add to the CRC computation 
 */
inline void GZipCRC32::update(const uint8_t byte)
{
    currentValue = (currentValue >> 8) ^ CRC32Table[(currentValue ^ byte) & 0xff];
}
