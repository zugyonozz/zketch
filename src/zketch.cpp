#include "../include/zketch.h"

namespace zketch {

const char* VERSION = "0.1.0";

// Global initialization function
bool initialize() {
    // Add any necessary global initialization code here
    // This might include system resources, graphics subsystems, etc.
    
    // Return true if initialization was successful
    return true;
}

// Global cleanup function
void cleanup() {
    // Add cleanup code to release resources allocated in initialize()
    // This might include freeing system resources, etc.
}

} // namespace zketch