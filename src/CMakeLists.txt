add_subdirectory(header)

# caution: we build non-PIE binaries, which do *not* support ASLR!
# we use -no-pie, -flto, -fdata-sections ... and --gc-sections to reduce the size of the resulting binary
# TODO: assess security hazard caused by not using PIE
# TODO: set up squashfuse build as an OBJECT library in CMake to improve the LTO result
set(COMPILE_FLAGS -static -flto -fno-exceptions -fdata-sections -ffunction-sections -Wl,--gc-sections -Wl,--print-gc-sections)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using clang which does not support -no-pie")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using gcc, adding -no-pie flag")
    list(APPEND COMPILE_FLAGS -no-pie)
endif()

if(CMAKE_BUILD_TYPE STRLESS_EQUAL release)
    message(STATUS "Release build detected, adding -Os")
    list(APPEND COMPILE_FLAGS -Os)
endif()

function(setup_target TARGET)
    message(STATUS "Setting up special build flags for target ${TARGET}")

    target_compile_options(${TARGET} PUBLIC ${COMPILE_FLAGS})
    target_link_options(${TARGET} PUBLIC ${COMPILE_FLAGS})
endfunction()

add_executable(runtime runtime.c)
target_link_libraries(runtime PRIVATE appimage_type3_header)
target_link_libraries(runtime PRIVATE libsquashfuse)
setup_target(runtime)
