#ifdef GRAPHICS_API_H // this file should only be included via include/window/window.h

#ifndef VULKAN_GRAPHICS_API_H
#define VULKAN_GRAPHICS_API_H

#include "vulkan/vulkan.h"

namespace undicht {

    namespace graphics {

        namespace vulkan {

            class GraphicsAPI : public interf::GraphicsAPI {

              public:
                // vulkan specific members

                VkInstance m_instance;

                // initializes the VkInstance (return true if all went well)
                bool loadInstanceData();

              public:
              
                // init the graphics api
                virtual bool init();

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