#include <vector>
#include <fstream>
#include <cstdint>
#include <string>
#include <bitset>
#include <iostream>
#include <set>
#include <climits>

#include "GZipFile.hpp"
#include "GZipFileBlock.hpp"
#include "GZipExceptions.hpp"
#include "GZipConstants.hpp"
#include "LZ77LengthDistancePair.hpp"
#include "HuffmanTable.hpp"

using namespace std;

/**
 * @brief Construct a new GZipFileBlock::GZipFileBlock object
 *
 * @param file uses the parent GZipFile to update CRC and LZ77Buffer
 */
GZipFileBlock::GZipFileBlock(GZipFile &file) : parent(file)
{

    //each block starts with a block header.
    // each block header is made up of 3 bits
    // first bit       BFINAL -> indicated if this is the last block
    // next 2 bits     BTYPE  -> indicated the compression method
    //                 00 - no compression
    //                 01 - compressed with fixed Huffman codes
    //                 10 - compressed with dynamic Huffman codes

    // block header is in little endian, but bitset understands that
    // peak the block header because we may need it again
    final_block = parent.file.consumeNextBit() == 0x01;
    uint8_t blocktype = parent.file.consumeNextNBits(2);
    uses_fixed_codes = blocktype == 0x01;
    uses_dynamic_codes = blocktype == 0x02;
}

/**
 * @brief Gets the uncompressed content of the block. Performs INFLATion of the DEFLATEd block
 *
 * @return vector<uint8_t>
 */
vector<uint8_t> GZipFileBlock::get_output()
{
    if (output_buffer.empty())
    {
        // this is the byte that constains the block header
        if (is_uncompressed())
        {
            //no compression skip the currnet byte
            length_of_uncompressed_content = parent.file.consumeNextByte() | (parent.file.consumeNextByte() << 8);
            //skip the next two bits as well because they the contain the 1's complement of the length
            parent.file.consumeNextByte();
            parent.file.consumeNextByte();
            for (uint16_t i{0}; i < length_of_uncompressed_content; i++)
            {
                auto byte = parent.file.consumeNextByte();
                parent.lz77Buffer.put(byte);
                output_buffer.push_back(byte);
            }
        }
        else if (is_fixed_huffman_codes())
        {
            literal_codes = FIXED_LITERALS_TABLE.getCodes();
            distance_codes = FIXED_DISTANCE_TABLE.getCodes();
            literal_code_map = LITERAL_MAP;
            distance_code_map = DISTANCE_MAP;
            output_buffer = decode();
        }
        else if (is_dynamic_huffman_codes())
        {
            decode_dynamic_codes();
            output_buffer = decode();
        }
        else
        {
            throw UnexpectedBlockHeader();
        }
        // after the output has been extracted, update the crc32 value
        parent.crc32.update(output_buffer);
        parent.runningLength += output_buffer.size();
    }
    return output_buffer;
}

/**
 * @brief Tests whether the block is final or not
 *
 * @return true if the block is not final
 * @return false otherwise
 */
bool GZipFileBlock::is_final()
{
    return final_block;
}

/**
 * @brief Test whether the block was compressed using fixed Huffman codes
 *
 * @return true true if the 2nd & 3rd bits are 01
 * @return false otherwise
 */
bool GZipFileBlock::is_fixed_huffman_codes()
{
    return uses_fixed_codes;
}

/**
 * @brief Test whether the block was compressed using dynamic Huffman codes
 *
 * @return true true if the 2nd & 3rd bits are 10
 * @return false otherwise
 */
bool GZipFileBlock::is_dynamic_huffman_codes()
{
    return uses_dynamic_codes;
}

/**
 * @brief Test of the block is uncompressed
 *
 * @return true if GZipFileBlock::is_fixed_huffman_codes && GZipFileBlock::is_dynamic_huffman_codesare false
 * @return false otherwise
 */
bool GZipFileBlock::is_uncompressed()
{
    // ~(p v q)
    return !(uses_dynamic_codes || uses_fixed_codes);
}

/**
 * @brief Reverse the current byte from little endian to big endian or vice versa
 *
 * @param byte byte to reverse
 * @return bitset<8> reversed byte
 */
