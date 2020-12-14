#include <array>
#include <vector>
#include <cstdint>
#include <array>

#include "LZ77Buffer.hpp"

using namespace std;

/**
 * @brief Add a sequence of byte to the LZ77Buffer
 *
 * @param bytes
 */
void LZ77Buffer::put(vector<uint8_t> &bytes)
{
    for (size_t i{0}; i < bytes.size(); i++){
        buffer[currentOffset++] = bytes[i];
        currentOffset = fast_mod(currentOffset, buffer.size());
    }
}

/**
 * @brief Get a sequence of bytes from the in input
 *
 * @param distanceBack how far back should we copy
 * @param lengthToCopy how many characters to copy
 * @return vector<uint8_t>
 */
vector<uint8_t> LZ77Buffer::get(uint16_t distanceBack, uint16_t lengthToCopy)
{
    // The length to get may need to wrap around currentOffset in the buffer
    vector<uint8_t> bytes(lengthToCopy);
    size_t startingOffset = fast_mod((currentOffset - distanceBack), buffer.size());
    size_t offset = startingOffset;
    for (size_t i{0}; i < lengthToCopy; i++)
    {
        // copy the current byte
        bytes[i] = buffer[offset];
        // advance the offset keeping in mind the circular buffer
        offset = fast_mod((offset + 1), buffer.size());
        // we might be asked to wrap around for the distance that we are told to go back to
        if (offset == currentOffset)
        {
            offset = startingOffset;
        }
    }
    return bytes;
}

/**
 * @brief Operator<< to print out the content of the current buffer
 *
 * @param out
 * @param lz
 * @return ostream&
 */
ostream &operator<<(ostream &out, const LZ77Buffer &lz)
{
    for (size_t i{0}; i < lz.buffer.size(); i++)
    {
        out << unsigned(lz.buffer[i]) << ", ";
    }
    out << endl;
    return out;
}
