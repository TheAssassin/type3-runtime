#! /bin/bash

set -x
set -e

# change to this directory; makes specifying paths quite a lot easier
here=$(readlink -f $(dirname "$0"))
cd "$here"

if [[ "$DIST" == "" ]] || [[ "$DIST_VERSION" == "" ]] || [[ "$ARCH" == "" ]] || [[ "$COMPILER" == "" ]]; then
    echo "Usage: env DIST=... DIST_VERSION=... ARCH=... COMPILER=... $0"
    exit 2
fi

DOCKERFILE=Dockerfile.build-"$DIST"-"$DIST_VERSION"-"$ARCH"-"$COMPILER"

if [[ ! -f "$DOCKERFILE" ]]; then
    echo "Error: could not find corresponding Dockerfile: $DOCKERFILE"
    exit 1
fi

IMAGE=appimage-type3-runtime-build:"$DIST"-"$DIST_VERSION"-"$ARCH"

# build image
# by using a fixed image name, the dependencies can be cached, which greatly speeds up builds
docker build --cache-from "$IMAGE" -t "$IMAGE" -f "$DOCKERFILE" "$here"

# run container from built image that calls the build script
DOCKER_OPTS=()
# fix for https://stackoverflow.com/questions/51195528/rcc-error-in-resource-qrc-cannot-find-file-png
if [ "$TRAVIS" != "" ]; then
    DOCKER_OPTS+=("--security-opt" "seccomp:unconfined")
fi

# it's important not to build in /dev/shm, don't ask me why, but squashfuse's configure doesn't want to run
# took me way too long to figure out...
docker run -e ARCH -e TRAVIS_BUILD_NUMBER -e CI=1 --rm -i "${DOCKER_OPTS[@]}" -v "$here"/..:/ws:ro "$IMAGE" \
    bash -xc "cd /ws && source ci/build.sh"
