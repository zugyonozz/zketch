// zenv.h
#pragma once
#include "SDL3/SDL.h"
#include "SDL3/SDL_image.h"
#include "SDL3/SDL_ttf.h"

using Font = TTF_Font*;
using Bound = SDL_Rect;
using FBound = SDL_FRect;
using Point = SDL_Point;
using FPoint = SDL_FPoint;
using Color = SDL_Color;
using Texture = SDL_Texture*;
using Window = SDL_Window*;
using Renderer = SDL_Renderer*;
using Surface = SDL_Surface*;
using Event = SDL_Event;