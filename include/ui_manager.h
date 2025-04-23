#ifndef ZKETCH_UI_MANAGER_H // ui_manager.h
#define ZKETCH_UI_MANAGER_H

#include <vector>
#include <memory>
#include "element.h"

namespace zketch {

// Class untuk manager UI
class UIManager {
public:
    static UIManager& getInstance();
    
    // Lifecycle methods
    bool initialize();
    void update(float deltaTime);
    void render();
    void cleanup();
    
    // Element management
    void addElement(std::shared_ptr<Element> element);
    void removeElement(std::shared_ptr<Element> element);
    void clearElements();
    
    // Element lookup
    std::shared_ptr<Element> getElementById(const std::string& id);
    std::vector<std::shared_ptr<Element>> getElementsByType(const std::string& typeName);
    
    // Event handling
    bool handleMouseEvent(const MouseEvent& event, EventType type);
    bool handleKeyEvent(const KeyEvent& event, EventType type);
    
    // Focus management
    void setFocusedElement(std::shared_ptr<Element> element);
    std::shared_ptr<Element> getFocusedElement() const;
    void clearFocus();
    
private:
    UIManager();  // Singleton pattern
    ~UIManager();
    
    // Prevent copy
    UIManager(const UIManager&) = delete;
    UIManager& operator=(const UIManager&) = delete;
    
    std::vector<std::shared_ptr<Element>> elements_;
    std::weak_ptr<Element> focusedElement_;
    std::weak_ptr<Element> hoveredElement_;
};

} // namespace zketch

#endif // ZKETCH_UI_MANAGER_H