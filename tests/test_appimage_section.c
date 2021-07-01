#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "appimage-header.h"


void print_header_raw_data(const appimage_header_t* header);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments");
        return -1;
    }

    appimage_header_t* header = read_appimage_header(argv[1]);

    print_header_raw_data(header);

    assert(memcmp(header->appimage_magic, "AppImageFile", 12) == 0);
    assert(header->header_revision == 0x03);
    assert(header->payload_format == 0x01);
    assert(header->payload_offset == 0x0);
    assert(header->resources_offset == 0x0);
    assert(header->signature_offset == 0x0);
}

void print_header_raw_data(const appimage_header_t* header) {
    char* raw_data = (char*) header;
    fprintf(stderr, "raw header: \n");
    for (int i = 0; i < sizeof(appimage_header_t); i++)
        fprintf(stderr, "%02x ", (unsigned char) *(raw_data + i));
    fprintf(stderr, "\n");

    for (int i = 0; i < sizeof(appimage_header_t); i++)
        fprintf(stderr, " %c ", (unsigned char) *(raw_data + i));
    fprintf(stderr, "\n");
    fflush(stderr);
}
