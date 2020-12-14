#include <string>

#include "GZipFile.hpp"
#include "GZipExceptions.hpp"
#include "GZipConstants.hpp"
#include "BitInputStream.hpp"

using namespace std;

/**
 * @brief Construct a new GZipFile::GZipFile object
 *
 * @param source path to the source archive
 * @param dest path to out the archive
 */
GZipFile::GZipFile(const string &source, const string &dest) : dest(dest)
{
    //open up the file for reading in binary mode
    file = BitInputStream(source);

    // initialize the header
    header = GZipFileHeader(file);

    compressed_filename = extract_filename();
}

/**
 * @brief Inflate the compressed blocks regardless of their compression method
 *
 */
void GZipFile::inflate()
{
    // check that we can actually write somewhere
    if (dest.empty())
    {
        throw DestinationFileNotSpecified();
    }
    // attempt to open the file
    ofstream out_file{dest};
    if (out_file)
    {
        // start inflating the blocks
        // iterate over all the blocks
        bool final_block = false;
        do
        {
            GZipFileBlock block(*this);
            final_block = block.is_final();
            auto payload = block.get_output();
            auto size = payload.size();
            for (size_t i{0}; i < size; i++) {
                out_file << payload[i];
            }
        } while (!final_block);
    }
    else
    {
        throw UnableToOpenFile();
    }

    footer = GZipFileFooter(file);
    footer.validateFooter(crc32.getValue(), runningLength);
}

/**
 * @brief Gets the filename that was compressed
 *
 * @return string the file name stored in the header of the archive
 */
string GZipFile::get_filename()
{
    if (compressed_filename.empty() && header.get_flags().is_name())
    {
        compressed_filename = extract_filename();
    }
    return compressed_filename;
}
/**
 * @brief Return the header info of the archive
 *
 * @return GZipFileHeader Object containing all of the archive headers
 */
GZipFileHeader GZipFile::get_header()
{
    return header;
}
/**
 * @brief Return an object containing all of the archive footer info
 *
 * @return GZipFileFooter
 */
GZipFileFooter GZipFile::get_footer()
{
    return footer;
}

/**
 * @brief Extract the file name from the header
 *
 * @return string - filename
 */
string GZipFile::extract_filename()
{
    vector<char> filename;
    char current_char = -1;
    // keep reading
    while (current_char)
    {
        // read a byte and push it onto the vector
        current_char = file.consumeNextByte();
        filename.push_back(current_char);
    }
    // concat the vector into a string
    return string(filename.begin(), filename.end());
}