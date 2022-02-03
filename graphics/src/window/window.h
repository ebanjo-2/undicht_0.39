#ifndef WINDOW_H
#define WINDOW_H

#include "graphics_config.h"

namespace undicht {

    namespace graphics {

        namespace interf {
            // the namespace in which the classes are merely described but never implemented
            // the implementation is then done in namespace hinting at what library was used
            // depending on the configuration of the engine such an implementation is then adopted
            // in the main namespace (in this case undicht::graphics)

            class Window {
              public:
                Window() = default;
                Window(int width, int height, const char *title);
                virtual ~Window() = default;

                /** @brief opens the window (what else?)
				* @return false, if the window could not be created 
				* the first window opened will also initialize the graphics library
				* and the last window close will also terminate the graphics
                * objects using the lib should only be initialized after the window was opened and closed before it is closed*/
                virtual bool open() = 0;

                virtual bool open(int width, int height, const char *title) = 0;

                virtual void close() = 0;

                virtual void update() = 0;

                /** \return whether a close was requested by the user */
                virtual bool shouldClose() = 0;

                virtual void setSize(int width, int height) = 0;

                virtual void getSize(int &width, int &height) = 0;

                virtual void setTitle(const char *title) = 0;

                virtual void setWindowMode(bool fullscreen, bool windowed) = 0;

                virtual void setCursorVisible(bool visible) = 0;

                virtual bool getCursorVisible() = 0;
            };

        } // namespace interf

    } // namespace graphics

} // namespace undicht

// adopting implemented classes into the main namespace
#if defined(USE_GLFW)
#include "window/glfw/glfw_window.h"

namespace undicht {

    namespace graphics {

        // bringing the glfw implementation into the main namespace
        typedef glfw::Window Window;

    } // namespace graphics

} // namespace undicht

#endif // GLFW

#endif // WINDOW_H