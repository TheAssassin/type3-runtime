#! /bin/bash

set -x
set -e

# use English for all outputs
# makes grepping a lot easier
export LANGUAGE=C
export LC_ALL=C

# use RAM disk if possible
if [ "$CI" == "" ] && [ -d /dev/shm ]; then
    TEMP_BASE=/dev/shm
else
    TEMP_BASE=/tmp
fi

BUILD_DIR=$(mktemp -d -p "$TEMP_BASE" appimage-type3-runtime-build-XXXXXX)

cleanup () {
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi
}

trap cleanup EXIT

REPO_ROOT=$(readlink -f $(dirname "${BASH_SOURCE[0]}")/..)
OLD_CWD=$(readlink -f .)

pushd "$BUILD_DIR"

# configure build
cmake "$REPO_ROOT" -DCMAKE_BUILD_TYPE=MinSizeRel #-DCMAKE_GENERATOR="Ninja"

# run the build
make -j$(nproc --ignore=1)

# make sure the built binary is static
readelf -d src/runtime
readelf -d src/runtime | grep -q "There is no dynamic section in this file." && echo "Yay, it's linked statically!" || exit 1

# show size of the binary
du -sh src/runtime
ls -al src/runtime

# run the tests
ctest -V
