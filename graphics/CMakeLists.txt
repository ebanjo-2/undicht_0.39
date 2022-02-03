# graphics + window library

add_library(graphics STATIC
	
	src/graphics_config.h		

	src/window/window.h
	src/window/window.cpp

	src/window/glfw/glfw_window.h
	src/window/glfw/glfw_window.cpp

	src/api/graphics_api.h

	src/api/gl33/gl33_graphics_api.h
	src/api/gl33/gl33_graphics_api.cpp

	src/api/vulkan/vulkan_graphics_api.h
	src/api/vulkan/vulkan_graphics_api.cpp

)

target_include_directories(graphics PUBLIC src)

add_subdirectory(extern/glfw)
add_subdirectory(extern/glad)
add_subdirectory(extern/EGL-Registry)
add_subdirectory(extern/vulkan_sdk)


target_link_libraries("graphics" core)
target_link_libraries("graphics" EGL-Registry) # khrplatform.h
target_link_libraries("graphics" glfw)
target_link_libraries("graphics" glad)
target_link_libraries("graphics" vulkan_sdk)