bitset<8> GZipFileBlock::reverse(uint8_t byte)
{
    bitset<8> temp(0);
    bitset<8> incoming(byte);
    // copy bits from one bit set to another
    for (uint8_t i{0}; i < 8; i++)
    {
        temp[i] = incoming[8 - i - 1];
    }
    return temp;
}

/**
 * @brief Decodes the current block after reading the Length codes, literal codes, distance codes along with their maps as well.
 * This method reads through the block one symbol at a time to find if it is a literal/length/distance code and. Once the symbol is found it is then copied to the LZ77Buffer.
 *
 * @return vector<uint8_t> decompressed content
 */
vector<uint8_t> GZipFileBlock::decode()
{
    vector<uint8_t> out;
    uint16_t symbol = readSymbol(literal_codes, literal_code_map);
    while (symbol != END_OF_BLOCK_CODE)
    {
        // literal symbol
        if (symbol < END_OF_BLOCK_CODE)
        {
            out.push_back(symbol);
            parent.lz77Buffer.put(symbol);
        }
        else
        { // distance symbol
            // compute length of the sequence to duplicate
            symbol -= 257;
            auto lengthTuple = LZ77LengthDistancePair::LENGTH[symbol];
            uint16_t length = get<1>(lengthTuple) + parent.file.consumeNextNBits(get<0>(lengthTuple));

            // read the distance code
            symbol = readSymbol(distance_codes, distance_code_map);
            auto distanceTuple = LZ77LengthDistancePair::DISTANCE[symbol];
            uint16_t distance = get<1>(distanceTuple) + parent.file.consumeNextNBits(get<0>(distanceTuple));

            // copy the data from the buffer
            auto match = parent.lz77Buffer.get(distance, length);

            // data needs to be put back into the buffer
            parent.lz77Buffer.put(match);

            // copy the match into output buffer
            out.insert(out.end(), match.begin(), match.end());
        }
        symbol = readSymbol(literal_codes, literal_code_map);
    }
    return out;
}

/**
 * @brief Get the index of an item in the vector
 *
 * @param vec vector to iterate through
 * @param item item to look for
 * @return int offset of the item in the vec container
 */
