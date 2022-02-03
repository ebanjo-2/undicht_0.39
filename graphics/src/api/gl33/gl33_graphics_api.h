#ifdef GRAPHICS_API_H // this file should only be included via include/window/window.h

#ifndef GL33_GRAPHICS_API_H
#define GL33_GRAPHICS_API_H

namespace undicht {

    namespace graphics {

        namespace gl33 {

            class GraphicsAPI : public interf::GraphicsAPI {

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

        } // namespace gl33

    } // namespace graphics

} // namespace undicht

#endif // GL33_GRAPHICS_API_H

#endif // GRAPHICS_API_H