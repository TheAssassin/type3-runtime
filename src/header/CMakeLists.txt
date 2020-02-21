set(include_dir ${PROJECT_SOURCE_DIR}/include/appimage/type3/)

add_library(appimage_type3_header OBJECT ${include_dir}/appimage-header.h)
target_link_directories(appimage_type3_header PRIVATE libsquashfuse)
target_include_directories(appimage_type3_header PUBLIC ${PROJECT_SOURCE_DIR}/include)
