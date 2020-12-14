# GZip Unarchiver

> This utility support the decompression of any gzip files compressed using the DEFLATE compression method.

## GZip format
A GZip archive consists of a header, footer, and a number of blocks. The header consists of 10 bytes which includes the file magic, compression method, flags, modification time, extra flags, and operating system. See [file signature](https://forensicswiki.xyz/page/Gzip) The footer consists of 8 bytes which contain CRC32 of the compressed content as well as the length of the original content. The archive consists of one or more blocks that each have a header consisting of 3 bits. The 3 bits define the compression method that was used to compress the block. The block header is followed by the literal length, code length, and the distance codes. The huffman tree is also at the beginning of the header is also encoded using a huffman tree. So, for each block there are two huffman trees that need to be decoded.
See [RFC1951](https://tools.ietf.org/html/rfc1951) and [RFC1952](https://tools.ietf.org/html/rfc1952)

*All the bytes in a gzip archive are in little endian format*

#### Driver: gzipUtil.cpp
  * contains the main driver of the utility

#### A script is provided to run a the tool against the samples provided in the *samples* directory. Please run the script using ```. runTests.sh```
#### Doxyfile is also provided to generate documentation for the library.
