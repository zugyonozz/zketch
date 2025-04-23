#include "../include/style.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <fstream>

namespace zketch {

// Implementasi Color::FromHex
Color Color::FromHex(const std::string& hex) {
    if (hex.empty() || hex[0] != '#') {
        return Color::Black();
    }
    
    std::string cleanHex = hex.substr(1); // Remove '#'
    unsigned int value = 0;
    
    try {
        value = std::stoul(cleanHex, nullptr, 16);
    } catch (...) {
        return Color::Black();
    }
    
    if (cleanHex.length() == 6) {
        // #RRGGBB format
        unsigned char r = (value >> 16) & 0xFF;
        unsigned char g = (value >> 8) & 0xFF;
        unsigned char b = value & 0xFF;
        return Color(r, g, b, 255);
    } else if (cleanHex.length() == 8) {
        // #RRGGBBAA format
        unsigned char r = (value >> 24) & 0xFF;
        unsigned char g = (value >> 16) & 0xFF;
        unsigned char b = (value >> 8) & 0xFF;
        unsigned char a = value & 0xFF;
        return Color(r, g, b, a);
    } else if (cleanHex.length() == 3) {
        // #RGB format, expand to #RRGGBB
        unsigned char r = ((value >> 8) & 0xF) * 17; // Duplicate the nibble (e.g., 0xA -> 0xAA)
        unsigned char g = ((value >> 4) & 0xF) * 17;
        unsigned char b = (value & 0xF) * 17;
        return Color(r, g, b, 255);
    }
    
    return Color::Black();
}

// Implementasi Style
Style::Style()
    : backgroundColor_(Color::Transparent()),
      foregroundColor_(Color::Black()),
      borderColor_(Color::Black()),
      borderWidth_(0),
      borderStyle_(BorderStyle::None),
      borderRadius_(0),
      width_(0),
      height_(0),
      paddingTop_(0), paddingRight_(0), paddingBottom_(0), paddingLeft_(0),
      marginTop_(0), marginRight_(0), marginBottom_(0), marginLeft_(0),
      fontSize_(12),
      fontFamily_("Arial"),
      hoverStyle_(nullptr),
      activeStyle_(nullptr),
      focusStyle_(nullptr) {
}

Style::~Style() {
    delete hoverStyle_;
    delete activeStyle_;
    delete focusStyle_;
}

Style& Style::setBackgroundColor(const Color& color) {
    backgroundColor_ = color;
    return *this;
}

Style& Style::setForegroundColor(const Color& color) {
    foregroundColor_ = color;
    return *this;
}

Style& Style::setBorderColor(const Color& color) {
    borderColor_ = color;
    return *this;
}

Style& Style::setBorderWidth(float width) {
    borderWidth_ = width;
    return *this;
}

Style& Style::setBorderStyle(BorderStyle style) {
    borderStyle_ = style;
    return *this;
}

Style& Style::setBorderRadius(float radius) {
    borderRadius_ = radius;
    return *this;
}

Style& Style::setWidth(float width) {
    width_ = width;
    return *this;
}

Style& Style::setHeight(float height) {
    height_ = height;
    return *this;
}

Style& Style::setPadding(float padding) {
    paddingTop_ = paddingRight_ = paddingBottom_ = paddingLeft_ = padding;
    return *this;
}

Style& Style::setPadding(float top, float right, float bottom, float left) {
    paddingTop_ = top;
    paddingRight_ = right;
    paddingBottom_ = bottom;
    paddingLeft_ = left;
    return *this;
}

Style& Style::setMargin(float margin) {
    marginTop_ = marginRight_ = marginBottom_ = marginLeft_ = margin;
    return *this;
}

Style& Style::setMargin(float top, float right, float bottom, float left) {
    marginTop_ = top;
    marginRight_ = right;
    marginBottom_ = bottom;
    marginLeft_ = left;
    return *this;
}

Style& Style::setFontSize(float size) {
    fontSize_ = size;
    return *this;
}

Style& Style::setFontFamily(const std::string& family) {
    fontFamily_ = family;
    return *this;
}

Color Style::getBackgroundColor() const {
    return backgroundColor_;
}

Color Style::getForegroundColor() const {
    return foregroundColor_;
}

Color Style::getBorderColor() const {
    return borderColor_;
}

float Style::getBorderWidth() const {
    return borderWidth_;
}

BorderStyle Style::getBorderStyle() const {
    return borderStyle_;
}

float Style::getBorderRadius() const {
    return borderRadius_;
}

float Style::getWidth() const {
    return width_;
}

float Style::getHeight() const {
    return height_;
}

float Style::getPaddingTop() const {
    return paddingTop_;
}

float Style::getPaddingRight() const {
    return paddingRight_;
}

float Style::getPaddingBottom() const {
    return paddingBottom_;
}

float Style::getPaddingLeft() const {
    return paddingLeft_;
}

float Style::getMarginTop() const {
    return marginTop_;
}

float Style::getMarginRight() const {
    return marginRight_;
}

float Style::getMarginBottom() const {
    return marginBottom_;
}

float Style::getMarginLeft() const {
    return marginLeft_;
}

float Style::getFontSize() const {
    return fontSize_;
}

std::string Style::getFontFamily() const {
    return fontFamily_;
}

Style Style::clone() const {
    Style cloned;
    
    cloned.backgroundColor_ = backgroundColor_;
    cloned.foregroundColor_ = foregroundColor_;
    cloned.borderColor_ = borderColor_;
    cloned.borderWidth_ = borderWidth_;
    cloned.borderStyle_ = borderStyle_;
    cloned.borderRadius_ = borderRadius_;
    cloned.width_ = width_;
    cloned.height_ = height_;
    cloned.paddingTop_ = paddingTop_;
    cloned.paddingRight_ = paddingRight_;
    cloned.paddingBottom_ = paddingBottom_;
    cloned.paddingLeft_ = paddingLeft_;
    cloned.marginTop_ = marginTop_;
    cloned.marginRight_ = marginRight_;
    cloned.marginBottom_ = marginBottom_;
    cloned.marginLeft_ = marginLeft_;
    cloned.fontSize_ = fontSize_;
    cloned.fontFamily_ = fontFamily_;
    
    if (hoverStyle_) {
        cloned.hoverStyle_ = new Style(hoverStyle_->clone());
    }
    if (activeStyle_) {
        cloned.activeStyle_ = new Style(activeStyle_->clone());
    }
    if (focusStyle_) {
        cloned.focusStyle_ = new Style(focusStyle_->clone());
    }
    
    return cloned;
}

Style& Style::merge(const Style& other) {
    // Only override properties that are set in other
    if (other.backgroundColor_.a > 0) {
        backgroundColor_ = other.backgroundColor_;
    }
    
    if (other.foregroundColor_.a > 0) {
        foregroundColor_ = other.foregroundColor_;
    }
    
    if (other.borderColor_.a > 0) {
        borderColor_ = other.borderColor_;
    }
    
    if (other.borderWidth_ > 0) {
        borderWidth_ = other.borderWidth_;
    }
    
    if (other.borderStyle_ != BorderStyle::None) {
        borderStyle_ = other.borderStyle_;
    }
    
    if (other.borderRadius_ > 0) {
        borderRadius_ = other.borderRadius_;
    }
    
    if (other.width_ > 0) {
        width_ = other.width_;
    }
    
    if (other.height_ > 0) {
        height_ = other.height_;
    }
    
    if (other.paddingTop_ > 0 || other.paddingRight_ > 0 || 
        other.paddingBottom_ > 0 || other.paddingLeft_ > 0) {
        paddingTop_ = other.paddingTop_;
        paddingRight_ = other.paddingRight_;
        paddingBottom_ = other.paddingBottom_;
        paddingLeft_ = other.paddingLeft_;
    }
    
    if (other.marginTop_ > 0 || other.marginRight_ > 0 || 
        other.marginBottom_ > 0 || other.marginLeft_ > 0) {
        marginTop_ = other.marginTop_;
        marginRight_ = other.marginRight_;
        marginBottom_ = other.marginBottom_;
        marginLeft_ = other.marginLeft_;
    }
    
    if (other.fontSize_ > 0) {
        fontSize_ = other.fontSize_;
    }
    
    if (!other.fontFamily_.empty()) {
        fontFamily_ = other.fontFamily_;
    }
    
    // Merge state styles
    if (other.hoverStyle_) {
        if (!hoverStyle_) {
            hoverStyle_ = new Style(other.hoverStyle_->clone());
        } else {
            hoverStyle_->merge(*other.hoverStyle_);
        }
    }
    
    if (other.activeStyle_) {
        if (!activeStyle_) {
            activeStyle_ = new Style(other.activeStyle_->clone());
        } else {
            activeStyle_->merge(*other.activeStyle_);
        }
    }
    
    if (other.focusStyle_) {
        if (!focusStyle_) {
            focusStyle_ = new Style(other.focusStyle_->clone());
        } else {
            focusStyle_->merge(*other.focusStyle_);
        }
    }
    
    return *this;
}

bool Style::parseFromString(const std::string& cssLike) {
    std::istringstream stream(cssLike);
    std::string line;
    
    while (std::getline(stream, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (line.empty() || line.front() == '/' || line.front() == '#') {
            continue; // Skip comments and empty lines
        }
        
        // Find key-value separator
        size_t colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            continue; // Invalid line, no colon
        }
        
        std::string key = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);
        
        // Trim key and value
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t;") + 1);
        
        // Parse each property
        if (key == "background-color" || key == "background") {
            if (value.front() == '#') {
                backgroundColor_ = Color::FromHex(value);
            } else if (value == "transparent") {
                backgroundColor_ = Color::Transparent();
            } else if (value == "black") {
                backgroundColor_ = Color::Black();
            } else if (value == "white") {
                backgroundColor_ = Color::White();
            } else if (value == "red") {
                backgroundColor_ = Color::Red();
            } else if (value == "green") {
                backgroundColor_ = Color::Green();
            } else if (value == "blue") {
                backgroundColor_ = Color::Blue();
            }
        } else if (key == "color") {
            if (value.front() == '#') {
                foregroundColor_ = Color::FromHex(value);
            } else if (value == "black") {
                foregroundColor_ = Color::Black();
            } else if (value == "white") {
                foregroundColor_ = Color::White();
            } else if (value == "red") {
                foregroundColor_ = Color::Red();
            } else if (value == "green") {
                foregroundColor_ = Color::Green();
            } else if (value == "blue") {
                foregroundColor_ = Color::Blue();
            }
        } else if (key == "border-color") {
            if (value.front() == '#') {
                borderColor_ = Color::FromHex(value);
            } else if (value == "black") {
                borderColor_ = Color::Black();
            } else if (value == "white") {
                borderColor_ = Color::White();
            } else if (value == "red") {
                borderColor_ = Color::Red();
            } else if (value == "green") {
                borderColor_ = Color::Green();
            } else if (value == "blue") {
                borderColor_ = Color::Blue();
            }
        } else if (key == "border-width") {
            try {
                borderWidth_ = std::stof(value);
            } catch (...) {}
        } else if (key == "border-radius") {
            try {
                borderRadius_ = std::stof(value);
            } catch (...) {}
        } else if (key == "border-style") {
            if (value == "none") {
                borderStyle_ = BorderStyle::None;
            } else if (value == "solid") {
                borderStyle_ = BorderStyle::Solid;
            } else if (value == "dashed") {
                borderStyle_ = BorderStyle::Dashed;
            } else if (value == "dotted") {
                borderStyle_ = BorderStyle::Dotted;
            }
        } else if (key == "width") {
            try {
                width_ = std::stof(value);
            } catch (...) {}
        } else if (key == "height") {
            try {
                height_ = std::stof(value);
            } catch (...) {}
        } else if (key == "padding") {
            try {
                float padding = std::stof(value);
                setPadding(padding);
            } catch (...) {}
        } else if (key == "margin") {
            try {
                float margin = std::stof(value);
                setMargin(margin);
            } catch (...) {}
        } else if (key == "font-size") {
            try {
                fontSize_ = std::stof(value);
            } catch (...) {}
        } else if (key == "font-family") {
            fontFamily_ = value;
        }
    }
    