int indexOf(vector<uint16_t> &vec, uint16_t item)
{
    // using the find and distance gave bad performance because of all of the iterator changes
    // using a template also impacted the performance of this according to gprof
    for (size_t i{0}; i < vec.size(); i++)
    {
        if (vec[i] == item)
        {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Consume bits from the block and try to parse them into unambiguous codes
 *
 * @param codes the code lengths to look for
 * @param map of the code lengths to codes
 * @return uint16_t decoded symbol
 */
uint16_t GZipFileBlock::readSymbol(vector<uint16_t> &codes, unordered_map<uint16_t, vector<uint16_t>> &map)
{
    uint16_t code{0}, codeLen{0};
    int index{-1};

    do
    {
        if (codeLen == 15) throw EncounteredInvalidCode();

        // consume the next bit
        code = (code << 1) | parent.file.consumeNextBit();
        codeLen++;

        // check if the map contains a code with the specified length
        auto codeList = map.find(codeLen);
        if (codeList != map.end())
        {
            index = indexOf(codeList->second, code);
        }
    } while (index == -1);

    // return the indexOf of the code
    return indexOf(codes, code);
}

/**
 * @brief Parse through the block header. The dynamic Huffman codes are also compressed using the same encoding. So we need to decompress the codes twice to get the codes that was used in the original compression. The codes are read bit by bit and the maps and Huffman tables are populated accordingly.
 *
 */
void GZipFileBlock::decode_dynamic_codes()
{
    uint16_t numberOfLiteralCodes = 257 + parent.file.consumeNextNBits(5);
    uint16_t numberOfDistanceCodes = 1 + parent.file.consumeNextNBits(5);
    uint16_t numberOfCodeLengths = 4 + parent.file.consumeNextNBits(4);

    // decode the length of the codes
    vector<uint16_t> lengthOfCodeLengths(19);
    for (uint8_t i{0}; i < numberOfCodeLengths; i++)
    {
        lengthOfCodeLengths[CODE_LENGTHS_OF_CODE_LENGTH[i]] = parent.file.consumeNextNBits(3);
    }
    length_codes = constructCodes(lengthOfCodeLengths);
    HuffmanTable lengthCodesTable(length_codes, lengthOfCodeLengths);
    length_code_map = HuffmanTableToMap(lengthCodesTable);

    //read the lengths of the literal and distance codes
    vector<uint16_t> allLengths(numberOfLiteralCodes + numberOfDistanceCodes);

    ////// CODE REUSE TIME //////

    /*
        0 - 15: Represent code lengths of 0 - 15
        16: Copy the previous code length 3 - 6 times.
            The next 2 bits indicate repeat length
                    (0 = 3, ... , 3 = 6)
                Example:  Codes 8, 16 (+2 bits 11),
                        16 (+2 bits 10) will expand to
                        12 code lengths of 8 (1 + 6 + 5)
        17: Repeat a code length of 0 for 3 - 10 times.
            (3 bits of length)
        18: Repeat a code length of 0 for 11 - 138 times
            (7 bits of length)
    */
    for (size_t i{0}; i < allLengths.size(); i++) {
        uint16_t symbol = readSymbol(length_codes, length_code_map);
        uint16_t numberOfRepeats{0}, codeToRepeat{0};
        // add the symbol or read the bits for its length
        if (symbol == 16) {
            // read the number of repeated codes
            numberOfRepeats = 3 + parent.file.consumeNextNBits(2);
            codeToRepeat = allLengths[i - 1];
        } else if (symbol == 17) {
             // read the number of repeated codes
            numberOfRepeats = 3 + parent.file.consumeNextNBits(3);
        } else if (symbol == 18) {
            // read the number of repeated codes
            numberOfRepeats = 11 + parent.file.consumeNextNBits(7);
        } else {
            allLengths[i] = symbol;
        }
        // do the code repeation if we need to
        if (numberOfRepeats > 0) {
            // repeat the codes
            for (int j{0}; j < numberOfRepeats; j++){
                allLengths[j + i] = codeToRepeat;
            }
            // advance the offset of the vector after repeats
            i += (numberOfRepeats - 1);
        }
    }
    // copy the literal length out of the main length vector
    vector<uint16_t> lengthOfLiteralLengths(allLengths.begin(), allLengths.begin() + numberOfLiteralCodes);
    // decode the length of the literals
    literal_codes = constructCodes(lengthOfLiteralLengths);
    HuffmanTable literalCodesTable(literal_codes, lengthOfLiteralLengths);
    // create the literal code-length map
    literal_code_map = HuffmanTableToMap(literalCodesTable);

    // copy the distance length out of the main length vector
    vector<uint16_t> lengthOfDistanceLengths(allLengths.begin() + numberOfLiteralCodes, allLengths.end());
    // decode the length of the distances
    distance_codes = constructCodes(lengthOfDistanceLengths);
    HuffmanTable distanceCodesTable(distance_codes, lengthOfDistanceLengths);
    // create the distance code-length map
    distance_code_map = HuffmanTableToMap(distanceCodesTable);
}

/**
 * @brief Construct the codes using the length table provided. The codes are constructed in the order in which they occur in the codeLengths. The codes are always increasing after every iteration
 *
 * @param codeLengths table of the ordered code lengths
 * @return vector<uint16_t> the possible codes
 */
vector<uint16_t> GZipFileBlock::constructCodes(vector<uint16_t> &codeLengths)
{
    // because we are dealing with unsigned and the code will never be more than 32768. init the vector to the Unsigned short max
    vector<uint16_t> codes(codeLengths.size(), USHRT_MAX);
    set<uint16_t> unique_codeLengths;
    for_each(codeLengths.begin(), codeLengths.end(), [&unique_codeLengths](auto item) { if (item > 0) unique_codeLengths.insert(item); });
    uint16_t code{0}, previousCodeLength{0};
    // doing the following in lambda's will complicate things because of the number of captures
    for (auto it = unique_codeLengths.begin(); it != unique_codeLengths.end(); it++) {
        auto length = *it;
        // keep increasing the code value every iteration fo the loop
        code <<= (length - previousCodeLength);
        previousCodeLength = length;
        // repeat the codes for all the offsets that had the same length
        for (size_t i{0}; i < codeLengths.size(); i++) {
            if (codeLengths[i] == length) {
                codes[i] = code++;
            }
        }
    }
    return codes;
}
