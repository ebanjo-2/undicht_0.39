#include <iostream>
#include <vector>


#include "types.h"
#include "buffer_layout.h"
#include "undicht_thread.h"

#include "debug.h"

using namespace undicht;

void thread_function() {

    std::cout << "Hello Second World" << "\n";
    std::cout << "test" << "\n";
}

int main() {

    Thread thread(thread_function);

    FixedType t = UND_INT32; // 32 bit int
    BufferLayout layout({UND_VEC3F, UND_FLOAT32});

    if(t == Type::INT) {

       UND_LOG << "is int" << "\n";
    }

    UND_ERROR << "Failed to initialize Engine\n";
    UND_LOG << "This is to be expected\n";
    UND_WARNING <<"This is just a Warning, you can just ignore it\n";
    
    std::cout << "data struct size: " << layout.getTotalSize() << "\n";

    thread.join();

    return 0;
}