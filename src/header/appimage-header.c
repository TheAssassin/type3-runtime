#include <stdio.h>
#include <stdlib.h>
#include <memory.h>


#include "appimage-header.h"

appimage_header_t* read_appimage_header(const char* filename) {
    appimage_header_t* header = malloc(sizeof(appimage_header_t));

    FILE* in = fopen(filename, "rb");
    size_t bytes_read = 0;
    if (in != NULL) {
        fseek(in, APPIMAGE_HEADER_OFFSET, SEEK_SET);
        bytes_read = fread(header, sizeof(appimage_header_t), 1, in);
        fclose(in);
        if (bytes_read == 0) {
            fprintf(stderr, "ERROR: Unable to read target file contents %s!\n", filename);
        }
    } else {
        fprintf(stderr, "ERROR: Unable to open target file %s!\n", filename);
        free(header);
        return NULL;
    }

    return header;
}

void assert_header(appimage_header_t* header, unsigned short header_revision, unsigned short payload_format) {
    if (memcmp(header->appimage_magic, "AppImageFile", 12) != 0) {
        fprintf(stderr, "ERROR: Missing magic bytes \"AppImageFile\"!\n");
        exit(1);
    }

    if (header->header_revision != header_revision) {
        fprintf(stderr, "ERROR: Expected header revision %d, but found %d!\n",
                header_revision, header->header_revision);
        exit(1);
    }

    if (header->payload_format != payload_format) {
        fprintf(stderr, "ERROR: Expected payload format %d, but found %d!\n",
                payload_format, header->payload_format);
        exit(1);
    }
}