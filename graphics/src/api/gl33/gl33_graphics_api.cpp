#include "graphics_config.h"
#ifdef USE_GL33

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "debug.h"

#include "api/graphics_api.h"

namespace undicht {

    namespace graphics {

        namespace gl33 {

            bool GraphicsAPI::init() {
                // init the graphics api

                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) { //loading all opengl functions

                    UND_ERROR << "Failed to initialize GLAD"
                              << "\n";

                    return false;
                }

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

        } // namespace gl33

    } // namespace graphics

} // namespace undicht

#endif // USE_VULKAN