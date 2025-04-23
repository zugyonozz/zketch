#include "../include/element.h"
#include "../include/ui_manager.h"
#include <algorithm>

namespace zketch {

// ========== Base Element Implementation ==========

Element::Element()
    : position_(0, 0),
      size_(0, 0),
      visible_(true),
      enabled_(true),
      id_(""),
      hovered_(false),
      focused_(false),
      active_(false),
      parent_(nullptr) {
}

Element::~Element() {
    // Clear event listeners
    eventListeners_.clear();
    
    // Clear children (shared_ptr will handle cleanup)
    children_.clear();
}

void Element::initialize() {
    // Initialize all children
    for (auto& child : children_) {
        child->initialize();
    }
}

void Element::update(float deltaTime) {
    // Update all children
    for (auto& child : children_) {
        if (child->isVisible()) {
            child->update(deltaTime);
        }
    }
}

void Element::render() {
    // Base render doesn't do anything
    // This should be overridden by derived classes
    
    // Render all visible children
    for (auto& child : children_) {
        if (child->isVisible()) {
            child->render();
        }
    }
}

void Element::cleanup() {
    // Cleanup all children
    for (auto& child : children_) {
        child->cleanup();
    }
}

void Element::setPosition(float x, float y) {
    position_.x = x;
    position_.y = y;
}

void Element::setSize(float width, float height) {
    size_.width = width;
    size_.height = height;
}

void Element::setStyle(const Style& style) {
    style_ = style;
}

void Element::setVisible(bool visible) {
    visible_ = visible;
}

void Element::setEnabled(bool enabled) {
    enabled_ = enabled;
}

void Element::setId(const std::string& id) {
    id_ = id;
}

Point Element::getPosition() const {
    return position_;
}

Size Element::getSize() const {
    return size_;
}

Rect Element::getBounds() const {
    return Rect(position_.x, position_.y, size_.width, size_.height);
}

const Style& Element::getStyle() const {
    return style_;
}

bool Element::isVisible() const {
    return visible_;
}

bool Element::isEnabled() const {
    return enabled_;
}

std::string Element::getId() const {
    return id_;
}

bool Element::handleEvent(const Event& event) {
    // Process event for this element
    // and dispatch to listener if exists
    auto it = eventListeners_.find(event.type);
    if (it != eventListeners_.end()) {
        it->second(event, this);
        return true;
    }
    
    // Event not handled by this element
    return false;
}

bool Element::handleMouseEvent(const MouseEvent& event, EventType type) {
    // Check if event is within bounds
    if (!isVisible() || !isEnabled()) {
        return false;
    }
    
    Rect bounds = getBounds();
    bool isInside = bounds.contains(Point(event.position.x, event.position.y));
    
    // Handle hover state
    if (type == EventType::MouseMove) {
        bool wasHovered = hovered_;
        hovered_ = isInside;
        
        if (hovered_ && !wasHovered) {
            // MouseEnter
            Event enterEvent(EventType::MouseEnter);
            handleEvent(enterEvent);
        } else if (!hovered_ && wasHovered) {
            // MouseLeave
            Event leaveEvent(EventType::MouseLeave);
            handleEvent(leaveEvent);
        }
    }
    
    // Handle click/active state
    if (isInside) {
        if (type == EventType::MouseDown) {
            active_ = true;
            
            // Request focus
            UIManager::getInstance().setFocusedElement(std::shared_ptr<Element>(this, [](Element*){})); // No-op deleter since we don't own this
        } else if (type == EventType::MouseUp) {
            active_ = false;
        }
        
        // Dispatch event
        Event e(type);
        return handleEvent(e);
    }
    
    // Check children
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if ((*it)->handleMouseEvent(event, type)) {
            return true;
        }
    }
    
    return false;
}

bool Element::handleKeyEvent(const KeyEvent& event, EventType type) {
    // Only process key events if focused
    if (!isVisible() || !isEnabled() || !isFocused()) {
        return false;
    }
    
    // Dispatch event
    Event e(type);
    if (handleEvent(e)) {
        return true;
    }
    
    // Check children
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if ((*it)->handleKeyEvent(event, type)) {
            return true;
        }
    }
    
    return false;
}

