#pragma once

#include <vector>
#include <cstdint>

using namespace std;

class HuffmanTable
{
    friend ostream &operator<<(ostream &, const HuffmanTable &);
public:
    HuffmanTable(vector<uint16_t>&, vector<uint16_t>&);
    vector<uint16_t> getCodes();
    vector<uint16_t> getCodes() const;
    vector<uint16_t> getCodesLengths();
    vector<uint16_t> getCodesLengths() const;
private:
    vector<uint16_t> codes, codesLengths;
};