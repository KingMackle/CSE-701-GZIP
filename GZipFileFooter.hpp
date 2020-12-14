#pragma once

#include <vector>
#include <fstream>
#include <cstdint>

#include "BitInputStream.hpp"

using namespace std;

/*
    This class represents the footer of a gzip archive.
    It is made of 8 bytes that contain the CRC32 of the
    compressed content as well as the length of the original content
*/
class GZipFileFooter
{
public:
    GZipFileFooter();
    // create a footer object using a file stream
    GZipFileFooter(BitInputStream &);
    void validateFooter(uint32_t, uint32_t);
private:
    uint32_t crc32{0}, original_length{0};
};
