#include <iostream>
#include <vector>


#include "types.h"
#include "buffer_layout.h"
#include "undicht_thread.h"

#include "debug.h"

#include "window/window.h"
#include "api/graphics_api.h"

using namespace undicht;
using namespace graphics;

int main() {

    Window window(800, 600, "Hello World");
    GraphicsAPI vulkan;

    if(!vulkan.init(&window)) {

        UND_ERROR << "Failed to initialize Vulkan\n";
    }

    while(!window.shouldClose()) {
        
        window.update();
    }

    vulkan.term();
    window.close();

    return 0;
}