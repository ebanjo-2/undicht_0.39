#include "graphics_config.h"
#ifdef USE_VULKAN

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "set"

#include "debug.h"

#include "api/graphics_api.h"

namespace undicht {

    namespace graphics {

        namespace vulkan {

            // extensions needed if the engine wants to use vulkan
            const std::vector<const char *> REQUIRED_VULKAN_EXTENSIONS = {"VK_KHR_surface"};

            // extensions the gpu has to provide
            const std::vector<const char *> REQUIRED_DEVICE_EXTENSIONS = {"VK_KHR_swapchain"};

            // names of the layers that need to be found to enable validation layers
            const std::vector<const char *> VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

            ////////////////////////////////////////// creating a vulkan instance /////////////////////////////////////////

            bool GraphicsAPI::createInstance() {

                // these structs need to be filled to create the vulkan instance
                VkApplicationInfo app_info{};
                VkInstanceCreateInfo instance_info{};

                app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // type of the struct
                app_info.pApplicationName = "Hello World";
                app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                app_info.pEngineName = "No Engine";
                app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                app_info.apiVersion = VK_API_VERSION_1_0;

                instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                instance_info.pApplicationInfo = &app_info; // storing the info set above
                getInstanceExtensions(instance_info.ppEnabledExtensionNames, instance_info.enabledExtensionCount);
                instance_info.enabledLayerCount = 0;

                // checking for the required instance extensions
                if (!findVulkanExtensions(REQUIRED_VULKAN_EXTENSIONS)) {
                    UND_ERROR << "failed to find the necessary vulkan extensions\n";
                    return false;
                }

// enabling debugging mode
#ifdef DEBUG_MODE
                if (!findVulkanLayers(VALIDATION_LAYERS)) {
                    UND_WARNING << "failed to find the validation layers to enable debugging for vulkan\n";
                } else {
                    // enable validation layers
                    UND_LOG << "Enabling Debugging mode for Vulkan\n";
                    instance_info.enabledLayerCount = 1;
                    instance_info.ppEnabledLayerNames = VALIDATION_LAYERS.data();
                }
#endif // DEBUG_MODE

                // creating the instance
                if (vkCreateInstance(&instance_info, nullptr, &m_instance) != VK_SUCCESS) {
                    return false;
                }

                return true;
            }

            void GraphicsAPI::getInstanceExtensions(const char *const *&extensions, uint32_t &count) {
// finds the extensions that need to be enabled for the instance
#ifdef USE_GLFW
                extensions = glfwGetRequiredInstanceExtensions(&count);
#endif // USE_GLFW
            }

            bool GraphicsAPI::findVulkanExtensions(const std::vector<const char *> &extension_names) {

                // counting the available extensions
                uint32_t extension_count = 0;
                vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);

                // getting the available extensions
                std::vector<VkExtensionProperties> available_extensions(extension_count);
                vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());

                // checking for the required extensions
                for (const char *required : extension_names) {

                    bool extension_found = false;

                    for (const VkExtensionProperties &available : available_extensions) {

                        if (available.extensionName == std::string(required)) {
                            UND_LOG << "Found Vulkan Extension " << available.extensionName << "\n";
                            extension_found = true;
                            break;
                        }
                    }

                    if (!extension_found) {
                        UND_ERROR << "Cant find Vulkan Extension " << required << "\n";
                        return false;
                    }
                }

                return true;
            }

            bool GraphicsAPI::findVulkanLayers(const std::vector<const char *> &layer_names) {
                // the only use for layers i currently know is the validation layer,
                // which is basically a debug mode for vulkan
                // returns whether the layers were found

                // getting the instance layers available
                uint32_t layer_count;
                vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

                std::vector<VkLayerProperties> available_layers(layer_count);
                vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

                // checking if the validation layers are available
                for (const char *name : layer_names) {

                    bool layer_found = false;
                    for (const VkLayerProperties &layer : available_layers) {

                        if (layer.layerName == std::string(name)) {
                            layer_found = true;
                            break;
                        }
                    }

                    if (!layer_found)
                        return false;
                }

                return true;
            }

            //////////////////////////////////////// creating a surface to render on //////////////////////////////////////////

            bool GraphicsAPI::createRenderSurface(graphics::Window &window) {
                // creates a surface to render to linked to the window

                return window.createRenderSurface(m_instance, m_render_surface);
            }

