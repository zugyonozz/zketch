#include "zbj.h"

bool zbj::drawLineImpl() {  // Renamed to avoid confusion with public method
	SDL_Texture* t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, bound.w, bound.h);
	if (!t) { std::cerr << "Error: Could not create texture! " << SDL_GetError() << std::endl; return false; }
	SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, t);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderLine(renderer, 0, 0, bound.w, bound.h);  // Fixed coordinates
	SDL_SetRenderTarget(renderer, oldTarget);
	textures.push_back(t);
	return true;
}

bool zbj::drawRectImpl(float radiusScale) {
	if (radiusScale < 0.0f || radiusScale > 1.0f) { std::cerr << "Error: Radius scale value must be between 0.0 and 1.0" << std::endl; return false; }
	if (radiusScale == 0.0f) {
		SDL_Surface* s = SDL_CreateSurface(bound.w, bound.h, SDL_PIXELFORMAT_RGBA32);
		if (!s) { std::cerr << "Error: Could not create surface! " << SDL_GetError() << std::endl; return false; }   
		Uint32 pixel = SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, color.r, color.g, color.b, color.a);
		SDL_FillSurfaceRect(s, nullptr, pixel);
		SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
		SDL_DestroySurface(s);
		if (!t) { std::cerr << "Error: Could not create texture! " << SDL_GetError() << std::endl; return false; }
		textures.push_back(t);
		return true;
	}
	float minDimension = std::min(bound.w, bound.h);
	float radius = minDimension * radiusScale * 0.5f;
	SDL_Surface* s = SDL_CreateSurface(bound.w, bound.h, SDL_PIXELFORMAT_RGBA32);
	if (!s) { std::cerr << "Error: Could not create surface! " << SDL_GetError() << std::endl; return false; }
	SDL_FillSurfaceRect(s, nullptr, SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, 0, 0, 0, 0));        
	if (SDL_LockSurface(s) != 0) { std::cerr << "Error: Failed to lock surface! " << SDL_GetError() << std::endl; SDL_DestroySurface(s); return false; }
	Uint32 pixel = SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, color.r, color.g, color.b, color.a);
	SDL_Rect inner1 = { static_cast<int>(radius), 0, bound.w - static_cast<int>(2 * radius), bound.h };
	SDL_Rect inner2 = { 0, static_cast<int>(radius), bound.w, bound.h - static_cast<int>(2 * radius) };
	SDL_FillSurfaceRect(s, &inner1, pixel);
	SDL_FillSurfaceRect(s, &inner2, pixel);
	auto drawCorner = [&](SDL_FPoint center, float startAngle) {
		for (float angle = 0; angle <= 90.0f; angle += 0.5f) {
			float rad = (startAngle + angle) * (M_PI / 180.0f);
			for (float r = 0; r <= radius; r += 0.5f) {
				int x = static_cast<int>(center.x + r * cosf(rad));
				int y = static_cast<int>(center.y + r * sinf(rad));
				if (x >= 0 && y >= 0 && x < s->w && y < s->h) {
					static_cast<Uint32*>(s->pixels)[y * s->w + x] = pixel;
				}
			}
		}
	};
	drawCorner({ radius, radius }, 180.0f);
	drawCorner({ static_cast<float>(bound.w) - radius, radius }, 270.0f);
	drawCorner({ static_cast<float>(bound.w) - radius, static_cast<float>(bound.h) - radius }, 0.0f);
	drawCorner({ radius, static_cast<float>(bound.h) - radius }, 90.0f);
	SDL_UnlockSurface(s);
	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
	SDL_DestroySurface(s);
	if (!t) { std::cerr << "Error: Could not create rounded texture! " << SDL_GetError() << std::endl; return false; }
	textures.push_back(t);
	return true;
}

bool zbj::drawTextImpl(const Font& font, const char* text, Point pos) {
	if (!font || !text) { std::cerr << "Error: Invalid font or text!\n"; return false; }
	SDL_Surface* s = TTF_RenderText_Blended(font, text, strlen(text), color);
	if (!s) { std::cerr << "Error: Could not render text! " << SDL_GetError() << std::endl;  return false; }
	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
	if (!t) { std::cerr << "Error: Could not create texture from text! " << SDL_GetError() << std::endl; SDL_DestroySurface(s); return false; }
	bound = {pos.x, pos.y, s->w, s->h};
	textures.push_back(t);
	SDL_DestroySurface(s);
	return true;
}

bool zbj::drawImageImpl(const char* path) {
	if (!path) {std::cerr << "Error: Invalid image path!\n"; return false; }
	SDL_Surface* s = IMG_Load(path);
	if (!s) { std::cerr << "Error: Could not load image! " << SDL_GetError() << std::endl; return false; }
	SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
	if (!t) { std::cerr << "Error: Could not create texture from image! " << SDL_GetError() << std::endl; return false; }
	SDL_DestroySurface(s);
	textures.push_back(t);
	return true;
}

zbj::zbj() : bound({0, 0, 0, 0}), color({0, 0, 0, 0}), renderer(nullptr) {}

zbj::zbj(Bound bound, Color color, SDL_Renderer* renderer) : bound(bound),  color(color), renderer(renderer){}

zbj::~zbj(){
	clearTextures();
}

bool zbj::drawLine() {
	if (!textures.empty()) { std::cerr << "Error: Could not draw new line - clear textures first!" << std::endl; return false; }
	return drawLineImpl();
}

bool zbj::draw(float radiusScale) {
	if (!textures.empty()) { std::cerr << "Error: Could not draw new rectangle - clear textures first!" << std::endl; return false; }
	return drawRectImpl(radiusScale);
}

bool zbj::draw(const Font font, const char* text, Point pos) {
	if (!textures.empty()) { std::cerr << "Error: Could not draw new text - clear textures first!" << std::endl; return false; }
	return drawTextImpl(font, text, pos);
}

bool zbj::draw(const char* path) {
	if (!textures.empty()) { std::cerr << "Error: Could not draw new image - clear textures first!" << std::endl; return false; }
	return drawImageImpl(path);
}

bool zbj::clearTextures() {
	for (auto tex : textures) {
		if (tex) { SDL_DestroyTexture(tex); }
	}
	textures.clear();
	return true;
}

bool zbj::show() {
	if (textures.empty()) { std::cerr << "Error: No texture to show!" << std::endl; return false; }
	SDL_FRect Fr = { static_cast<float>(bound.x), static_cast<float>(bound.y), static_cast<float>(bound.w), static_cast<float>(bound.h) };
	SDL_RenderTexture(renderer, textures.back(), nullptr, &Fr);
	return true;
}

void zbj::setBound(const Bound& newBound) { 
	bound = newBound; 
}

void zbj::setColor(const Color& newColor) { 
	color = newColor; 
}

const Bound& zbj::getBound() const {
	return bound; 
}

const Color& zbj::getColor() const { 
	return color; 
}