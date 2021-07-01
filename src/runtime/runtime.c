#include <stdio.h>

#include "../commands/commands.h"

int main() {
    char* target = "/proc/self/exe";

    appimage_header_t* header = read_appimage_header(target);
    assert_header(header, 0x03, PAYLOAD_SQUASHFS);

    printf("Hello world, this is the AppImage type 3 runtime!\n");
    return 0;
}

