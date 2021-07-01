# caution: we build non-PIE binaries, which do *not* support ASLR!
# we use -no-pie, -flto, -fdata-sections ... and --gc-sections to reduce the size of the resulting binary
# TODO: assess security hazard caused by not using PIE
# TODO: set up squashfuse build as an OBJECT library in CMake to improve the LTO result
set(COMPILE_FLAGS -static -flto -fno-exceptions -fdata-sections -ffunction-sections)

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using clang which does not support -no-pie")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using gcc, adding -no-pie flag")
    list(APPEND COMPILE_FLAGS -no-pie)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using clang whose linker does not support --gc-sections")
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using gcc, adding --gc-sections linker flag")
    list(APPEND COMPILE_FLAGS -Wl,--gc-sections -Wl,--print-gc-sections)
endif()

function(setup_target TARGET)
    message(STATUS "Setting up special build flags for target ${TARGET}")

    target_compile_options(${TARGET} PUBLIC ${COMPILE_FLAGS})
    target_link_options(${TARGET} PUBLIC ${COMPILE_FLAGS})
endfunction()

add_library(appimage_type3_header OBJECT ${PROJECT_SOURCE_DIR}/include/appimage-header.h)
target_include_directories(appimage_type3_header PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(runtime runtime.c)
target_link_libraries(runtime PRIVATE libsquashfuse)
setup_target(runtime)
# add magic bytes section to the final executable
target_link_options(runtime PRIVATE -T ${CMAKE_CURRENT_SOURCE_DIR}/magic_bytes.ld )
