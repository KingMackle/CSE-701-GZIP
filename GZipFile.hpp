#pragma once

#include <vector>
#include <fstream>

#include "GZipFileHeader.hpp"
#include "GZipFileBlock.hpp"
#include "GZipFileFooter.hpp"
#include "BitInputStream.hpp"
#include "GZipCRC32.hpp"
#include "LZ77Buffer.hpp"

using namespace std;

/*
    This class represents the entire GZipFile including header, footer, and blocks.
    This is where all the interactions should happen
*/
class GZipFile
{
    friend class GZipFileBlock;
public:
    GZipFile(const string &, const string &);
    // this will inflate all the blocks and write to the destination file
    void inflate();
    // returns the original file name of this compressed file
    string get_filename();
    // returns the header of this gzipfile
    GZipFileHeader get_header();
    // returns the footer of this gzipfile
    GZipFileFooter get_footer();

private:
    BitInputStream file;
    string dest;
    GZipFileHeader header;
    GZipFileFooter footer;
    string compressed_filename;
    LZ77Buffer lz77Buffer;
    GZipCRC32 crc32;
    // total length of the decompressed content
    uint32_t runningLength{0};
    // helper method to extract the original name of the compressed file
    string extract_filename();
};
