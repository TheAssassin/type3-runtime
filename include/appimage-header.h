#ifndef APPIMAGE_TYPE3_RUNTIME_APPIMAGE_HEADER_H
#define APPIMAGE_TYPE3_RUNTIME_APPIMAGE_HEADER_H

typedef struct {
    unsigned char appimage_magic[12];
    unsigned short header_revision;
    unsigned short payload_format;
    off_t payload_offset __attribute__ ((aligned (8)));
    off_t resources_offset __attribute__ ((aligned (8)));
    off_t signature_offset __attribute__ ((aligned (8)));
} appimage_header_t;

#endif //APPIMAGE_TYPE3_RUNTIME_APPIMAGE_HEADER_H
