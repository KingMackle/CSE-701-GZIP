#pragma once

#include <fstream>
#include <cstdint>
#include <string>
#include <vector>

using namespace std;

// check if header file has already been included

class BitInputStream : public istream
{
public:
    // Constructor - Convert ifStream into a BitInputStream
    BitInputStream(const string &);
    BitInputStream(){};
    // consume the next byte
    uint8_t consumeNextByte();
    // consume the next n bytes
    vector<uint8_t> consumeNextNBytes(uint16_t);
    // consume the next bit
    uint8_t consumeNextBit();
    // consume the next n bits
    uint16_t consumeNextNBits(uint16_t);

private:
    ifstream inputStream;
    uint8_t buffer{0}, currentOffset{0};
    bool hasBufferBeenFilled{false};
    // fills the buffer that holds the current byte being consumed the consumeNextBit
    void fillBuffer();
};
