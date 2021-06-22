/* Script for AppImage Type 3 Runtime */
SECTIONS
{
  /* Add AppImage magic bytes at offset 400 */
  .appimage : ALIGN(0x400) {
    BYTE(0x41);
    BYTE(0x49);
    BYTE(0x03);
    SHORT(0x01); /* header revision */
    SHORT(0x01); /* payload format */
    QUAD(0xAA); /* payload offset */
    QUAD(0xAA); /* signature offset */
  }
}

INSERT AFTER .interp;
