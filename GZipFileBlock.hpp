#pragma once

#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <bitset>
#include <unordered_map>

#include "GZipFile.hpp"
#include "BitInputStream.hpp"
#include "LZ77Buffer.hpp"

using namespace std;

class GZipFile;

/*
    This class represents a member block of the gzip archive. Each member start with a header of 3 bits that outline what follows.
      each block header is made up of 3 bits
      first bit       BFINAL -> indicated if this is the last block
      next 2 bits     BTYPE  -> indicated the compression method
                      00 - no compression
                      01 - compressed with fixed Huffman codes
                      10 - compressed with dynamic Huffman codes
*/
class GZipFileBlock
{
public:
    GZipFileBlock(GZipFile &);
    // get the putput after decompressing the block
    vector<uint8_t> get_output();
    // true if this is the last block to be inflated
    bool is_final();
    // true if the block was compressed using fixed huffman codes of the same length
    bool is_fixed_huffman_codes();
    // true if the block was compressed using dynamic huffman codes of varying lengths
    bool is_dynamic_huffman_codes();
    // true if the contents of the block were not compressed using huffman codes
    bool is_uncompressed();

private:
    GZipFile &parent;
    bool final_block{false}, uses_fixed_codes{false}, uses_dynamic_codes{false};
    uint16_t length_of_uncompressed_content{0};
    vector<uint8_t> output_buffer;
    vector<uint16_t> literal_codes, distance_codes, length_codes;
    unordered_map<uint16_t, vector<uint16_t>> literal_code_map, distance_code_map, length_code_map;
    // helper function to reverse the bits of a byte. All methods would require eaither 8 bit shifts or a loop
    bitset<8> reverse(uint8_t);
    // decode the current block
    vector<uint8_t> decode();
    // read a symbol
    uint16_t readSymbol(vector<uint16_t> &, unordered_map<uint16_t, vector<uint16_t>> &);
    // decode the dynamic codes
    void decode_dynamic_codes();
    // helper function to construct codes from non-zero code-code lengths
    vector<uint16_t> constructCodes(vector<uint16_t> &);
};
