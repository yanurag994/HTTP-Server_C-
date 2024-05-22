#include <zlib.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <iomanip>

std::vector<std::byte> compress(const std::byte* data, size_t size_in_bytes)
{
    // Initialize GZip compressor stream.
    z_stream zs;

    zs.zalloc = Z_NULL;
    zs.zfree = Z_NULL;
    zs.opaque = Z_NULL;
    if (Z_OK != deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, 15 + 16, 8, Z_DEFAULT_STRATEGY)) {
        std::cerr << "ERROR WHEN INITIALIZING ZLIB GZIP STREAM" << std::endl;
        return {};
    }
    gz_header header;

    header.text = 0;  // Set if the data is ASCII text
    header.time = std::time(nullptr);  // Current time
    header.os = 255;  // OS type, 255 for unknown

    if (deflateSetHeader(&zs, &header) != Z_OK) {
        std::cerr << "deflateSetHeader Failed" << std::endl;
        return {};
    }
    //
    zs.next_in = reinterpret_cast<Bytef*>(const_cast<std::byte*>(data));
    zs.avail_in = size_in_bytes;
    constexpr size_t kWorkingBufferSize = 32768;
    Byte working_buffer[kWorkingBufferSize];
    int result;
    // TODO: don't use a vector here, let the caller handle that.
    std::vector<std::byte> compressed_output;
    do {
        zs.next_out = working_buffer;
        zs.avail_out = sizeof(working_buffer);
        result = deflate(&zs, Z_FINISH);
        if (compressed_output.size() < zs.total_out) {
            compressed_output.insert(compressed_output.end(), reinterpret_cast<std::byte*>(working_buffer), reinterpret_cast<std::byte*>(working_buffer + zs.total_out - compressed_output.size()));
        }
    } while (result == Z_OK);
    deflateEnd(&zs);
    // errors?
    if (result != Z_STREAM_END) {
        std::cerr << "ZLIB ERROR: " << result << std::endl;
    }
    return compressed_output;
}