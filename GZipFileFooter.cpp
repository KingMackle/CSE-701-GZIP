#include <vector>
#include <fstream>
#include <cstdint>
#include <iostream>

#include "GZipFileFooter.hpp"
#include "GZipConstants.hpp"
#include "GZipExceptions.hpp"
#include "BitInputStream.hpp"

using namespace std;

/**
 * @brief Construct a new GZipFileFooter::GZipFileFooter object
 *
 */
GZipFileFooter::GZipFileFooter() {}

/**
 * @brief Construct a new GZipFileFooter::GZipFileFooter object
 *
 * @param file BitInputStream to initialize the footer from
 */
GZipFileFooter::GZipFileFooter(BitInputStream &file)
{
    // compute the CRC32 and original content length
    vector<uint8_t> bytes(FOOTER_SIZE);
    for (uint8_t i{0}; i < FOOTER_SIZE; i++)
    {
        bytes[i] = file.consumeNextByte();
    }
    crc32 = bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
    original_length = bytes[4] | bytes[5] << 8 | bytes[6] << 16 | bytes[7] << 24;
    cout << "CRC32: " << crc32 << endl;
    cout << "Original length: " << original_length << endl;
}

/**
 * @brief Validate that the current footer contains the specified information
 *
 * @param crc CRC32 to check against
 * @param length length of the content
 */
void GZipFileFooter::validateFooter(uint32_t crc, uint32_t length) {
    if (crc32 != crc){
        cerr << "CRC32 values don't match" << endl;
    } else {
        cout << "CRC32 values match" << endl;
    }

    if (original_length != length){
        cerr << "Lengths don't match" << endl;
    } else {
        cout << "Lengths match" << endl;
    }
}
