#ifndef GRAPHICS_API_H
#define GRAPHICS_API_H

#include "graphics_config.h"

namespace undicht {

    namespace graphics {

        namespace interf {

            class GraphicsAPI {

              public:
                // init the graphics api
                virtual bool init() = 0;

                // terminate the graphics api
                virtual bool term() = 0;

                // prints the state of the api (running fine / error)
                virtual void printStatus() = 0;

                // anything other then 0 is something to worry about
                virtual int getError() = 0;
            };

        } // namespace interf

    } // namespace graphics

} // namespace undicht


#ifdef USE_GL33
#include "api/gl33/gl33_graphics_api.h"

namespace undicht {
    namespace graphics {
        typedef gl33::GraphicsAPI GraphicsAPI;
    }
}
#endif // USE_GL33

#ifdef USE_VULKAN
#include "api/vulkan/vulkan_graphics_api.h"

namespace undicht {
    namespace graphics {
        typedef vulkan::GraphicsAPI GraphicsAPI;
    }
}
#endif // USE_VULKAN

#endif // GRAPHICS_API_H