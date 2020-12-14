#include <vector>
#include <fstream>
#include <cstdint>
#include <ctime>
#include <iostream>

#include "GZipFileHeader.hpp"
#include "GZipFileHeaderFlags.hpp"
#include "GZipConstants.hpp"
#include "GZipExceptions.hpp"
#include "BitInputStream.hpp"

using namespace std;

/**
 * @brief Construct a new GZipFileHeader::GZipFileHeader object
 *
 */
GZipFileHeader::GZipFileHeader() {}

/**
 * @brief Construct a new GZipFileHeader::GZipFileHeader object
 *
 * @param file BitInputStream to initialize the header from
 */
GZipFileHeader::GZipFileHeader(BitInputStream &file)
{
    // read a vector of 10 bytes
    vector<uint8_t> bytes(HEADER_SIZE);
    for (uint8_t i{0}; i < HEADER_SIZE; i++)
    {
        bytes[i] = file.consumeNextByte();
    }
    // gzip uses little endian for everything and I mean everything
    // magic header
    // 1F 8B       - 1st-2nd bytes are the file signature
    // 08          - 3nd byte is the compression algorithm
    // 08          - 4rd byte is the flags
    // 15 FE A5 5F - 5th-8th bytes are the timestamp
    // 04          - 9th byte is extra flags
    // 00          - 10th byte is the operating system
    if (bytes.size() < HEADER_SIZE)
    {
        throw IncorrectHeaderLength();
    }

    // the first two bytes are the file magic number
    id1 = bytes[0], id2 = bytes[1];
    if (id1 == 0x1f && id2 == 0x8b)
    {
        cout << "The file is a gzip file" << endl;
    }
    else
    {
        throw UnkownFileFormat();
    }
    // 3rd byte is compression method
    compression_method = bytes[2];
    if (compression_method == 0x08)
    {
        cout << "The file was compressed using the DEFLATE algorithm." << endl;
    }
    else
    {
        throw UnsupportedCompressionAlgorithm();
    }

    // 4 byte is flags
    flags = GZipFileHeaderFlags(bytes[3]);

    // convert the 4 bytes into an time_t by shifting it 3 times
    modified_time = bytes[7] << 24 | bytes[6] << 16 | bytes[5] << 8 | bytes[4];
    cout << "The last modification time of the file was: " << asctime(localtime(&modified_time)) << endl;

    // last two bytes of the header are ignored
    extra_flags = bytes[8];
    operating_system = bytes[9];
}

/**
 * @brief Gets the flags stored in the current header
 *
 * @return GZipFileHeaderFlags
 */
GZipFileHeaderFlags GZipFileHeader::get_flags() const
{
    return flags;
}