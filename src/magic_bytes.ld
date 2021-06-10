/* Script for AppImage Type 3 Runtime */
SECTIONS
{
  /* Add AppImage magic bytes at offset 400 */
  appimage.magic : ALIGN(0x400) {
    BYTE(0x41);
    BYTE(0x49);
    BYTE(0x03);
  }
}

INSERT AFTER .interp;
