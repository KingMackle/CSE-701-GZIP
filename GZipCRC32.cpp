#include <cstdint>
#include <array>
#include <vector>

#include "GZipCRC32.hpp"

using namespace std;

/**
 * @brief Get the value of the current crc value as 1's complement
 *
 * @return uint32_t crc32 value
 */
uint32_t GZipCRC32::getValue()
{
    return ~currentValue;
}
/**
 * @brief Update the CRC value by including the bytes into the calculation
 *
 * @param bytes bytes to add into the CRC32
 */
void GZipCRC32::update(const vector<uint8_t> &bytes)
{
    for (size_t i{0}; i < bytes.size(); i++){
        update(bytes[i]);
    }
}

/**
 * @brief Computed table of all the CRC values for an 8-bit message
 *
 */
const array<uint32_t, 256> GZipCRC32::CRC32Table = ([]() {
    array<uint32_t, 256> temp;
    for (uint16_t i{0}; i < 256; i++)
    {
        uint32_t checkSum{i};
        for (uint8_t j{0}; j < 8; j++)
        {
            if (checkSum & 1)
            {
                checkSum = (checkSum >> 1) ^ 0xedb88320;
            }
            else
            {
                checkSum >>= 1;
            }
        }
        temp[i] = checkSum;
    }
    return temp;
})();