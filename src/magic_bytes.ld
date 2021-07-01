/* Script for AppImage Type 3 Runtime */
APPIMAGE_HEADER_OFFSET = 0x400;

SECTIONS
{
  /* Add AppImage magic bytes aligned at 0x400; */
  .appimage : ALIGN(0x400) {
    BYTE(0x41);
    BYTE(0x70);
    BYTE(0x70);
    BYTE(0x49);
    BYTE(0x6D);
    BYTE(0x61);
    BYTE(0x67);
    BYTE(0x65);
    BYTE(0x46);
    BYTE(0x69);
    BYTE(0x6C);
    BYTE(0x65);
    SHORT(0x03); /* header revision */
    SHORT(0x01); /* payload format */
    QUAD(0x0); /* payload offset */
    QUAD(0x0); /* resources offset */
    QUAD(0x0); /* signatures offset */
  }
}

INSERT AFTER .interp;
