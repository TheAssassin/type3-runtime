add_executable(runtime runtime.c)
target_link_libraries(runtime PRIVATE libsquashfuse)
