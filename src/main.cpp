// No main() function here
// Instead, expose an "application" function

#include "zketch.h"

// This is called by the platform-specific entry point
bool zketch_app_initialize() {
    // Initialize UI elements
    auto& uiManager = zketch::UIManager::getInstance();
    
    // Create elements
    auto rect = std::make_shared<zketch::RectElement>(50, 50, 200, 100);
    // ...
    
    uiManager.addElement(rect);
    return true;
}

void zketch_app_update(float deltaTime) {
    // Update application logic
}

void zketch_app_cleanup() {
    // Clean up resources
}

int main(){
	if(!zketch_app_initialize()){
		cerr << "gagal initialisasi zketch" << endl;
	}
}