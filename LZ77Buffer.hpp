#pragma once

#include <array>
#include <vector>
#include <cstdint>

#include "GZipConstants.hpp"

using namespace std;

class LZ77Buffer {
    public:
        void put(uint8_t);
        void put(vector<uint8_t> &);
        vector<uint8_t> get(uint16_t, uint16_t);
        friend ostream &operator<<(ostream &, const LZ77Buffer &);
    private:
        uint16_t currentSize{0}, currentOffset{0};
        array<uint8_t, MAX_BUFFER_SIZE> buffer;
};

/**
 * @brief Optimized way to do the modulus operation because the buffer will get called many times when LZ77 is enabled
 * see https://stackoverflow.com/questions/33333363/built-in-mod-vs-custom-mod-function-improve-the-performance-of-modulus-op
 *
 * @param input
 * @param ceil
 * @return uint16_t
 */
inline uint16_t fast_mod(uint16_t input, size_t ceil) {
    return input >= ceil ? input % ceil : input;
}

/**
 * @brief Put a singly byte onto the LZ77Buffer
 *
 * @param byte
 */
inline void LZ77Buffer::put(uint8_t byte)
{
    buffer[currentOffset++] = byte;
    currentOffset = fast_mod(currentOffset, buffer.size());
}