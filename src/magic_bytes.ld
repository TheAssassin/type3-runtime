/* Script for AppImage Type 3 Runtime */
APPIMAGE_HEADER_OFFSET = 0x400;

SECTIONS
{
  /* Add AppImage magic bytes aligned at APPIMAGE_HEADER_OFFSET; */
  .appimage : ALIGN(APPIMAGE_HEADER_OFFSET) {
    BYTE(0x41);
    BYTE(0x49);
    BYTE(0x03);
    BYTE(0x00); /* padding byte */
    SHORT(0x01); /* header revision */
    SHORT(0x01); /* payload format */
    QUAD(0xAA); /* payload offset */
    QUAD(0xAA); /* signature offset */
  }
}

ASSERT ( (SIZEOF_HEADERS < APPIMAGE_HEADER_OFFSET) ,   "Not enough space to place the AppImage header section");
INSERT AFTER .interp;
