#include <fstream>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "BitInputStream.hpp"
#include "GZipExceptions.hpp"

using namespace std;

/**
 * @brief Construct a new Bit Input Stream:: Bit Input Stream object
 *
 * @param path the path to file to be opened as a BitInputStream
 * @throws UnableToOpenFile
 */
BitInputStream::BitInputStream(const string &path)
{
    inputStream = ifstream{path, ios::in | ios::binary};
    if (!inputStream)
    {
        throw UnableToOpenFile();
    }
    if (inputStream.eof())
    {
        throw UnexpectedEndOfFile();
    }
}

/**
 * @brief Consume the next byte in the input stream
 *
 * @return uint8_t the byte consumed
 * @throw UnexpectedEndOfFile
 */
uint8_t BitInputStream::consumeNextByte()
{
    // whenever we consume a byte, discard the buffer
    if (inputStream.eof())
    {
        throw UnexpectedEndOfFile();
    }
    hasBufferBeenFilled = false;
    return (uint8_t)inputStream.get();
}

/**
 * @brief Consume the next N bytes from the input stream
 *
 * @param numberOfBytes The number of bytes to consume
 * @return vector<uint8_t> vector containing the consumed bytes
 * @throws UnexpectedEndOfFile
 */
vector<uint8_t> BitInputStream::consumeNextNBytes(uint16_t numberOfBytes)
{
    vector<uint8_t> temp(numberOfBytes);
    for (uint16_t i{0}; i < numberOfBytes; i++)
    {
        temp[i] = consumeNextByte();
    }
    return temp;
}

/**
 * @brief Consume the next bit in the input stream. The Current byte is stored in a buffer and bits are read off from there. If there are no more bits to consume from the current byte; the buffer will be reflowed.
 *
 * @return uint8_t the bit as a byte
 */
uint8_t BitInputStream::consumeNextBit()
{
    // check if we are at the boundary or we need to fill the buffer
    if (8 == currentOffset || !hasBufferBeenFilled)
    {
        fillBuffer();
    }
    // read the byte
    uint8_t bit = (buffer >> currentOffset) & 1;
    currentOffset++;
    return bit;
}

/**
 * @brief Consume the next N bits in the input stream. The Current byte is stored in a buffer and bits are read off from there. If there are no more bits to consume from the current byte; the buffer will be reflowed.
 *
 * @return uint8_t the bits as a byte
 */
uint16_t BitInputStream::consumeNextNBits(uint16_t numberOfBits)
{
    uint16_t temp = 0;
    for (uint16_t i{0}; i < numberOfBits; i++)
    {
        // perform or with shift i bits to the right
        temp |= (consumeNextBit() << i);
    }
    return temp;
}

/**
 * @brief Fill the current byte buffer. This is a helper method to aid with bit by bit consumption
 */
void BitInputStream::fillBuffer()
{
    currentOffset = 0;
    buffer = consumeNextByte();
    hasBufferBeenFilled = true;
}