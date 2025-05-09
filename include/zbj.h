// zbj.h
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include <cstring>
#include "zenv.h"

enum class AnchorType {
    ANCHOR_TOP_LEFT,
    ANCHOR_TOP_MID,
    ANCHOR_TOP_RIGHT,
    ANCHOR_LEFT_MID,
    ANCHOR_BOT_RIGHT,
    ANCHOR_BOT_MID,
    ANCHOR_BOT_LEFT,
    ANCHOR_RIGHT_MID,
    ANCHOR_CENTER
};

struct Anchor {
    Bound origin;
    Point current;
};

class zbj {
private:
    std::vector<Anchor> bounds;
    Color color;
    Renderer renderer;
    std::vector<Texture> textures;
    size_t ID;
    std::vector<AnchorType> anchor;

public:
    zbj(Renderer renderer);
    zbj(Bound bound, Color color, Renderer renderer);
    ~zbj();
    
    // Drawing functions
    bool drawLine();
    bool draw(float radiusScale = 0.0f);
    bool draw(const Font font, const char* text, Point pos);
    bool draw(const char* path);
    
    // Item management
    bool clearItems();
    bool show(size_t ID);
    bool show();
    void setBound(const Bound& newBound);
    void setColor(const Color& newColor);
    void addItem();
    bool removeItem(size_t index);
    void setActiveID(size_t newID);
    void setAnchorPt(AnchorType anchor);
    
    // Getters
	const Bound getRealBound(size_t index) const;
    const size_t& getID() const;
    const std::vector<Anchor>& getBounds() const;
    const std::vector<Texture>& getTextures() const;
};