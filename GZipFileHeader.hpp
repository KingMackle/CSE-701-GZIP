#pragma once

#include <vector>
#include <fstream>
#include <cstdint>
#include <ctime>

#include "GZipFileHeaderFlags.hpp"
#include "BitInputStream.hpp"

using namespace std;

/*
    Class to define the header of a gzip file. The header consists of 10 bytes
      gzip uses little endian for everything and I mean everything
        1F 8B       - 1st-2nd bytes are the file signature
        08          - 3nd byte is the compression algorithm
        08          - 4rd byte is the flags
        15 FE A5 5F - 5th-8th bytes are the timestamp
        04          - 9th byte is extra flags
        00          - 10th byte is the operating system
*/

class GZipFileHeader
{
public:
    GZipFileHeader();
    // create a header object using a file stream
    GZipFileHeader(BitInputStream &);
    // return the flags set on the gzip header
    GZipFileHeaderFlags get_flags() const;

private:
    uint8_t id1{0}, id2{0}, compression_method{0}, extra_flags{0}, operating_system{0};
    GZipFileHeaderFlags flags;
    time_t modified_time{0};
};
