#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

// macros for logging and debugging

#define UND_ERROR std::cout << "ERROR: " <<  " from " << __FILE__ << "\n    "

#define UND_WARNING std::cout << "WARNING: " << " from " << __FILE__ << "\n    "

#define UND_LOG std::cout << "Note: " << "\n    "

#endif // DEBUG_H