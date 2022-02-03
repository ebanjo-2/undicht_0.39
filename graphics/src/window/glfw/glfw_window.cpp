#include <graphics_config.h>
#ifdef USE_GLFW

#include "debug.h"
#include <iostream>
#include <window/window.h>

namespace undicht {

    namespace graphics {

        namespace glfw {

            Window::Window() {
            }

            Window::Window(int width, int height, const char *title) {

                open(width, height, title);
            }

            Window::~Window() {
                // doing nothing

                close();
            }

            bool Window::open() {
                /** opens the window
                * @return false, if the window could not be created */

                // initializing glfw
                glfwInit();

                // graphic api specific hints
#ifdef USE_GL33

                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#endif // USE_GL33

#ifdef USE_VULKAN

                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // temporary. will need some work later

#endif // USE_VULKAN

                // opening the window
                m_window = glfwCreateWindow(m_width, m_height, m_title.data(), NULL, NULL); //glfwGetPrimaryMonitor() for fullscreen

                if (m_window == NULL) {
                    UND_ERROR << "FAILED TO CREATE WINDOW"
                              << "\n";

                    return false;
                }

                glfwMakeContextCurrent(m_window);

                return true;
            }

            bool Window::open(int width, int height, const char *title) {
                setSize(width, height);
                setTitle(title);
                return open();
            }

            void Window::close() {
                // terminate the graphics lib

                glfwDestroyWindow(m_window);

                glfwTerminate();
            }

            void Window::update() {
                getSize(m_width, m_height);
                glfwPollEvents();
                glfwSwapBuffers(m_window);
            }

            bool Window::shouldClose() {
                /** @return whether a close was requested by the user */

                return glfwWindowShouldClose(m_window);
            }

            void Window::setSize(int width, int height) {

                m_width = width;
                m_height = height;

                if (m_window) {

                    glfwSetWindowSize(m_window, width, height);

                } // to be done
            }

            void Window::getSize(int &width, int &height) {

                glfwGetWindowSize(m_window, &width, &height);
            }

            void Window::setTitle(const char *title) {

                m_title = title;

                if (m_window) {
                    glfwSetWindowTitle(m_window, title);
                }
            }

            void Window::setWindowMode(bool fullscreen, bool windowed) {
                // still to be done

                if (windowed) {

                    if (fullscreen) {
                        // window without border the size of the screen

                        GLFWmonitor *monitor = glfwGetPrimaryMonitor();

                        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

                        glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

                    } else {
                        // window with default borders

                        glfwSetWindowMonitor(m_window, 0, 0, 0, m_width, m_height, 0);
                    }

                } else {

                    if (fullscreen) {
                        // true fullscreen mode

                        GLFWmonitor *monitor = glfwGetPrimaryMonitor();

                        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

                        glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

                    } else {
                        // borderless window, keeping the size
                        // not sure how to do this (after approx. 30 sec of research i couldnt find an easy result)
                    }
                }
            }

            void Window::setCursorVisible(bool visible) {

                m_cursor_visible = visible;

                if (visible) {
                    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                } else {
                    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //cursor disappears
                }
            }

            bool Window::getCursorVisible() {

                return m_cursor_visible;
            }
        } // namespace glfw

    } // namespace graphics

} // namespace undicht

#endif // USE_GLFW