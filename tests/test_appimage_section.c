#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../include/appimage-header.h"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments");
        return -1;
    }

    appimage_header_t header;
    char* filename = argv[1];
    FILE* in = fopen(filename, "rb");
    size_t bytes_read = 0;
    if (in != NULL) {
        fseek(in, 0x400, SEEK_SET);
        bytes_read = fread(&header, sizeof(appimage_header_t), 1, in);
        fclose(in);
        if (bytes_read == 0) {
            fprintf(stderr, "Unable to read target file contents %s", filename);
        }
    } else {
        fprintf(stderr, "Unable to open target file %s", filename);
        return -1;
    }

    char* raw_data = (char*) &header;
    fprintf(stderr, "read data: ");
    fprintf(stderr, "\n");
    for (int i = 0; i < sizeof(header); i++)
        fprintf(stderr, "%02x ", (unsigned char) *(raw_data + i));
    fprintf(stderr, "\n");
    for (int i = 0; i < sizeof(header); i++)
        fprintf(stderr, " %c ", (unsigned char) *(raw_data + i));
    fprintf(stderr, "\n");
    fflush(stderr);

    printf("header size: %zu\n", sizeof(header));
    assert(sizeof(header) == 40);

    assert(memcmp(header.appimage_magic, "AppImageFile", 12) == 0);
    assert(header.header_revision == 0x03);
    assert(header.payload_format == 0x01);
    assert(header.payload_offset == 0x0);
    assert(header.resources_offset == 0x0);
    assert(header.signature_offset == 0x0);
}