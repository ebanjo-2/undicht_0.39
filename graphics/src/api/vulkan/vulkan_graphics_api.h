#ifdef GRAPHICS_API_H // this file should only be included via include/window/window.h

#ifndef VULKAN_GRAPHICS_API_H
#define VULKAN_GRAPHICS_API_H

#include "vulkan/vulkan.h"

#include "vector"
#include "string"

namespace undicht {

    namespace graphics {

        namespace vulkan {

          const extern std::vector<const char*> REQUIRED_VULKAN_EXTENSIONS; // extensions the vulkan installation has to provide
          const extern std::vector<const char*> REQUIRED_DEVICE_EXTENSIONS; // extensions the gpu has to provide
          const extern std::vector<const char*> VALIDATION_LAYERS;


            class GraphicsAPI : public interf::GraphicsAPI {
                // the member functions are structured in the order in which they are used to get vulkan ready
                // most functions returning a boolean value will return true if everything worked, false if something failed

              public:
                // creating a vulkan instance

                VkInstance m_instance = VK_NULL_HANDLE;

                bool createInstance();

                // finds the extensions that need to be enabled for the instance
                void getInstanceExtensions(const char *const *& extensions, uint32_t& count);

                bool findVulkanExtensions(const std::vector<const char*>& extension_names);
                bool findVulkanLayers(const std::vector<const char*>& layer_names);

              public:
                // creating a surface to render on

                VkSurfaceKHR m_render_surface = VK_NULL_HANDLE;

                // details about the render surface
                VkSurfaceCapabilitiesKHR m_surface_capabilities;
                std::vector<VkSurfaceFormatKHR> m_surface_formats;
                std::vector<VkPresentModeKHR> m_surface_present_modes;

                // creates a surface to render to linked to the window
                bool createRenderSurface(graphics::Window& window);

                // asks vulkan for the capabilities of the surface in combination with a physical device
                bool checkSurfaceCapabilities(const VkPhysicalDevice& device);

              public:
                // choosing a physical device (aka graphics card)

                VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
                uint32_t m_graphics_queue_id = 0;
                uint32_t m_present_queue_id = 0;

                // true, if the device supports all extensions specified in REQUIRED_DEVICE_EXTENSIONS
                bool choosePhysicalDevice();

                // rates the physical device based on how good it suits the needs of the engine
                // a score of 0 means it wont work
                uint32_t ratePhysicalDevice(const VkPhysicalDevice& device);

                bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);

                bool initQueueIndices();

              public:
                // creating a logical device to interface with the physical device

                VkDevice m_logical_device = VK_NULL_HANDLE;
                VkQueue m_graphics_queue = VK_NULL_HANDLE;
                VkQueue m_present_queue = VK_NULL_HANDLE;

                bool createLogicalDevice();

                bool initQueues();

              public:
                // creating the swap chain

                VkSwapchainKHR m_swap_chain;

                VkSurfaceFormatKHR m_surface_format; // the surface format that is going to be used (out of m_surface_formats)
                VkPresentModeKHR m_present_mode;
                VkExtent2D m_swap_extent; // width and height of the swap images
                uint32_t m_swap_image_count = 0;

                // returns false if not all requirements (parameters) could be met
                // will most likely still work though
                bool createSwapChain(bool vsync, bool tripple_buffering, uint32_t width, uint32_t height);

                bool chooseSwapSurfaceFormat();

                // vsync: frames are rendered at the frequency at which they are presented (monitor refresh rate)
                // tripple_buffering: frames are rendered as frequently as possible, but only complete images are presented (less latency)
                // everything false: frames are rendered as fast as possible, visible tearing might occur
                bool chooseSwapPresentMode(bool vsync = false, bool tripple_buffering = true);

                // determines the size of the images stored in the swap chain
                // should match the size of the window that gets rendered to
                bool chooseSwapExtent(uint32_t width, uint32_t height);

                void chooseSwapImageCount();

              public:
                // GraphicsAPI interface functions

                // init the graphics api
                // @param window: the window on which the graphics api should draw stuff
                virtual bool init(graphics::Window* window);

                // terminate the graphics api
                virtual bool term();

                // prints the state of the api (running fine / error)
                virtual void printStatus();

                // anything other then 0 is something to worry about
                virtual int getError();
            };

        } // namespace vulkan

    } // namespace graphics

} // namespace undicht

#endif // VULKAN_GRAPHICS_API_H

#endif // GRAPHICS_API_H