void Element::addChild(std::shared_ptr<Element> child) {
    if (!child) {
        return;
    }
    
    // Remove from previous parent
    if (child->getParent()) {
        child->getParent()->removeChild(child);
    }
    
    // Set parent and add to children list
    child->setParent(this);
    children_.push_back(child);
}

void Element::removeChild(std::shared_ptr<Element> child) {
    if (!child) {
        return;
    }
    
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        (*it)->setParent(nullptr);
        children_.erase(it);
    }
}

void Element::clearChildren() {
    for (auto& child : children_) {
        child->setParent(nullptr);
    }
    children_.clear();
}

void Element::addEventListener(EventType type, EventListener listener) {
    eventListeners_[type] = listener;
}

void Element::removeEventListener(EventType type) {
    eventListeners_.erase(type);
}

void Element::setParent(Element* parent) {
    parent_ = parent;
}

Element* Element::getParent() const {
    return parent_;
}

bool Element::isHovered() const {
    return hovered_;
}

bool Element::isFocused() const {
    return focused_;
}

bool Element::isActive() const {
    return active_;
}

void Element::setHovered(bool hovered) {
    hovered_ = hovered;
}

void Element::setFocused(bool focused) {
    focused_ = focused;
    
    if (focused) {
        Event e(EventType::Focus);
        handleEvent(e);
    } else {
        Event e(EventType::Blur);
        handleEvent(e);
    }
}

void Element::setActive(bool active) {
    active_ = active;
}

Style Element::getEffectiveStyle() const {
    Style result = style_;
    
    // Apply state styles
    if (isActive() && style_.getActiveStyle()) {
        result.merge(*style_.getActiveStyle());
    } else if (isFocused() && style_.getFocusStyle()) {
        result.merge(*style_.getFocusStyle());
    } else if (isHovered() && style_.getHoverStyle()) {
        result.merge(*style_.getHoverStyle());
    }
    
    return result;
}

void Element::dispatchEvent(const Event& event) {
    auto it = eventListeners_.find(event.type);
    if (it != eventListeners_.end()) {
        it->second(event, this);
    }
}

// ========== RectElement Implementation ==========

RectElement::RectElement() : Element() {
}

RectElement::RectElement(float x, float y, float width, float height) : Element() {
    setPosition(x, y);
    setSize(width, height);
}

RectElement::~RectElement() {
}

void RectElement::render() {
    // Implementasi render untuk rectangle akan bergantung pada platform rendering
    // Untuk sekarang kita hanya memberikan placeholder implementasi
    
    // Implementasi Win32 akan ditambahkan di platform/win32/main.cpp
    
    // Render children
    Element::render();
}

// ========== LineElement Implementation ==========

LineElement::LineElement() : Element(), startPoint_(0, 0), endPoint_(0, 0) {
}

LineElement::LineElement(float x1, float y1, float x2, float y2) 
    : Element(), startPoint_(x1, y1), endPoint_(x2, y2) {
    
    // Calculate bounds to encompass the line
    float minX = std::min(x1, x2);
    float minY = std::min(y1, y2);
    float maxX = std::max(x1, x2);
    float maxY = std::max(y1, y2);
    
    setPosition(minX, minY);
    setSize(maxX - minX, maxY - minY);
}

LineElement::~LineElement() {
}

void LineElement::setPoints(float x1, float y1, float x2, float y2) {
    startPoint_.x = x1;
    startPoint_.y = y1;
    endPoint_.x = x2;
    endPoint_.y = y2;
    
    // Update bounds
    float minX = std::min(x1, x2);
    float minY = std::min(y1, y2);
    float maxX = std::max(x1, x2);
    float maxY = std::max(y1, y2);
    
    setPosition(minX, minY);
    setSize(maxX - minX, maxY - minY);
}

void LineElement::setStartPoint(float x, float y) {
    startPoint_.x = x;
    startPoint_.y = y;
    
    // Update bounds
    float minX = std::min(x, endPoint_.x);
    float minY = std::min(y, endPoint_.y);
    float maxX = std::max(x, endPoint_.x);
    float maxY = std::max(y, endPoint_.y);
    
    setPosition(minX, minY);
    setSize(maxX - minX, maxY - minY);
}

