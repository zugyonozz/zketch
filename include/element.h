#ifndef ZKETCH_ELEMENT_H // element.h
#define ZKETCH_ELEMENT_H

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "style.h"

namespace zketch {

// Forward declarations
class UIManager;

// Struct untuk posisi
struct Point {
    float x, y;
    
    Point() : x(0), y(0) {}
    Point(float x, float y) : x(x), y(y) {}
};

// Struct untuk ukuran
struct Size {
    float width, height;
    
    Size() : width(0), height(0) {}
    Size(float w, float h) : width(w), height(h) {}
};

// Struct untuk rectangle
struct Rect {
    float x, y, width, height;
    
    Rect() : x(0), y(0), width(0), height(0) {}
    Rect(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) {}
    
    // Mengecek apakah point ada di dalam rect
    bool contains(const Point& point) const {
        return point.x >= x && point.x <= x + width &&
               point.y >= y && point.y <= y + height;
    }
    
    // Mendapatkan titik tengah
    Point center() const {
        return Point(x + width / 2, y + height / 2);
    }
};

// Enum untuk event type
enum class EventType {
    MouseDown,
    MouseUp,
    MouseMove,
    MouseEnter,
    MouseLeave,
    KeyDown,
    KeyUp,
    Focus,
    Blur,
    Change
};

// Struct untuk mouse event
struct MouseEvent {
    Point position;
    bool leftButton;
    bool rightButton;
    bool middleButton;
    
    MouseEvent() : leftButton(false), rightButton(false), middleButton(false) {}
};

// Struct untuk keyboard event
struct KeyEvent {
    int keyCode;
    bool shift;
    bool control;
    bool alt;
    
    KeyEvent() : keyCode(0), shift(false), control(false), alt(false) {}
};

// Base class untuk semua event
struct Event {
    EventType type;
    
    Event(EventType type) : type(type) {}
    virtual ~Event() {}
};

// Class untuk element dasar
class Element {
public:
    Element();
    virtual ~Element();
    
    // Lifecycle methods
    virtual void initialize();
    virtual void update(float deltaTime);
    virtual void render();
    virtual void cleanup();
    
    // Properti dasar
    void setPosition(float x, float y);
    void setSize(float width, float height);
    void setStyle(const Style& style);
    void setVisible(bool visible);
    void setEnabled(bool enabled);
    void setId(const std::string& id);
    
    // Getters
    Point getPosition() const;
    Size getSize() const;
    Rect getBounds() const;
    const Style& getStyle() const;
    bool isVisible() const;
    bool isEnabled() const;
    std::string getId() const;
    
    // Event handling
    virtual bool handleEvent(const Event& event);
    virtual bool handleMouseEvent(const MouseEvent& event, EventType type);
    virtual bool handleKeyEvent(const KeyEvent& event, EventType type);
    
    // Child management
    void addChild(std::shared_ptr<Element> child);
    void removeChild(std::shared_ptr<Element> child);
    void clearChildren();
    
    // Event listeners
    typedef std::function<void(const Event&, Element*)> EventListener;
    void addEventListener(EventType type, EventListener listener);
    void removeEventListener(EventType type);
    
    // Parent-child relationship
    void setParent(Element* parent);
    Element* getParent() const;
    
    // Style states
    bool isHovered() const;
    bool isFocused() const;
    bool isActive() const;
    
    void setHovered(bool hovered);
    void setFocused(bool focused);
    void setActive(bool active);
    
    // Get effective style based on current state
    Style getEffectiveStyle() const;
    
protected:
    Point position_;
    Size size_;
    Style style_;
    bool visible_;
    bool enabled_;
    std::string id_;
    
    // State
    bool hovered_;
    bool focused_;
    bool active_;
    
    // Parent-child relationship
    Element* parent_;
    std::vector<std::shared_ptr<Element>> children_;
    
    // Event listeners
    std::map<EventType, EventListener> eventListeners_;
    
    // Dispatch event to listeners
    void dispatchEvent(const Event& event);
};

// Class untuk Rectangle Element
class RectElement : public Element {
public:
    RectElement();
    RectElement(float x, float y, float width, float height);
    virtual ~RectElement();
    
    virtual void render() override;
};

// Class untuk Line Element
class LineElement : public Element {
public:
    LineElement();
    LineElement(float x1, float y1, float x2, float y2);
    virtual ~LineElement();
    
    void setPoints(float x1, float y1, float x2, float y2);
    void setStartPoint(float x, float y);
    void setEndPoint(float x, float y);
    
    Point getStartPoint() const;
    Point getEndPoint() const;
    
    virtual void render() override;
    
private:
    Point startPoint_;
    Point endPoint_;
};

// Class untuk Text Element
class TextElement : public Element {
public:
    TextElement();
    TextElement(const std::string& text, float x, float y);
    virtual ~TextElement();
    
    void setText(const std::string& text);
    std::string getText() const;
    
    virtual void render() override;
    
private:
    std::string text_;
};

// Class untuk InputBox Element
class InputBox : public Element {
public:
    InputBox();
    InputBox(float x, float y, float width, float height);
    virtual ~InputBox();
    
    void setText(const std::string& text);
    std::string getText() const;
    
    void setPlaceholder(const std::string& placeholder);
    std::string getPlaceholder() const;
    
    void setMaxLength(size_t maxLength);
    size_t getMaxLength() const;
    
    void setPasswordMode(bool passwordMode);
    bool isPasswordMode() const;
    
    // Event handling overrides
    virtual bool handleKeyEvent(const KeyEvent& event, EventType type) override;
    virtual bool handleMouseEvent(const MouseEvent& event, EventType type) override;
    
    // Render override
    virtual void render() override;
    
    // Callbacks
    typedef std::function<void(InputBox*, const std::string&)> TextChangeCallback;
    void setOnTextChanged(TextChangeCallback callback);
    
private:
    std::string text_;
    std::string placeholder_;
    size_t maxLength_;
    bool passwordMode_;
    size_t cursorPosition_;
    TextChangeCallback onTextChanged_;
    
    // Helper methods
    void updateCursorPosition(const Point& mousePos);
    void insertCharacter(char c);
    void deleteCharacter(bool backspace);
};

} // namespace zketch

#endif // ZKETCH_ELEMENT_H