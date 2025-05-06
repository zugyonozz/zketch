#include "zbj.h"

bool zbj::drawLine() {
	if (textures[ID]) { std::cerr << "Error: Could not draw new line - clear textures first!" << std::endl; return false; }
	Texture t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, bounds[ID].w, bounds[ID].h);
	if (!t) { std::cerr << "Error: Could not create texture! " << SDL_GetError() << std::endl; return false; }
	Texture oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, t);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderLine(renderer, 0, 0, bounds[ID].w, bounds[ID].h);
	SDL_SetRenderTarget(renderer, oldTarget);
	textures[ID] = t;
	return true;
}

bool zbj::draw(float radiusScale) {
	if (textures[ID]) { std::cerr << "Error: Could not draw new rectangle - clear textures first!" << std::endl; return false; }
	if (radiusScale < 0.0f || radiusScale > 1.0f) { std::cerr << "Error: Radius scale value must be between 0.0 and 1.0" << std::endl; return false; }
	if (radiusScale == 0.0f) {
		Surface s = SDL_CreateSurface(bounds[ID].w, bounds[ID].h, SDL_PIXELFORMAT_RGBA32);
		if (!s) { std::cerr << "Error: Could not create surface! " << SDL_GetError() << std::endl; return false; }   
		Uint32 pixel = SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, color.r, color.g, color.b, color.a);
		SDL_FillSurfaceRect(s, nullptr, pixel);
		Texture t = SDL_CreateTextureFromSurface(renderer, s);
		SDL_DestroySurface(s);
		if (!t) { std::cerr << "Error: Could not create texture! " << SDL_GetError() << std::endl; return false; }
		textures[ID] = t;
		return true;
	}
	float minDimension = std::min(bounds[ID].w, bounds[ID].h);
	float radius = minDimension * radiusScale * 0.5f;
	Surface s = SDL_CreateSurface(bounds[ID].w, bounds[ID].h, SDL_PIXELFORMAT_RGBA32);
	if (!s) { std::cerr << "Error: Could not create surface! " << SDL_GetError() << std::endl; return false; }
	SDL_FillSurfaceRect(s, nullptr, SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, 0, 0, 0, 0));        
	if (!SDL_LockSurface(s)) { std::cerr << "Error: Failed to lock surface! " << SDL_GetError() << std::endl; SDL_DestroySurface(s); return false; }
	Uint32 pixel = SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, color.r, color.g, color.b, color.a);
	SDL_Rect inner1 = { static_cast<int>(radius), 0, bounds[ID].w - static_cast<int>(2 * radius), bounds[ID].h };
	SDL_Rect inner2 = { 0, static_cast<int>(radius), bounds[ID].w, bounds[ID].h - static_cast<int>(2 * radius) };
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
	drawCorner({ static_cast<float>(bounds[ID].w) - radius, radius }, 270.0f);
	drawCorner({ static_cast<float>(bounds[ID].w) - radius, static_cast<float>(bounds[ID].h) - radius }, 0.0f);
	drawCorner({ radius, static_cast<float>(bounds[ID].h) - radius }, 90.0f);
	SDL_UnlockSurface(s);
	Texture t = SDL_CreateTextureFromSurface(renderer, s);
	SDL_DestroySurface(s);
	if (!t) { std::cerr << "Error: Could not create rounded texture! " << SDL_GetError() << std::endl; return false; }
	textures[ID] = t;
	return true;
}

bool zbj::draw(const Font font, const char* text, Point pos) {
	if (textures[ID]) { std::cerr << "Error: Could not draw new text - clear textures first!" << std::endl; return false; }
	if (!font || !text) { std::cerr << "Error: Invalid font or text!\n"; return false; }
	Surface s = TTF_RenderText_Blended(font, text, strlen(text), color);
	if (!s) { std::cerr << "Error: Could not render text! " << SDL_GetError() << std::endl;  return false; }
	Texture t = SDL_CreateTextureFromSurface(renderer, s);
	if (!t) { std::cerr << "Error: Could not create texture from text! " << SDL_GetError() << std::endl; SDL_DestroySurface(s); return false; }
	bounds[ID] = {pos.x, pos.y, s->w, s->h};
	textures[ID] = t;
	SDL_DestroySurface(s);
	return true;
}

bool zbj::draw(const char* path, char keepRatioIn) {
	if (textures[ID]) { std::cerr << "Error: Could not draw new image - clear textures first!" << std::endl; return false; }
	if (!path) {std::cerr << "Error: Invalid image path!\n"; return false; }
	Surface s = IMG_Load(path);
	if (!s) { std::cerr << "Error: Could not load image! " << SDL_GetError() << std::endl; return false; }
	Texture t = SDL_CreateTextureFromSurface(renderer, s);
	if (!t) { std::cerr << "Error: Could not create texture from image! " << SDL_GetError() << std::endl; return false; }
	if(keepRatioIn == 'W'){
		float aspect = static_cast<float>(s->w) / static_cast<float>(s->h);
		bounds[ID].w = static_cast<int>(bounds[ID].h * aspect);
	}else if(keepRatioIn == 'H'){
		float aspect = static_cast<float>(s->h) / static_cast<float>(s->w);
		bounds[ID].h = static_cast<int>(bounds[ID].w * aspect);
	}else if(keepRatioIn != 'n'){
		std::cerr << "Error: Invalid keep ratio!\n";
		return false;
	}
	SDL_DestroySurface(s);
	textures[ID] = t;
	return true;
}

zbj::zbj() : renderer(nullptr), ID(0){
	bounds.resize(1);
	bounds[ID] = {0, 0, 0, 0};
}

zbj::zbj(Bound bound, Color color, Renderer renderer) : color(color), renderer(renderer), ID(0){
	this->bounds.resize(1);
	this->textures.resize(1);
	this->bounds[ID] = bound;
	this->textures[ID] = nullptr;
}

zbj::~zbj(){
	clearItems();
}

bool zbj::clearItems() {
	for(auto& t : textures){
		if (t) { SDL_DestroyTexture(t); }
	} 
	textures.clear();
	bounds.clear();

	return true;
}

bool zbj::show() {
	if (!textures[ID]) { std::cerr << "Error: No texture to show!" << std::endl; return false; }
	FBound fRect = { static_cast<float>(bounds[ID].x), static_cast<float>(bounds[ID].y), static_cast<float>(bounds[ID].w), static_cast<float>(bounds[ID].h) };
	SDL_RenderTexture(renderer, textures[ID], nullptr, &fRect);
	return true;
}

void zbj::setBound(const Bound& newBound) { 
	bounds[ID] = newBound; 
}

void zbj::setColor(const Color& newColor) { 
	color = newColor; 
}

void zbj::addItem(){
	ID = textures.size();
	textures.resize(ID+1);
	bounds.resize(ID+1);
	bounds[ID] = bounds[ID-1];
	textures[ID] = nullptr;
}

bool zbj::removeItem(size_t index) {
    if (index >= bounds.size()) return false;

    bounds.erase(bounds.begin() + index);
    textures.erase(textures.begin() + index);

    // Pastikan activeID tetap valid
    if (ID >= bounds.size()) {
        ID = bounds.empty() ? 0 : bounds.size() - 1;
    }

	return true;
}


void zbj::setActiveID(size_t newID){
	ID = newID;
}

const Bound& zbj::getBound() const {
	return bounds[ID];
}

const Color& zbj::getColor() const { 
	return color; 
}

const size_t& zbj::getID() const {
	return ID;
}

const std::vector<Bound>& zbj::getBounds() const{
	return bounds;
}

const std::vector<Texture>& zbj::getTextures() const{
	return textures;
}