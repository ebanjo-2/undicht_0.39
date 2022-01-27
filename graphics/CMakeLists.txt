# graphics + window library

add_library(graphics STATIC

	#include/window/window.h
	#src/window/glfw/glfw_window.h
	#src/window/glfw/glfw_window.cpp

)

target_include_directories(graphics PUBLIC src)

add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/EGL-Registry)

target_link_libraries("graphics" core)
target_link_libraries("graphics" EGL-Registry) # khrplatform.h
target_link_libraries("graphics" glfw)
target_link_libraries("graphics" glad)
