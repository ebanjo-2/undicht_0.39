#include "graphics_config.h"
#ifdef USE_VULKAN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "debug.h"

#include "api/graphics_api.h"

namespace undicht {

    namespace graphics {

        namespace vulkan {
            ///////////////////////////////////////// vulkan specific methods /////////////////////////////////////////////////

            bool GraphicsAPI::loadInstanceData() {
                // initializes the VkInstance (return true if all went well)

                VkApplicationInfo appInfo{};                        // non critical info about the application
                appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // type of the struct
                appInfo.pApplicationName = "Hello World";
                appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                appInfo.pEngineName = "No Engine";
                appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                appInfo.apiVersion = VK_API_VERSION_1_0;

                VkInstanceCreateInfo createInfo{}; // necessary to init the VkInstance
                createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                createInfo.pApplicationInfo = &appInfo; // storing the info set above

#ifdef USE_GLFW
                // getting the extensions necessary for interfacing with the platform specific window system
                uint32_t glfwExtensionCount = 0;
                const char **glfwExtensions;

                glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

                createInfo.enabledExtensionCount = glfwExtensionCount;
                createInfo.ppEnabledExtensionNames = glfwExtensions;

#endif // USE_GLFW

                // validation layers
                createInfo.enabledLayerCount = 0;

                // creating the instance
                if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
                    return false;
                }

                return true;
            }

            ///////////////////////////////////////// implementing the interf methods /////////////////////////////////////////

            bool GraphicsAPI::init() {
                // init the graphics api

                loadInstanceData();

                return true;
            }

            bool GraphicsAPI::term() {
                // terminate the graphics api

                return true;
            }

            void GraphicsAPI::printStatus() {
                // prints the state of the api (running fine / error)
            }

            int GraphicsAPI::getError() {
                // anything other then 0 is something to worry about

                return 0;
            }

        } // namespace vulkan

    } // namespace graphics

} // namespace undicht

#endif // USE_VULKAN