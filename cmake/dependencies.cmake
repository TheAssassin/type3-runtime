# >= 3.2 required for ExternalProject_Add_StepDependencies
cmake_minimum_required(VERSION 3.2)

include(${CMAKE_CURRENT_LIST_DIR}/scripts.cmake)


# as distros don't provide suitable squashfuse and squashfs-tools, those dependencies are bundled in, can, and should
# be used from this repository for AppImageKit
# for distro packaging, it can be linked to an existing package just fine
set(USE_SYSTEM_SQUASHFUSE OFF CACHE BOOL "Use system libsquashfuse instead of building our own")

if(NOT USE_SYSTEM_SQUASHFUSE)
    message(STATUS "Downloading and building squashfuse")

    # Check if fuse is installed to provide early error reports
    import_pkgconfig_target(TARGET_NAME libfuse PKGCONFIG_TARGET fuse)

    # TODO: implement out-of-source builds for squashfuse, as for the other dependencies
    configure_file(
        ${PROJECT_SOURCE_DIR}/patches/squashfuse/patch-squashfuse.sh.in
        ${PROJECT_BINARY_DIR}/patch-squashfuse.sh
        @ONLY
    )

    ExternalProject_Add(
        squashfuse-EXTERNAL
        GIT_REPOSITORY https://github.com/vasi/squashfuse/
        GIT_TAG 0.1.103
        UPDATE_COMMAND ""  # make sure CMake won't try to fetch updates unnecessarily and hence rebuild the dependency every time
        PATCH_COMMAND bash -xe ${PROJECT_BINARY_DIR}/patch-squashfuse.sh
        CONFIGURE_COMMAND sh autogen.sh
        COMMAND ${SED} -i "s/typedef off_t sqfs_off_t/typedef int64_t sqfs_off_t/g" common.h  # off_t's size might differ, see https://stackoverflow.com/a/9073762
        COMMAND sh <SOURCE_DIR>/configure --disable-demo --disable-high-level --without-lzo --without-lz4 --prefix=<INSTALL_DIR> --libdir=<INSTALL_DIR>/lib --with-xz=${xz_PREFIX}
        COMMAND ${SED} -i "s|XZ_LIBS = -llzma |XZ_LIBS = -Bstatic ${xz_LIBRARIES}/|g" Makefile
        BUILD_COMMAND ${MAKE}
        BUILD_IN_SOURCE ON
        INSTALL_COMMAND ${MAKE} install
        UPDATE_DISCONNECTED On
    )

    import_external_project(
        TARGET_NAME libsquashfuse
        EXT_PROJECT_NAME squashfuse-EXTERNAL
        LIBRARIES "<SOURCE_DIR>/.libs/libsquashfuse.a;<SOURCE_DIR>/.libs/libfuseprivate.a"
        INCLUDE_DIRS "<SOURCE_DIR>"
    )
else()
    message(STATUS "Using system squashfuse")

    import_pkgconfig_target(TARGET_NAME libsquashfuse PKGCONFIG_TARGET squashfuse)
endif()
