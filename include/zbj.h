// zbj.h
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include "zenv.h"

enum class AnchorType{
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

struct Anchor{
	Bound origin;
	Point current;
};

class zbj{
private:
    std::vector<Anchor> bounds;
    Color color;
	Renderer renderer;
    std::vector<Texture> textures;
	size_t ID;
	AnchorType anchor;

public:
    zbj();
    zbj(Bound bound, Color color, Renderer renderer);
    ~zbj();
    bool drawLine();
    bool draw(float radiusScale = 0.0f);
    bool draw(const Font font, const char* text, Point pos);
    bool draw(const char* path, char keepRatio = 'n');
    bool clearItems();
    bool show();
    void setBound(const Bound& newBound);
    void setColor(const Color& newColor);
	void addItem();
	bool removeItem(size_t ID);
	void setActiveID(size_t newID);
	void setAnchorPt(AnchorType anchor);
    
    const Bound& getBound() const;
    const Color& getColor() const;
	const size_t& getID() const;
	const std::vector<Anchor>& getBounds() const;
	const std::vector<Texture>& getTextures() const;
};