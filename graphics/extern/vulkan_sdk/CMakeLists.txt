# this file makes the vulkan sdk accessible for projects that link this library
# thanks to https://stackoverflow.com/questions/60721709/setting-up-a-vulkan-project-with-cmake-on-windows
add_library(vulkan_sdk INTERFACE)


# update this to whereever you installed the vulkan sdk from lunarG website (Windows only)
# on linux just install the vulkan-dev library
set(ENV{VULKAN_SDK} "C:/VulkanSDK/1.2.198.1")
target_include_directories(vulkan_sdk INTERFACE "C:/VulkanSDK/1.2.198.1/Include")

find_package(Vulkan REQUIRED)

target_link_libraries(vulkan_sdk INTERFACE ${Vulkan_LIBRARIES})