void LineElement::setEndPoint(float x, float y) {
    endPoint_.x = x;
    endPoint_.y = y;
    
    // Update bounds
    float minX = std::min(startPoint_.x, x);
    float minY = std::min(startPoint_.y, y);
    float maxX = std::max(startPoint_.x, x);
    float maxY = std::max(startPoint_.y, y);
    
    setPosition(minX, minY);
    setSize(maxX - minX, maxY - minY);
}

Point LineElement::getStartPoint() const {
    return startPoint_;
}

Point LineElement::getEndPoint() const {
    return endPoint_;
}

void LineElement::render() {
    // Implementasi render untuk line akan bergantung pada platform rendering
    // Untuk sekarang kita hanya memberikan placeholder implementasi
    
    // Implementasi Win32 akan ditambahkan di platform/win32/main.cpp
    
    // Render children
    Element::render();
}

// ========== TextElement Implementation ==========

TextElement::TextElement() : Element(), text_("") {
}

TextElement::TextElement(const std::string& text, float x, float y) 
    : Element(), text_(text) {
    setPosition(x, y);
    // Size will depend on the text metrics, which are platform dependent
}

TextElement::~TextElement() {
}

void TextElement::setText(const std::string& text) {
    text_ = text;
    // Update size based on text metrics (platform dependent)
}

std::string TextElement::getText() const {
    return text_;
}

void TextElement::render() {
    // Implementasi render untuk text akan bergantung pada platform rendering
    // Untuk sekarang kita hanya memberikan placeholder implementasi
    
    // Implementasi Win32 akan ditambahkan di platform/win32/main.cpp
    
    // Render children
    Element::render();
}

// ========== InputBox Implementation ==========

InputBox::InputBox() 
    : Element(),
      text_(""),
      placeholder_(""),
      maxLength_(100),
      passwordMode_(false),
      cursorPosition_(0),
      onTextChanged_(nullptr) {
    
    // Set default style for input box
    Style style;
    style.setBackgroundColor(Color(255, 255, 255))  // White background
         .setBorderColor(Color(200, 200, 200))      // Light gray border
         .setBorderWidth(1)
         .setBorderStyle(BorderStyle::Solid)
         .setBorderRadius(2)
         .setPadding(5);
    
    Style hoverStyle;
    hoverStyle.setBorderColor(Color(150, 150, 150)); // Darker border on hover
    style.setHoverStyle(hoverStyle);
    
    Style focusStyle;
    focusStyle.setBorderColor(Color(100, 150, 255))  // Blue border when focused
             .setBorderWidth(2);
    style.setFocusStyle(focusStyle);
    
    setStyle(style);
}

InputBox::InputBox(float x, float y, float width, float height) 
    : InputBox() {
    setPosition(x, y);
    setSize(width, height);
}

InputBox::~InputBox() {
}

void InputBox::setText(const std::string& text) {
    if (text != text_) {
        text_ = text;
        cursorPosition_ = text_.length();
        
        if (onTextChanged_) {
            onTextChanged_(this, text_);
        }
    }
}

std::string InputBox::getText() const {
    return text_;
}

void InputBox::setPlaceholder(const std::string& placeholder) {
    placeholder_ = placeholder;
}

std::string InputBox::getPlaceholder() const {
    return placeholder_;
}

void InputBox::setMaxLength(size_t maxLength) {
    maxLength_ = maxLength;
    if (text_.length() > maxLength_) {
        setText(text_.substr(0, maxLength_));
    }
}

size_t InputBox::getMaxLength() const {
    return maxLength_;
}

void InputBox::setPasswordMode(bool passwordMode) {
    passwordMode_ = passwordMode;
}

bool InputBox::isPasswordMode() const {
    return passwordMode_;
}

