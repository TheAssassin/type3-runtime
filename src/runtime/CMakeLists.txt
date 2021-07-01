add_executable(runtime runtime.c)
target_link_libraries(runtime PRIVATE libsquashfuse appimage_type3_header)
setup_target(runtime)

# add magic bytes section to the final executable
target_link_options(runtime PRIVATE -T ${CMAKE_CURRENT_SOURCE_DIR}/magic_bytes.ld)