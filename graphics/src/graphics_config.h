#ifndef GRAPHICS_CONFIG_H
#define GRAPHICS_CONFIG_H


//#define PRE_CONFIG_OPENGL
#define PRE_CONFIG_VULKAN

#define DEBUG_MODE


#ifdef PRE_CONFIG_OPENGL

#define USE_GLFW // works for opengl and vulkan on both windows and linux
#define USE_GLAD // loads the functions for opengl
#define USE_GL33 // use opengl 3.3

#endif // PRE_CONFIG_OPENGL


#ifdef PRE_CONFIG_VULKAN

#define USE_GLFW // works for opengl and vulkan on both windows and linux
// for vulkan you need to install the sdk for vulkan from lunarg 
#define USE_VULKAN

#endif // PRE_CONFIG_VULKAN



#endif // GRAPHICS_CONFIG_H