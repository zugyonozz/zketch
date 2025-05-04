// zbj.h
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include "zenv.h"

enum class BaseShape {
    LINE,
    RECT,
    TEXT,
    IMAGE
};

class zbj{
private:
    std::vector<Bound> bounds;
    Color color;
	Renderer renderer;
    std::vector<Texture> textures;
	size_t ID;

public:
    zbj();
    zbj(Bound bound, Color color, Renderer renderer);
    ~zbj();
    bool drawLine();
    bool draw(float radiusScale = 0.0f);
    bool draw(const Font font, const char* text, Point pos);
    bool draw(const char* path);
    bool clearItems();
    bool show();
    void setBound(const Bound& newBound);
    void setColor(const Color& newColor);
	void addItem();
	bool removeItem(size_t ID);
	void setActiveID(size_t newID);
    
    const Bound& getBound() const;
    const Color& getColor() const;
	const size_t& getID() const;
	const std::vector<Bound>& getBounds() const;
	const std::vector<Texture>& getTextures() const;
};