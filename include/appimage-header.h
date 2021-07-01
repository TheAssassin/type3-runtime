#ifndef APPIMAGE_TYPE3_RUNTIME_APPIMAGE_HEADER_H
#define APPIMAGE_TYPE3_RUNTIME_APPIMAGE_HEADER_H

#include <unistd.h>

#define APPIMAGE_HEADER_OFFSET 0x400

enum payload_formats_e {
    PAYLOAD_SQUASHFS = 1
};

typedef struct {
    unsigned char appimage_magic[12];
    unsigned short header_revision;
    unsigned short payload_format;
    off_t payload_offset __attribute__ ((aligned (8)));
    off_t resources_offset __attribute__ ((aligned (8)));
    off_t signature_offset __attribute__ ((aligned (8)));
} appimage_header_t;


appimage_header_t* read_appimage_header(const char* filename);

void assert_header(appimage_header_t* header, unsigned short header_revision, unsigned short payload_format);

#endif //APPIMAGE_TYPE3_RUNTIME_APPIMAGE_HEADER_H
