#ifndef ZKETCH_STYLE_H // style.h
#define ZKETCH_STYLE_H

#include <string>
#include <map>
#include <vector>

namespace zketch {

// Forward declaration
class Element;

// Struct untuk warna RGBA
struct Color {
    unsigned char r, g, b, a;
    
    Color() : r(0), g(0), b(0), a(255) {}
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
        : r(r), g(g), b(b), a(a) {}
    
    // Helper untuk warna umum
    static Color Black() { return Color(0, 0, 0); }
    static Color White() { return Color(255, 255, 255); }
    static Color Red() { return Color(255, 0, 0); }
    static Color Green() { return Color(0, 255, 0); }
    static Color Blue() { return Color(0, 0, 255); }
    static Color Transparent() { return Color(0, 0, 0, 0); }
    
    // Parse warna dari string hex (#RRGGBB atau #RRGGBBAA)
    static Color FromHex(const std::string& hex);
};

// Enum untuk border style
enum class BorderStyle {
    None,
    Solid,
    Dashed,
    Dotted
};

// Class Style untuk menyimpan semua properti styling
class Style {
public:
    Style();
    ~Style();
    
    // Setter dasar
    Style& setBackgroundColor(const Color& color);
    Style& setForegroundColor(const Color& color);
    Style& setBorderColor(const Color& color);
    Style& setBorderWidth(float width);
    Style& setBorderStyle(BorderStyle style);
    Style& setBorderRadius(float radius);
    Style& setWidth(float width);
    Style& setHeight(float height);
    Style& setPadding(float padding);
    Style& setPadding(float top, float right, float bottom, float left);
    Style& setMargin(float margin);
    Style& setMargin(float top, float right, float bottom, float left);
    Style& setFontSize(float size);
    Style& setFontFamily(const std::string& family);
    
    // Getter
    Color getBackgroundColor() const;
    Color getForegroundColor() const;
    Color getBorderColor() const;
    float getBorderWidth() const;
    BorderStyle getBorderStyle() const;
    float getBorderRadius() const;
    float getWidth() const;
    float getHeight() const;
    float getPaddingTop() const;
    float getPaddingRight() const;
    float getPaddingBottom() const;
    float getPaddingLeft() const;
    float getMarginTop() const;
    float getMarginRight() const;
    float getMarginBottom() const;
    float getMarginLeft() const;
    float getFontSize() const;
    std::string getFontFamily() const;
    
    // Clone style
    Style clone() const;
    
    // Merge dengan style lain (properti non-default dari other menggantikan)
    Style& merge(const Style& other);
    
    // Parse dari string CSS-like
    bool parseFromString(const std::string& cssLike);
    
    // Properti khusus untuk state
    Style& setHoverStyle(const Style& hoverStyle);
    Style& setActiveStyle(const Style& activeStyle);
    Style& setFocusStyle(const Style& focusStyle);
    
    // Mendapatkan style untuk state tertentu
    const Style* getHoverStyle() const;
    const Style* getActiveStyle() const;
    const Style* getFocusStyle() const;
    
private:
    Color backgroundColor_;
    Color foregroundColor_;
    Color borderColor_;
    float borderWidth_;
    BorderStyle borderStyle_;
    float borderRadius_;
    float width_;
    float height_;
    float paddingTop_, paddingRight_, paddingBottom_, paddingLeft_;
    float marginTop_, marginRight_, marginBottom_, marginLeft_;
    float fontSize_;
    std::string fontFamily_;
    
    // Style untuk state berbeda
    Style* hoverStyle_;
    Style* activeStyle_;
    Style* focusStyle_;
};

// Class StyleParser untuk parsing dari CSS atau format lain
class StyleParser {
public:
    StyleParser();
    ~StyleParser();
    
    // Parse CSS string dan return style
    Style parseCSS(const std::string& css);
    
    // Parse file CSS dan return map dari selector ke style
    std::map<std::string, Style> parseFile(const std::string& filename);
    
private:
    // Helper methods untuk parsing
    void parseDeclaration(const std::string& declaration, Style& style);
    Color parseColor(const std::string& colorStr);
};

} // namespace zketch

#endif // ZKETCH_STYLE_H