            bool GraphicsAPI::checkSurfaceCapabilities(const VkPhysicalDevice &device) {
                // asks vulkan for the capabilities of the surface

                // capabilities
                vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_render_surface, &m_surface_capabilities);

                // supported formats
                uint32_t format_count;
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_render_surface, &format_count, nullptr);

                if (!format_count)
                    return false;

                m_surface_formats.resize(format_count);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_render_surface, &format_count, m_surface_formats.data());

                // supported present modes
                uint32_t present_mode_count;
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_render_surface, &present_mode_count, nullptr);

                if (!present_mode_count)
                    return false;

                m_surface_present_modes.resize(present_mode_count);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_render_surface, &present_mode_count, m_surface_present_modes.data());

                return true;
            }

            /////////////////////////////// choosing a physical device (aka graphics card) ////////////////////////////////////

            bool GraphicsAPI::choosePhysicalDevice() {

                // getting all available gpus
                uint32_t device_count = 0;
                vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);

                std::vector<VkPhysicalDevice> devices(device_count);
                vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

                // finding the best one
                uint32_t highest_score = 0;
                for (const VkPhysicalDevice &device : devices) {
                    uint32_t device_score = ratePhysicalDevice(device);
                    if (device_score > highest_score) {
                        if (!checkSurfaceCapabilities(device))
                            continue; // even though this device scored high, it wont work
                        highest_score = device_score;
                        m_physical_device = device;
                    }
                }

                return highest_score && initQueueIndices(); // if its still at 0 then there are no usable devices
            }

            uint32_t GraphicsAPI::ratePhysicalDevice(const VkPhysicalDevice &device) {
                // rates the physical device based on how good it suits the needs of the engine
                // a score of 0 means it wont work

                int score = 0;

                // getting information about the device
                VkPhysicalDeviceProperties device_properties;
                VkPhysicalDeviceFeatures device_features;
                vkGetPhysicalDeviceProperties(device, &device_properties);
                vkGetPhysicalDeviceFeatures(device, &device_features);

                // dedicated gpus are usually better
                if (device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                    score += 1000;

                // bigger texture size == more good
                score += device_properties.limits.maxImageDimension2D;

                // critical requirements for the device
                if (!device_features.geometryShader)
                    return 0;

                if (!checkDeviceExtensionSupport(device))
                    return 0;

                UND_LOG << "found gpu: " << device_properties.deviceName << ", score " << score << "\n";

                return score;
            }

            bool GraphicsAPI::checkDeviceExtensionSupport(const VkPhysicalDevice &device) {
                // true, if the device supports all extensions specified in REQUIRED_DEVICE_EXTENSIONS

                // getting the number of available extensions
                uint32_t extension_count;
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

                // getting the available extensions
                std::vector<VkExtensionProperties> available_extensions(extension_count);
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

                // checking the required extensions
                for (const char *required : REQUIRED_DEVICE_EXTENSIONS) {
                    bool extension_found = false;
                    for (VkExtensionProperties &extension : available_extensions) {
                        if (extension.extensionName == std::string(required)) {
                            extension_found = true;
                            break;
                        }
                    }
                    if (!extension_found)
                        return false; // this extension wasnt found
                }

                return true;
            }

            bool GraphicsAPI::initQueueIndices() {

                // getting the number of available queue families
                uint32_t queue_family_count = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, nullptr);

                // storing the data about the queue families
                std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
                vkGetPhysicalDeviceQueueFamilyProperties(m_physical_device, &queue_family_count, queue_families.data());

                // finding queue familys
                VkBool32 graphics = false, present = false; // whether the queues have been found
                uint32_t index = 0;
                for (const VkQueueFamilyProperties &queue_family : queue_families) {

                    if (!graphics && (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
                        graphics = true;
                        m_graphics_queue_id = index;
                    }

                    if (!present) {
                        vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, index, m_render_surface, &present);
                        if (present)
                            m_present_queue_id = index;
                    }

                    index++;
                }

                return graphics && present;
            }
            /////////////////////////// creating a logical device to interface with the physical device ///////////////////////

            bool GraphicsAPI::createLogicalDevice() {

                // to create a logical device you need to fill this struct
                VkDeviceCreateInfo create_info{};
                create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

                // specifying the queue families this device needs
                std::set<uint32_t> queues = {m_graphics_queue_id, m_present_queue_id}; // the set makes sure that there are no duplicate ids
                std::vector<VkDeviceQueueCreateInfo> queue_create_infos;

                float queue_priority = 1.0f;

                for (const uint32_t &queue_id : queues) {
                    // for each queue this struct needs to be filled
                    VkDeviceQueueCreateInfo queue_create_info{};
                    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    queue_create_info.queueFamilyIndex = queue_id;
                    queue_create_info.queueCount = 1;
                    queue_create_info.pQueuePriorities = &queue_priority;

                    queue_create_infos.push_back(queue_create_info);
                }

                create_info.pQueueCreateInfos = queue_create_infos.data();
                create_info.queueCreateInfoCount = queue_create_infos.size();

                // specifying the device features that are going to be used
                VkPhysicalDeviceFeatures deviceFeatures{};
                create_info.pEnabledFeatures = &deviceFeatures;

                // specifying extensions and validation layers
                create_info.enabledExtensionCount = REQUIRED_DEVICE_EXTENSIONS.size();
                create_info.ppEnabledExtensionNames = REQUIRED_DEVICE_EXTENSIONS.data();
                create_info.enabledLayerCount = 0; // validation layers dont need to be created on logical devices anymore (i think)

                if (vkCreateDevice(m_physical_device, &create_info, nullptr, &m_logical_device) != VK_SUCCESS) {
                    return false;
                }

                return true && initQueues();
            }

            bool GraphicsAPI::initQueues() {

                vkGetDeviceQueue(m_logical_device, m_graphics_queue_id, 0, &m_graphics_queue);
                vkGetDeviceQueue(m_logical_device, m_present_queue_id, 0, &m_present_queue);

                return true;
            }

            ///////////////////////////////////////////// creating the swap chain /////////////////////////////////////////////

            bool GraphicsAPI::createSwapChain(bool vsync, bool tripple_buffering, uint32_t width, uint32_t height) {
                // returns false if not all requirements (parameters) could be met
                // will most likely still work though

                // the support for swap chains has already been checked when picking the physical device

                // choosing the right settings
                bool return_value = true;

                if (!chooseSwapSurfaceFormat())
                    return_value = false;

                if (!chooseSwapPresentMode(false, true))
                    return_value = false;

                if (!chooseSwapExtent(width, height))
                    return_value = false;

                chooseSwapImageCount();

                // actually creating the swap chain (surprise: we need to fill a struct)
                VkSwapchainCreateInfoKHR create_info{};
                create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
                create_info.surface = m_render_surface;
                create_info.minImageCount = m_swap_image_count;
                create_info.imageFormat = m_surface_format.format;
                create_info.imageColorSpace = m_surface_format.colorSpace;
                create_info.imageExtent = m_swap_extent;
                create_info.imageArrayLayers = 1;
                create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
                create_info.presentMode = m_present_mode;

                // info about which queue family uses the swap images
                uint32_t queue_family_indices[] = {m_graphics_queue_id, m_present_queue_id};
                if (m_graphics_queue_id != m_present_queue_id) {
                    create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // ownership of the swap images is shared, preformance might suffer
                    create_info.queueFamilyIndexCount = 2;
                    create_info.pQueueFamilyIndices = queue_family_indices; // specifying which queues share the image
                } else {                                                    // the graphics queue is in the same queue family as the present queue, no need for shared ownership
                    create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                    create_info.queueFamilyIndexCount = 0;     // Optional
                    create_info.pQueueFamilyIndices = nullptr; // Optional
                }

                // specifying whether an transformation should take place before displaying the image (i.e. 90 degree turn)
                create_info.preTransform = m_surface_capabilities.currentTransform; // just use the current transformation

                // blending with other windows
                create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

                // we dont need pixels obscured by other windows
                create_info.clipped = VK_TRUE;

                // needed for when the window gets resized and a new swapchain needs to be build
                create_info.oldSwapchain = VK_NULL_HANDLE;

                if (vkCreateSwapchainKHR(m_logical_device, &create_info, nullptr, &m_swap_chain) != VK_SUCCESS) {
                    UND_ERROR << "Failed to create swap chain\n";
                    return_value = false;
                }

                return return_value;
            }

            bool GraphicsAPI::chooseSwapSurfaceFormat() {

                // preferred color formats (in descending order)
                std::vector<VkFormat> preferred_color_formats = {// has higher prio then color space
                                                                 VK_FORMAT_B8G8R8A8_SRGB};

                // preferred color space
                std::vector<VkColorSpaceKHR> preferred_color_spaces = {
                    VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};

                // finding the preferred surface format
                for (const VkFormat &color_format : preferred_color_formats) {
                    for (const VkColorSpaceKHR &color_space : preferred_color_spaces) {
                        // going through the available surface formats
                        for (const VkSurfaceFormatKHR &surface_format : m_surface_formats) {
                            if (surface_format.format == color_format && surface_format.colorSpace == color_space) {
                                m_surface_format = surface_format;
                                return true;
                            }
                        }
                    }
                }

                // just choosing the first one (should still result in something being drawn)
                UND_WARNING << "no preferred surface format is available, using default one\n";
                m_surface_format = m_surface_formats[0];

                return false;
            }

            bool GraphicsAPI::chooseSwapPresentMode(bool vsync, bool tripple_buffering) {
                // vsync: frames are rendered at the frequency at which they are presented (monitor refresh rate)
                // tripple_buffering: frames are rendered as frequently as possible, but only complete images are presented (less latency)
                // everything false: frames are rendered as fast as possible, visible tearing might occur

                // translating the parameters into a present mode
                VkPresentModeKHR present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR; // immediate display of new frames
                if (vsync)
                    present_mode = VK_PRESENT_MODE_FIFO_KHR;
                if (tripple_buffering)
                    present_mode = VK_PRESENT_MODE_MAILBOX_KHR;

                for (VkPresentModeKHR &available : m_surface_present_modes) {
                    if (available == present_mode) {
                        m_present_mode = present_mode;
                        return true;
                    }
                }

                // only mode that is guaranteed to be there (vsync on)
                UND_WARNING << "vsync is the only available option\n";
                m_present_mode = VK_PRESENT_MODE_FIFO_KHR;

                return false;
            }

            bool GraphicsAPI::chooseSwapExtent(uint32_t width, uint32_t height) {
                // determines the size of the images stored in the swap chain
                // should match the size of the window that gets rendered to

                bool return_value = true;

                m_swap_extent.width = width;
                m_swap_extent.height = height;

                // checking if the requested dimensions are allowed
                if (m_surface_capabilities.maxImageExtent.width < width) {
                    m_swap_extent.width = m_surface_capabilities.maxImageExtent.width;
                    return_value = false;
                }

                if (m_surface_capabilities.maxImageExtent.height < height) {
                    m_swap_extent.height = m_surface_capabilities.maxImageExtent.height;
                    return_value = false;
                }

                return return_value;
            }

            void GraphicsAPI::chooseSwapImageCount() {

                // choosing one more image then necessary because vulkan tutorial said so
                m_swap_image_count = m_surface_capabilities.minImageCount + 1;

                // there might be a limit to how many images there can be
                if ((m_surface_capabilities.maxImageCount != 0) && (m_surface_capabilities.maxImageCount < m_swap_image_count)) {
                    // maxImageCount of 0 means there is no maximum
                    m_swap_image_count = m_surface_capabilities.maxImageCount;
                }
            }

            ///////////////////////////////////////// implementing the interf methods /////////////////////////////////////////

            bool GraphicsAPI::init(graphics::Window *window) {
                // init the graphics api
                // @param window: the window on which the graphics api should draw stuff

                int window_width, window_height;
                window->getSize(window_width, window_height);

                if (!createInstance()) {
                    UND_ERROR << "Failed to create vulkan Instance\n";
                    return false;
                }

                if (!createRenderSurface(*window)) {
                    UND_ERROR << "Failed to create render surface on window\n";
                    return false;
                }

                if (!choosePhysicalDevice()) {
                    UND_ERROR << "Failed to find a gpu with vulkan support\n";
                    return false;
                }

                if (!createLogicalDevice()) {
                    UND_ERROR << "Failed to create logical device\n";
                    return false;
                }

                if (!createSwapChain(false, true, window_width, window_height)) {
                    UND_ERROR << "Failed to create swap chain with the requested parameters, visual result might be different then expected\n";
                    return false;
                }

                return true;
            }

            bool GraphicsAPI::term() {
                // terminate the graphics api

                vkDestroyDevice(m_logical_device, nullptr);
                vkDestroySurfaceKHR(m_instance, m_render_surface, nullptr);
                vkDestroyInstance(m_instance, nullptr);

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