bool InputBox::handleKeyEvent(const KeyEvent& event, EventType type) {
    if (!isVisible() || !isEnabled() || !isFocused()) {
        return false;
    }
    
    if (type == EventType::KeyDown) {
        // Handle special keys
        if (event.keyCode == 8) {  // Backspace
            deleteCharacter(true);
            return true;
        } else if (event.keyCode == 46) {  // Delete
            deleteCharacter(false);
            return true;
        } else if (event.keyCode == 37) {  // Left arrow
            if (cursorPosition_ > 0) {
                cursorPosition_--;
            }
            return true;
        } else if (event.keyCode == 39) {  // Right arrow
            if (cursorPosition_ < text_.length()) {
                cursorPosition_++;
            }
            return true;
        } else if (event.keyCode == 36) {  // Home
            cursorPosition_ = 0;
            return true;
        } else if (event.keyCode == 35) {  // End
            cursorPosition_ = text_.length();
            return true;
        } else if (event.keyCode >= 32 && event.keyCode <= 126) {  // Printable ASCII
            char c = static_cast<char>(event.keyCode);
            if (event.shift) {
                // Handle shift for uppercase letters and special characters
                if (c >= 'a' && c <= 'z') {
                    c = c - 'a' + 'A';  // Convert to uppercase
                } else {
                    // Handle special characters
                    switch (c) {
                        case '1': c = '!'; break;
                        case '2': c = '@'; break;
                        case '3': c = '#'; break;
                        case '4': c = '$'; break;
                        case '5': c = '%'; break;
                        case '6': c = '^'; break;
                        case '7': c = '&'; break;
                        case '8': c = '*'; break;
                        case '9': c = '('; break;
                        case '0': c = ')'; break;
                        case '-': c = '_'; break;
                        case '=': c = '+'; break;
                        case '[': c = '{'; break;
                        case ']': c = '}'; break;
                        case '\\': c = '|'; break;
                        case ';': c = ':'; break;
                        case '\'': c = '"'; break;
                        case ',': c = '<'; break;
                        case '.': c = '>'; break;
                        case '/': c = '?'; break;
                        case '`': c = '~'; break;
                    }
                }
            }
            insertCharacter(c);
            return true;
        }
    }
    
    // Call base implementation
    return Element::handleKeyEvent(event, type);
}

bool InputBox::handleMouseEvent(const MouseEvent& event, EventType type) {
    bool handled = Element::handleMouseEvent(event, type);
    
    if (isVisible() && isEnabled() && 
        type == EventType::MouseDown && 
        getBounds().contains(event.position)) {
        
        updateCursorPosition(event.position);
        return true;
    }
    
    return handled;
}

void InputBox::render() {
    // Implementasi render untuk input box akan bergantung pada platform rendering
    // Untuk sekarang kita hanya memberikan placeholder implementasi
    
    // Implementasi Win32 akan ditambahkan di platform/win32/main.cpp
    
    // Render children
    Element::render();
}

void InputBox::setOnTextChanged(TextChangeCallback callback) {
    onTextChanged_ = callback;
}

void InputBox::updateCursorPosition(const Point& mousePos) {
    // This is a simplified implementation - actual cursor position
    // calculation would require measuring character widths which is platform dependent
    
    // For now, we'll just use a simple approximation
    float relativeX = mousePos.x - position_.x - style_.getPaddingLeft();
    float charWidth = 8.0f;  // Assume fixed width font with 8px per character
    
    cursorPosition_ = static_cast<size_t>(relativeX / charWidth);
    
    // Clamp cursor position
    if (cursorPosition_ > text_.length()) {
        cursorPosition_ = text_.length();
    }
}

void InputBox::insertCharacter(char c) {
    if (text_.length() >= maxLength_) {
        return;
    }
    
    text_.insert(cursorPosition_, 1, c);
    cursorPosition_++;
    
    if (onTextChanged_) {
        onTextChanged_(this, text_);
    }
}

void InputBox::deleteCharacter(bool backspace) {
    if (backspace) {
        // Delete character to the left of cursor
        if (cursorPosition_ > 0) {
            text_.erase(cursorPosition_ - 1, 1);
            cursorPosition_--;
            
            if (onTextChanged_) {
                onTextChanged_(this, text_);
            }
        }
    } else {
        // Delete character to the right of cursor
        if (cursorPosition_ < text_.length()) {
            text_.erase(cursorPosition_, 1);
            
            if (onTextChanged_) {
                onTextChanged_(this, text_);
            }
        }
    }
}

} // namespace zketch