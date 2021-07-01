add_executable(test_appimage_section test_appimage_section.c)
target_link_libraries(test_appimage_section PRIVATE -static-libgcc -static-libstdc++ -static appimage_type3_header)
add_test(test_appimage_section test_appimage_section ${CMAKE_BINARY_DIR}/src/runtime)
