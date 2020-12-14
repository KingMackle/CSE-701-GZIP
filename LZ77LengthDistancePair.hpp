#pragma once

#include <cstdint>
#include <array>
#include <iostream>
#include <tuple>

using namespace std;

/**
 * @brief Class to hold the LengthDistancePairs for fixed huffman codes
 *
 */
class LZ77LengthDistancePair
{
public:
    static const array<tuple<uint8_t, uint16_t, uint16_t>, 29> LENGTH;
    static const array<tuple<uint8_t, uint16_t, uint16_t>, 30> DISTANCE;
private:
};

/**
 * @brief Length table containing a Tuple where <num of bits, lowerBound, upperBound> for each possible code is stored
 *
 */
const array<tuple<uint8_t, uint16_t, uint16_t>, 29> LZ77LengthDistancePair::LENGTH =
    {{{0, 3, 3},
      {0, 4, 4},
      {0, 5, 5},
      {0, 6, 6},
      {0, 7, 7},
      {0, 8, 8},
      {0, 9, 9},
      {0, 10, 10},
      {1, 11, 12},
      {1, 13, 14},
      {1, 15, 16},
      {1, 17, 18},
      {2, 19, 22},
      {2, 23, 26},
      {2, 27, 30},
      {2, 31, 34},
      {3, 35, 42},
      {3, 43, 50},
      {3, 51, 58},
      {3, 59, 66},
      {4, 67, 82},
      {4, 83, 98},
      {4, 99, 114},
      {4, 115, 130},
      {5, 131, 162},
      {5, 163, 194},
      {5, 195, 226},
      {5, 227, 257},
      {0, 258, 258}}};

/**
 * @brief Distance table containing a Tuple where <num of bits, lowerBound, upperBound> for each possible code is stored
 *
 */
const array<tuple<uint8_t, uint16_t, uint16_t>, 30> LZ77LengthDistancePair::DISTANCE =
    {{{0, 1, 1},
      {0, 2, 2},
      {0, 3, 3},
      {0, 4, 4},
      {1, 5, 6},
      {1, 7, 8},
      {2, 9, 12},
      {2, 13, 16},
      {3, 17, 24},
      {3, 25, 32},
      {4, 33, 48},
      {4, 49, 64},
      {5, 65, 96},
      {5, 97, 128},
      {6, 129, 192},
      {6, 193, 256},
      {7, 257, 384},
      {7, 385, 512},
      {8, 513, 768},
      {8, 769, 1024},
      {9, 1025, 1536},
      {9, 1537, 2048},
      {10, 2049, 3072},
      {10, 3073, 4096},
      {11, 4097, 6144},
      {11, 6145, 8192},
      {12, 8193, 12288},
      {12, 12289, 16384},
      {13, 16385, 24576},
      {13, 24577, 32768}}};
