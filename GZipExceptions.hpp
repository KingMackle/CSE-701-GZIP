#pragma once

#include <stdexcept>
#include <string>

using namespace std;

/**
 * @brief Generic class for all GZip operations
 *
 */
class GenericGZipException : public runtime_error
{
public:
    GenericGZipException(const string &_message) : runtime_error(_message), _message{_message} {};
    // leaving this here in case we need custom formating of the exception message
    // virtual const char *what() const noexcept
    // {
    //     return _message.c_str();
    // }
private:
    string _message;
};

//how can the following be convereted to a template

/**
 * @brief Exception to be thrown when unable to open the specified file
 *
 */
class UnableToOpenFile : public GenericGZipException
{
public:
    UnableToOpenFile() : GenericGZipException("Encountered an error when attempting to open file"){};
};

/**
 * @brief Exception to be thrown when the destination file is not specified
 *
 */
class DestinationFileNotSpecified : public GenericGZipException
{
public:
    DestinationFileNotSpecified() : GenericGZipException("Destination file was not specified"){};
};

/**
 * @brief Exception to be thrown when an unexpected end of file is reached
 *
 */
class UnexpectedEndOfFile : public GenericGZipException
{
public:
    UnexpectedEndOfFile() : GenericGZipException("Encountered an unexpected end of file while parsing file"){};
};

/**
 * @brief Exception to be thrown when the header bytes are less than 10
 *
 */
class IncorrectHeaderLength : public GenericGZipException
{
public:
    IncorrectHeaderLength() : GenericGZipException("File header is of incorrect length"){};
};

/**
 * @brief Exception to be thrown when the footer bytes are less than 8
 *
 */
class IncorrectFooterLength : public GenericGZipException
{
public:
    IncorrectFooterLength() : GenericGZipException("File footer is of incorrect length"){};
};

/**
 * @brief Exception to be thrown when file magic does not indicate GZip format
 *
 */
class UnkownFileFormat : public GenericGZipException
{
public:
    UnkownFileFormat() : GenericGZipException("File is in not in GZip format"){};
};

/**
 * @brief Exception to be thrown when block header is not 00, 01, or 10
 *
 */
class UnexpectedBlockHeader : public GenericGZipException
{
public:
    UnexpectedBlockHeader() : GenericGZipException("Encountered an unexpected block header 11"){};
};

/**
 * @brief Exception to be thrown when the compression algorithm is not DEFLATE
 *
 */
class UnsupportedCompressionAlgorithm : public GenericGZipException
{
public:
    UnsupportedCompressionAlgorithm() : GenericGZipException("File is compressed with unsupported compression algorithm"){};
};

/**
 * @brief Exception to be thrown when the flag headers contains invalid values
 *
 */
class UnsupportedFileFlag : public GenericGZipException
{
public:
    UnsupportedFileFlag() : GenericGZipException("File contains unsupported header flags"){};
};

/**
 * @brief Exception to be thrown when the fileblock is unable to process code length
 *
 */
class EncounteredInvalidCode : public GenericGZipException
{
public:
    EncounteredInvalidCode() : GenericGZipException("Encountered a code outside the max code length"){};
};
