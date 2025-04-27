// zbj.h
#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include "zenv.h"

using Bound = SDL_Rect;
using Point = SDL_Point;
using Color = SDL_Color;
using Font = TTF_Font*;

enum class BaseShape {
    LINE,
    RECT,
    TEXT,
    IMAGE
};

class zbj{
private:
    Bound bound;
    Color color;
	SDL_Renderer* renderer;
    std::vector<SDL_Texture*> textures;

    bool drawLineImpl();
    bool drawRectImpl(float radiusScale);
    bool drawTextImpl(const Font& font, const char* text, Point pos);
    bool drawImageImpl(const char* path);

public:
    zbj();
    zbj(Bound bound, Color color, SDL_Renderer* renderer);
    ~zbj();
    bool drawLine();
    bool draw(float radiusScale = 0.0f);
    bool draw(const Font font, const char* text, Point pos);
    bool draw(const char* path);
    bool clearTextures();
    bool show();
    void setBound(const Bound& newBound);
    void setColor(const Color& newColor);
    
    const Bound& getBound() const;
    const Color& getColor() const;
};