#include <bitset>
#include <cstdint>

#include "GZipFileHeaderFlags.hpp"
#include "GZipExceptions.hpp"

/**
 * @brief Construct a new GZipFileHeaderFlags::GZipFileHeaderFlags object
 *
 */
GZipFileHeaderFlags::GZipFileHeaderFlags() {}

/**
 * @brief Construct a new GZipFileHeaderFlags::GZipFileHeaderFlags object
 *
 * @param byte The byte to parse the flags from
 */
GZipFileHeaderFlags::GZipFileHeaderFlags(uint8_t byte)
{
    // set all of the flags
    bitset<8> flags{byte};
    FTEXT = flags.test(0);
    FHCRC = flags.test(1);
    FEXTRA = flags.test(2);
    FNAME = flags.test(3);
    FCOMMENT = flags.test(4);
    // if any of the following flags are true, throw an exception because they are not supported
    if (FTEXT || FHCRC || FEXTRA || FCOMMENT)
    {
        throw UnsupportedFileFlag();
    }
}

/**
 * @brief True if the header has a file name
 *
 * @return true
 * @return false
 */
bool GZipFileHeaderFlags::is_name()
{
    return FNAME;
}

/**
 * @brief true if the header has a CRC32
 *
 * @return true
 * @return false
 */
bool GZipFileHeaderFlags::is_header_crc()
{
    return FHCRC;
}

/**
 * @brief True if the file is a text file
 *
 * @return true
 * @return false
 */
bool GZipFileHeaderFlags::is_text()
{
    return FTEXT;
}

/**
 * @brief True if there are more flags that needs to be checked
 *
 * @return true
 * @return false
 */
bool GZipFileHeaderFlags::is_extra()
{
    return FNAME;
}

/**
 * @brief True if the header contains a comment after the file name
 *
 * @return true
 * @return false
 */
bool GZipFileHeaderFlags::is_comment()
{
    return FCOMMENT;
}