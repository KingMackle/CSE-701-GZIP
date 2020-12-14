#pragma once

#include <vector>
#include <iostream>
#include <cstdint>
#include <unordered_map>

#include "HuffmanTable.hpp"

using namespace std;

// consts for header and footer portions on the file
const uint8_t HEADER_SIZE = 10;
const uint8_t FOOTER_SIZE = 8;

// end of block code
const uint16_t END_OF_BLOCK_CODE = 256;
// number of fixed length codes
const uint16_t NUMBER_OF_FIXED_LENGTH_CODES = 288;
// number of fixed distance codes
const uint8_t NUMBER_OF_FIXED_DISTANCE_CODES = 30;
// maximum number of bits in any code
const uint8_t MAX_NUMBER_OF_BITS = 15;

// length codes code-lengths in order or their appearance
const vector<uint8_t> CODE_LENGTHS_OF_CODE_LENGTH{16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

// max LZ77Buffer size
const uint16_t MAX_BUFFER_SIZE = 32768;

/*
* Lit Value    Bits     Codes
* ---------    ----     -----
* 256 - 279     7       0000000 through
*                       0010111
*   0 - 143     8       00110000 through
*                       10111111
* 280 - 287     8       11000000 through
*                       11000111
* 144 - 255     9       110010000 through
*                       111111111
*/
static const HuffmanTable FIXED_LITERALS_TABLE = ([] {
    uint16_t currentCode = 0b0000000;
    vector<uint16_t> codes(286), codesLengths(286);
    // we will create the literal codes and sort them.
    // The sorting is done via the insertion at different locations in the vector, hence the four loops

    // add all codes of length 7 bits
    for (int i{256}; i < 280; i++)
    {
        codes[i] = currentCode;
        codesLengths[i] = 7;
        currentCode++;
    }

    // add all codes of length 8 bits
    // set current code to the next sequence
    currentCode = 0b00110000;
    for (int i{0}; i < 144; i++)
    {
        codes[i] = currentCode;
        codesLengths[i] = 8;
        currentCode++;
    }

    // set current code to the next sequence
    currentCode = 0b11000000;
    for (int i{280}; i < 286; i++)
    {
        codes[i] = currentCode;
        codesLengths[i] = 8;
        currentCode++;
    }
    // add all codes of length 9 bits
    // set current code to the next sequence
    currentCode = 0b110010000;
    for (int i{144}; i < 256; i++)
    {
        codes[i] = currentCode;
        codesLengths[i] = 9;
        currentCode++;
    }

    return HuffmanTable(codes, codesLengths);
})();

// FIXED distance table with codes from 0-29
// actual value in the RFC is 0-31 but 30 and 31 will never show in the payload
static const HuffmanTable FIXED_DISTANCE_TABLE = ([] {
    vector<uint16_t> codes(30), codesLengths(30);
    for (int i{0}; i < 30; i++)
    {
        codes[i] = i;
        codesLengths[i] = 5;
    }
    return HuffmanTable(codes, codesLengths);
})();

// convert Huffman table into a map
static const auto HuffmanTableToMap = [](const HuffmanTable &table) {
    unordered_map<uint16_t, vector<uint16_t>> temp;
    auto codes = table.getCodes();
    auto codesLengths = table.getCodesLengths();
    for (size_t i{0}; i < codesLengths.size(); i++)
    {
        uint16_t currentCodeLength = codesLengths[i];
        if (currentCodeLength > 0)
        {
            if (temp.find(currentCodeLength) == temp.end())
            {
                temp[currentCodeLength] = vector<uint16_t>();
            }
            temp[currentCodeLength].push_back(codes[i]);
        }
    }
    return temp;
};

// create maps of the literal and distance codes & lengths
static const auto LITERAL_MAP = HuffmanTableToMap(FIXED_LITERALS_TABLE);
static const auto DISTANCE_MAP = HuffmanTableToMap(FIXED_DISTANCE_TABLE);
