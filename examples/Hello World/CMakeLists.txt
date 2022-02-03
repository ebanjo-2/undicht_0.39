# hello world example
add_executable("hello_world" src/main.cpp)

target_link_libraries("hello_world" core)
target_link_libraries("hello_world" graphics)
target_link_libraries("hello_world" tools)