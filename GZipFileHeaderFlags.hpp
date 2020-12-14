#pragma once

#include <cstdint>

/*
Class to define the possible flags that may be present in the header
*/
class GZipFileHeaderFlags
{
public:
    GZipFileHeaderFlags();
    GZipFileHeaderFlags(uint8_t);
    // name of the file
    bool is_name();
    // header has a crc16
    bool is_header_crc();
    // file is a text file
    bool is_text();
    // extra fields are defined
    bool is_extra();
    // header has a comment
    bool is_comment();

private:
    bool FTEXT{false}, FHCRC{false}, FEXTRA{false}, FNAME{false}, FCOMMENT{false};
};