    return true;
}

Style& Style::setHoverStyle(const Style& hoverStyle) {
    if (hoverStyle_) {
        delete hoverStyle_;
    }
    hoverStyle_ = new Style(hoverStyle);
    return *this;
}

Style& Style::setActiveStyle(const Style& activeStyle) {
    if (activeStyle_) {
        delete activeStyle_;
    }
    activeStyle_ = new Style(activeStyle);
    return *this;
}

Style& Style::setFocusStyle(const Style& focusStyle) {
    if (focusStyle_) {
        delete focusStyle_;
    }
    focusStyle_ = new Style(focusStyle);
    return *this;
}

const Style* Style::getHoverStyle() const {
    return hoverStyle_;
}

const Style* Style::getActiveStyle() const {
    return activeStyle_;
}

const Style* Style::getFocusStyle() const {
    return focusStyle_;
}

// StyleParser implementation
StyleParser::StyleParser() {
}

StyleParser::~StyleParser() {
}

Style StyleParser::parseCSS(const std::string& css) {
    Style style;
    parseDeclaration(css, style);
    return style;
}

std::map<std::string, Style> StyleParser::parseFile(const std::string& filename) {
    std::map<std::string, Style> styles;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        return styles;
    }
    
    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    
    // Simple CSS parser for selector-style rules
    std::istringstream stream(content);
    std::string currentSelector;
    std::string currentBlock;
    bool inBlock = false;
    
    while (std::getline(stream, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (line.empty()) {
            continue;
        }
        
        if (!inBlock) {
            // Looking for selector
            size_t bracePos = line.find('{');
            if (bracePos != std::string::npos) {
                // Found start of block
                currentSelector = line.substr(0, bracePos);
                // Trim selector
                currentSelector.erase(0, currentSelector.find_first_not_of(" \t"));
                currentSelector.erase(currentSelector.find_last_not_of(" \t") + 1);
                
                // Start collecting block content
                currentBlock = line.substr(bracePos + 1);
                inBlock = true;
                
                // Check if block ends on same line
                size_t closeBracePos = currentBlock.find('}');
                if (closeBracePos != std::string::npos) {
                    // Complete block on same line
                    Style style;
                    parseDeclaration(currentBlock.substr(0, closeBracePos), style);
                    styles[currentSelector] = style;
                    
                    inBlock = false;
                    currentBlock.clear();
                }
            }
        } else {
            // In block, looking for end
            size_t closeBracePos = line.find('}');
            if (closeBracePos != std::string::npos) {
                // End of block found
                currentBlock += line.substr(0, closeBracePos);
                
                // Parse block and add to styles
                Style style;
                parseDeclaration(currentBlock, style);
                styles[currentSelector] = style;
                
                inBlock = false;
                currentBlock.clear();
            } else {
                // Continue collecting block content
                currentBlock += line;
            }
        }
    }
    
    // Handle case where file ends without closing brace
    if (inBlock && !currentBlock.empty() && !currentSelector.empty()) {
        Style style;
        parseDeclaration(currentBlock, style);
        styles[currentSelector] = style;
    }
    
    return styles;
}

