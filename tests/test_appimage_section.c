#include <stddef.h>
#include <stdio.h>
#include <assert.h>

typedef struct {
    unsigned char appimage_magic[3];
    unsigned short header_revision;
    unsigned short payload_format;
    size_t payload_offset;
    size_t signature_offset;
} appimage_header_t;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments");
        return -1;
    }

    appimage_header_t header;
    char* filename = argv[1];
    FILE* in = fopen(filename, "rb");
    if (in != NULL) {
        fseek(in, 0x400, SEEK_SET);
        size_t bytes_read = fread(&header, sizeof(appimage_header_t), 1, in);
        fclose(in);
        if (bytes_read == 0) {
            fprintf(stderr, "Unable to read target file contents %s", filename);
        }
    } else {
        fprintf(stderr, "Unable to open target file %s", filename);
        return -1;
    }

    assert(header.appimage_magic[0] == 0x41);
    assert(header.appimage_magic[1] == 0x49);
    assert(header.appimage_magic[2] == 0x03);

    assert(header.header_revision == 0x01);
    assert(header.payload_format == 0x01);
    assert(header.payload_offset == 0xaa);
    assert(header.signature_offset == 0xaa);
}