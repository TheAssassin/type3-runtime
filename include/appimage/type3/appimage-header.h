#pragma once

#ifdef __cplusplus
    #include <cstdio>
#else
    #include <stdint.h>
#endif

/**
 * Definition of the AppImage header, a binary metadata format describing an AppImage and providing space for
 * auxiliary information such as signatures and update information.
 *
 * The header supports fixed-size fields (so-called scalar fields) as well as fields with a dynamic length.
 * Dynamic length fields are described by an offset (a sort of pointer), counted from the start of the header, as well
 * as their length. They are appended to the fixed-size header fields in the order of which their two description
 * variables appear in the struct.
 *
 * The header is versioned by a monotonically increasing integer that is incremented on every change. Users can compare
 * this number to a minimum supported release they have implemented compatibility with. Existing header fields must not
 * be changed. New fixed-size header fields are always appended to this struct, ensuring that old definitions will
 * continue to work.
 * Dynamic header fields are appended after the fixed-size header fields. As they carry an offset already, adding new
 * fixed-size won't affect reading the header following older revisions of the header definition, as those will be
 * ignored automatically. As we define the order of dynamic-size fields appending to be the same as the order of the
 * variables in the struct, we implicitly require new fields to be appended as well, which further ensures backwards
 * compatibility.
 *
 * Dynamic size strings are guaranteed to be at most length bytes long, and may be zero-terminated. Therefore, users
 * should use strn* (e.g., strncmp, strncpy) to work with these values
 */
// The struct is explicitly aligned by four bytes to ensure portability.
// TODO: specify endianness explicitly (little endian should work fine on most relevant architectures)
typedef struct __attribute__((aligned(4), packed)) {

    /*
     * Size of the entire AppImage header. It could be calculated from the other values if you know the exact format.
     * However, this format is designed in a future proof way, as attributes may be appended at any time.
     * Therefore, by utilizing this variable, one can read all the attributes one needs, and safely skip all unknown
     * attributes to find the payload inside the file.
     */
    uint32_t appimage_header_size;

    /*
     * Type of the AppImage. For type 3, this value is a constant 3. Future types may use different values.
     */
    uint8_t appimage_type;

    /*
     * Payload type. The following types are supported:
     *
     *   - type 0: SquashFS image containing an AppDir
     */
    uint8_t payload_type;

    /*
     * Revision of the AppImage header. The number is a monotonically increasing unsigned integer, which will be
     * incremented on any addition to this header. Existing attributes will never be removed, but new ones might be
     * appended. Checking the header revision for a minimum known value guarantees compatibility with all newer
     * revisions.
     */
    // TODO: decide whether 8 bytes are sufficient for this revision
    // if we have to make 255 changes, we should probably release a new type anyway
    uint16_t header_revision;

    /*
     * Update information is a field with a dynamic size. It contains a string used for the AppImage delta update
     * mechanism.
     */
    uint16_t update_information_offset;
    uint16_t update_information_len;

    /*
     * AppImage signature section. Contains one or more signatures of the AppImage.
     * Verifying signatures for AppImages requires a special algorithm to allow for embedding the signature into an
     * AppImage after it has been built.
     */
    uint16_t signature_offset;
    uint16_t signature_len;

    /*
     * There are different "levels" of signatures in AppImages:
     *
     *   - type 0 means there is no signature; the signature field contains the "content-aware hash"
     *   - type 1 means there is a signature following the classic approach, signing the "content-aware hash" with
     *     GnuPG
     *
     * In contrary to the type 2 implementation, both the signature and the public key are embedded in the same
     * section, which makes checking the signature easier.
     * TODO: Before implementing the actual algorithm, we should check whether it makes sense to sign the key with
     * itself, too. That was erroneously missed for type 2.
     *
     * For desktop integration, the tools can extract this section from two AppImages and compare them to recognize
     * e.g., duplicates. For this purpose, security plays no role since it's a convenience feature only.
     *
     * The "content-aware hashing algorithm" is defined in a way that skips several fields of the AppImage header
     * so that the information that depends on it can be embedded into the AppImage, too. This mostly means skipping
     * respectively zeroing the contents of the signature section. The exact algorithm should be defined at a later
     * point when signing is implemented.
     */
    uint8_t signature_type;

} appimage_header;