void StyleParser::parseDeclaration(const std::string& declaration, Style& style) {
    std::istringstream stream(declaration);
    std::string line;
    
    while (std::getline(stream, line, ';')) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        
        if (line.empty()) {
            continue;
        }
        
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string property = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            
            // Trim property and value
            property.erase(0, property.find_first_not_of(" \t"));
            property.erase(property.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            // Handle properties
            if (property == "background-color" || property == "background") {
                style.setBackgroundColor(parseColor(value));
            } else if (property == "color") {
                style.setForegroundColor(parseColor(value));
            } else if (property == "border-color") {
                style.setBorderColor(parseColor(value));
            } else if (property == "border-width") {
                try {
                    float width = std::stof(value);
                    style.setBorderWidth(width);
                } catch (...) {}
            } else if (property == "border-radius") {
                try {
                    float radius = std::stof(value);
                    style.setBorderRadius(radius);
                } catch (...) {}
            } else if (property == "border-style") {
                if (value == "none") {
                    style.setBorderStyle(BorderStyle::None);
                } else if (value == "solid") {
                    style.setBorderStyle(BorderStyle::Solid);
                } else if (value == "dashed") {
                    style.setBorderStyle(BorderStyle::Dashed);
                } else if (value == "dotted") {
                    style.setBorderStyle(BorderStyle::Dotted);
                }
            } else if (property == "width") {
                try {
                    float width = std::stof(value);
                    style.setWidth(width);
                } catch (...) {}
            } else if (property == "height") {
                try {
                    float height = std::stof(value);
                    style.setHeight(height);
                } catch (...) {}
            } else if (property == "padding") {
                try {
                    float padding = std::stof(value);
                    style.setPadding(padding);
                } catch (...) {}
            } else if (property == "margin") {
                try {
                    float margin = std::stof(value);
                    style.setMargin(margin);
                } catch (...) {}
            } else if (property == "font-size") {
                try {
                    float size = std::stof(value);
                    style.setFontSize(size);
                } catch (...) {}
            } else if (property == "font-family") {
                style.setFontFamily(value);
            }
        }
    }
}

Color StyleParser::parseColor(const std::string& colorStr) {
    if (colorStr.empty()) {
        return Color::Black();
    }
    
    if (colorStr[0] == '#') {
        return Color::FromHex(colorStr);
    } else if (colorStr == "black") {
        return Color::Black();
    } else if (colorStr == "white") {
        return Color::White();
    } else if (colorStr == "red") {
        return Color::Red();
    } else if (colorStr == "green") {
        return Color::Green();
    } else if (colorStr == "blue") {
        return Color::Blue();
    } else if (colorStr == "transparent") {
        return Color::Transparent();
    }
    
    return Color::Black();
}

} // namespace zketch