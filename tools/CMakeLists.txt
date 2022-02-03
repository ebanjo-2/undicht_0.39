add_library("tools" STATIC

    src/undicht_time.h
    src/undicht_time.cpp
)

target_include_directories("tools" PUBLIC src extern/glm)

add_subdirectory(extern/stb)

target_link_libraries("tools" PUBLIC stb_image)
