#include "../include/ui_manager.h"
#include <algorithm>

namespace zketch {

UIManager& UIManager::getInstance() {
    static UIManager instance;
    return instance;
}

UIManager::UIManager() {
}

UIManager::~UIManager() {
    cleanup();
}

bool UIManager::initialize() {
    // Initialize all elements
    for (auto& element : elements_) {
        element->initialize();
    }
    return true;
}

void UIManager::update(float deltaTime) {
    // Update all visible elements
    for (auto& element : elements_) {
        if (element->isVisible()) {
            element->update(deltaTime);
        }
    }
}

void UIManager::render() {
    // Render all visible elements from back to front
    for (auto& element : elements_) {
        if (element->isVisible()) {
            element->render();
        }
    }
}

void UIManager::cleanup() {
    // Cleanup all elements
    for (auto& element : elements_) {
        element->cleanup();
    }
    
    // Clear the elements list
    elements_.clear();
    
    // Clear focus
    focusedElement_.reset();
    hoveredElement_.reset();
}

void UIManager::addElement(std::shared_ptr<Element> element) {
    if (element) {
        elements_.push_back(element);
    }
}

void UIManager::removeElement(std::shared_ptr<Element> element) {
    if (!element) {
        return;
    }
    
    auto it = std::find(elements_.begin(), elements_.end(), element);
    if (it != elements_.end()) {
        elements_.erase(it);
    }
    
    // Check if this was the focused element
    if (!focusedElement_.expired() && focusedElement_.lock() == element) {
        clearFocus();
    }
    
    // Check if this was the hovered element
    if (!hoveredElement_.expired() && hoveredElement_.lock() == element) {
        hoveredElement_.reset();
    }
}

void UIManager::clearElements() {
    elements_.clear();
    clearFocus();
    hoveredElement_.reset();
}

std::shared_ptr<Element> UIManager::getElementById(const std::string& id) {
    for (auto& element : elements_) {
        if (element->getId() == id) {
            return element;
        }
        
        // TODO: Recursive search through child elements
    }
    
    return nullptr;
}

std::vector<std::shared_ptr<Element>> UIManager::getElementsByType(const std::string& typeName) {
    // This would require RTTI (Run-Time Type Information)
    // For now, return an empty vector
    return std::vector<std::shared_ptr<Element>>();
}

bool UIManager::handleMouseEvent(const MouseEvent& event, EventType type) {
    // Process from top to bottom (front to back)
    for (auto it = elements_.rbegin(); it != elements_.rend(); ++it) {
        if ((*it)->isVisible() && (*it)->isEnabled() && 
            (*it)->handleMouseEvent(event, type)) {
            return true;
        }
    }
    
    // No element handled the event
    return false;
}

bool UIManager::handleKeyEvent(const KeyEvent& event, EventType type) {
    // First send to focused element
    auto focusedElement = getFocusedElement();
    if (focusedElement && focusedElement->isVisible() && focusedElement->isEnabled() &&
        focusedElement->handleKeyEvent(event, type)) {
        return true;
    }
    
    // If not handled by focused element, try others
    for (auto it = elements_.rbegin(); it != elements_.rend(); ++it) {
        if (*it != focusedElement && (*it)->isVisible() && (*it)->isEnabled() && 
            (*it)->handleKeyEvent(event, type)) {
            return true;
        }
    }
    
    // No element handled the event
    return false;
}

void UIManager::setFocusedElement(std::shared_ptr<Element> element) {
    // Remove focus from current element
    auto currentElement = focusedElement_.lock();
    if (currentElement && currentElement != element) {
        currentElement->setFocused(false);
    }
    
    // Set focus to new element
    focusedElement_ = element;
    if (element) {
        element->setFocused(true);
    }
}

std::shared_ptr<Element> UIManager::getFocusedElement() const {
    return focusedElement_.lock();
}

void UIManager::clearFocus() {
    auto currentElement = focusedElement_.lock();
    if (currentElement) {
        currentElement->setFocused(false);
    }
    focusedElement_.reset();
}

} // namespace zketch