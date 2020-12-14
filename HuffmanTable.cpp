#include <vector>
#include <cstdint>
#include <iostream>

#include "HuffmanTable.hpp"

using namespace std;

/**
 * @brief Construct a new Huffman Table:: Huffman Table object using the given codes and code lengths
 *
 * @param codes
 * @param codesLengths
 */
HuffmanTable::HuffmanTable(vector<uint16_t> &codes, vector<uint16_t> &codesLengths) : codes(codes), codesLengths(codesLengths) {}

/**
 * @brief Returns the codes stored in this table
 *
 * @return vector<uint16_t>
 */
vector<uint16_t> HuffmanTable::getCodes() { return codes; }

/**
 * @brief Returns the codes stored in this table
 *
 * @return vector<uint16_t>
 */
vector<uint16_t> HuffmanTable::getCodes() const { return codes; }

/**
 * @brief Returns the code lengths stored in this table
 *
 * @return vector<uint16_t>
 */
vector<uint16_t> HuffmanTable::getCodesLengths() { return codesLengths; }

/**
 * @brief Returns the code lengths stored in this table
 *
 * @return vector<uint16_t>
 */
vector<uint16_t> HuffmanTable::getCodesLengths() const { return codesLengths; }

/**
 * @brief Operator<< to print out table
 *
 * @param out
 * @param table
 * @return ostream&
 */
ostream &operator<<(ostream &out, const HuffmanTable &table)
{
    out << "Code table size: " << table.codes.size() << endl;
    out << '[';
    for (size_t i{0}; i < table.codes.size(); i++)
    {
        out << table.codes[i] << ", ";
    }
    out << ']' << endl;
    out << "Code Lengths table size: " << table.codesLengths.size() << endl;
    out << '[';
    for (size_t i{0}; i < table.codesLengths.size(); i++)
    {
        out << table.codesLengths[i] << ", ";
    }
    out << ']' << endl;
    return out;
}
