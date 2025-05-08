#include "zbj.h"

bool zbj::drawLine() {
	if (textures[ID]) { std::cerr << "Error: Could not draw new line - clear textures first!" << std::endl; return false; }
	Texture t = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, bounds[ID].origin.w, bounds[ID].origin.h);
	if (!t) { std::cerr << "Error: Could not create texture! " << SDL_GetError() << std::endl; return false; }
	Texture oldTarget = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, t);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderLine(renderer, 0, 0, bounds[ID].origin.w, bounds[ID].origin.h);
	SDL_SetRenderTarget(renderer, oldTarget);
	textures[ID] = t;
	return true;
}

bool zbj::draw(float radiusScale) {
	if (textures[ID]) { std::cerr << "Error: Could not draw new rectangle - clear textures first!" << std::endl; return false; }
	if (radiusScale < 0.0f || radiusScale > 1.0f) { std::cerr << "Error: Radius scale value must be between 0.0 and 1.0" << std::endl; return false; }
	if (radiusScale == 0.0f) {
		Surface s = SDL_CreateSurface(bounds[ID].origin.w, bounds[ID].origin.h, SDL_PIXELFORMAT_RGBA32);
		if (!s) { std::cerr << "Error: Could not create surface! " << SDL_GetError() << std::endl; return false; }   
		Uint32 pixel = SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, color.r, color.g, color.b, color.a);
		SDL_FillSurfaceRect(s, nullptr, pixel);
		Texture t = SDL_CreateTextureFromSurface(renderer, s);
		SDL_DestroySurface(s);
		if (!t) { std::cerr << "Error: Could not create texture! " << SDL_GetError() << std::endl; return false; }
		textures[ID] = t;
		return true;
	}
	float minDimension = std::min(bounds[ID].origin.w, bounds[ID].origin.h);
	float radius = minDimension * radiusScale * 0.5f;
	Surface s = SDL_CreateSurface(bounds[ID].origin.w, bounds[ID].origin.h, SDL_PIXELFORMAT_RGBA32);
	if (!s) { std::cerr << "Error: Could not create surface! " << SDL_GetError() << std::endl; return false; }
	SDL_FillSurfaceRect(s, nullptr, SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, 0, 0, 0, 0));        
	if (!SDL_LockSurface(s)) { std::cerr << "Error: Failed to lock surface! " << SDL_GetError() << std::endl; SDL_DestroySurface(s); return false; }
	Uint32 pixel = SDL_MapRGBA(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32), nullptr, color.r, color.g, color.b, color.a);
	SDL_Rect inner1 = { static_cast<int>(radius), 0, bounds[ID].origin.w - static_cast<int>(2 * radius), bounds[ID].origin.h };
	SDL_Rect inner2 = { 0, static_cast<int>(radius), bounds[ID].origin.w, bounds[ID].origin.h - static_cast<int>(2 * radius) };
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
	drawCorner({ static_cast<float>(bounds[ID].origin.w) - radius, radius }, 270.0f);
	drawCorner({ static_cast<float>(bounds[ID].origin.w) - radius, static_cast<float>(bounds[ID].origin.h) - radius }, 0.0f);
	drawCorner({ radius, static_cast<float>(bounds[ID].origin.h) - radius }, 90.0f);
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
	// Fixed: Added check for text length
	size_t textLength = strlen(text);
	if (textLength == 0) { std::cerr << "Error: Empty text string!\n"; return false; }
	Surface s = TTF_RenderText_Blended(font, text, textLength, color);
	if (!s) { std::cerr << "Error: Could not render text! " << SDL_GetError() << std::endl;  return false; }
	Texture t = SDL_CreateTextureFromSurface(renderer, s);
	if (!t) { std::cerr << "Error: Could not create texture from text! " << SDL_GetError() << std::endl; SDL_DestroySurface(s); return false; }
	bounds[ID].origin = {pos.x, pos.y, s->w, s->h};
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
	if (!t) { 
		std::cerr << "Error: Could not create texture from image! " << SDL_GetError() << std::endl; 
		SDL_DestroySurface(s);  // Fixed: Added missing surface cleanup
		return false; 
	}
	if(keepRatioIn == 'W'){
		float aspect = static_cast<float>(s->w) / static_cast<float>(s->h);
		bounds[ID].origin.w = static_cast<int>(bounds[ID].origin.h * aspect);
	}else if(keepRatioIn == 'H'){
		float aspect = static_cast<float>(s->h) / static_cast<float>(s->w);
		bounds[ID].origin.h = static_cast<int>(bounds[ID].origin.w * aspect);
	}else if(keepRatioIn != 'n'){
		std::cerr << "Error: Invalid keep ratio option '" << keepRatioIn << "'. Use 'W', 'H', or 'n'.\n";
		SDL_DestroySurface(s);
		SDL_DestroyTexture(t);  // Fixed: Added missing texture cleanup
		return false;
	}
	SDL_DestroySurface(s);
	textures[ID] = t;
	return true;
}

zbj::zbj() : renderer(nullptr), ID(0) {
	bounds.resize(1);
	textures.resize(1);  // Fixed: Added missing texture resize
	anchor.resize(1);    // Fixed: Added missing anchor resize
	bounds[ID].origin = {0, 0, 0, 0};
	textures[ID] = nullptr;  // Fixed: Initialize texture to nullptr
	anchor[ID] = AnchorType::ANCHOR_TOP_LEFT;
}

zbj::zbj(Bound bound, Color color, Renderer renderer) : color(color), renderer(renderer), ID(0) {
	this->bounds.resize(1);
	this->textures.resize(1);
	this->anchor.resize(1);  // Fixed: Added missing anchor resize
	this->bounds[ID].origin = bound;
	this->textures[ID] = nullptr;
	anchor[ID] = AnchorType::ANCHOR_TOP_LEFT;
}

zbj::~zbj() {
	clearItems();
}

bool zbj::clearItems() {
	for(auto& t : textures) {
		if (t) { SDL_DestroyTexture(t); t = nullptr; }  // Fixed: Set to nullptr after destroying
	} 
	textures.clear();
	bounds.clear();
	anchor.clear();
	ID = 0;  // Fixed: Reset ID to 0
	return true;
}

bool zbj::show(size_t id) {
	if (id >= textures.size()) { std::cerr << "Error: Invalid ID!" << std::endl; return false; }  // Fixed: Added index check
	if (!textures[id]) { std::cerr << "Error: No texture to show for ID " << id << "!" << std::endl; return false; }
	
	// Temporarily save the current ID
	size_t tempID = ID;
	ID = id;
	
	setAnchorPt(anchor[id]);
	FBound fRect = { 
		static_cast<float>(bounds[id].current.x), 
		static_cast<float>(bounds[id].current.y), 
		static_cast<float>(bounds[id].origin.w), 
		static_cast<float>(bounds[id].origin.h) };
	SDL_RenderTexture(renderer, textures[id], nullptr, &fRect);
	
	// Restore the previous ID
	ID = tempID;
	return true;
}

bool zbj::show() {
	if (textures.empty()) { std::cerr << "Error: No textures to show!" << std::endl; return false; }  // Fixed: Check if vector is empty
	
	bool success = true;
	size_t tempID = ID;  // Save original ID
	
	for(size_t i = 0; i < anchor.size(); i++) {
		ID = i;  // Set current ID for anchor calculation
		if (!textures[i]) { 
			std::cerr << "Warning: No texture to show for ID " << i << "!" << std::endl; 
			continue;  // Fixed: Skip instead of failing entirely
		}
		setAnchorPt(anchor[i]);
		FBound fRect = { 
			static_cast<float>(bounds[i].current.x), 
			static_cast<float>(bounds[i].current.y), 
			static_cast<float>(bounds[i].origin.w), 
			static_cast<float>(bounds[i].origin.h) };
		SDL_RenderTexture(renderer, textures[i], nullptr, &fRect);
	}
	
	ID = tempID;  // Restore original ID
	return success;
}

void zbj::setBound(const Bound& newBound) { 
	bounds[ID].origin = newBound;
}

void zbj::setColor(const Color& newColor) { 
	color = newColor; 
}

void zbj::addItem() {
	ID = bounds.size();
	textures.resize(ID+1);
	bounds.resize(ID+1);
	anchor.resize(ID+1);
	
	if (ID > 0) {
		bounds[ID].origin = bounds[ID-1].origin;
		bounds[ID].current = bounds[ID-1].current;
		anchor[ID] = anchor[ID-1];
	} else {
		bounds[ID].origin = {0, 0, 0, 0};
		bounds[ID].current = {0, 0};
		anchor[ID] = AnchorType::ANCHOR_TOP_LEFT;
	}
	
	textures[ID] = nullptr;
}

bool zbj::removeItem(size_t index) {
	if (index >= bounds.size()) {
		std::cerr << "Error: Invalid index for removal: " << index << std::endl;
		return false;
	}
	
	if (textures[index]) {
		SDL_DestroyTexture(textures[index]);
	}
	
	bounds.erase(bounds.begin() + index);
	textures.erase(textures.begin() + index);
	anchor.erase(anchor.begin() + index);
	
	// Ensure ID remains valid
	if (ID >= bounds.size()) {
		ID = bounds.empty() ? 0 : bounds.size() - 1;
	}
	
	return true;
}

void zbj::setActiveID(size_t newID) {
	if (newID >= bounds.size()) {
		std::cerr << "Warning: Trying to set invalid ID: " << newID << ". Using last valid ID." << std::endl;
		ID = bounds.empty() ? 0 : bounds.size() - 1;
		return;
	}
	ID = newID;
}

void zbj::setAnchorPt(AnchorType anchorType) {
	this->anchor[ID] = anchorType;
	Anchor& b = bounds[ID];
	
	switch(anchorType) {
		case AnchorType::ANCHOR_TOP_LEFT:
			b.current.x = b.origin.x;
			b.current.y = b.origin.y;
			break;
		case AnchorType::ANCHOR_TOP_MID:
			b.current.x = b.origin.x - b.origin.w / 2;
			b.current.y = b.origin.y;
			break;
		case AnchorType::ANCHOR_TOP_RIGHT:
			b.current.x = b.origin.x - b.origin.w;
			b.current.y = b.origin.y;
			break;
		case AnchorType::ANCHOR_RIGHT_MID:
			b.current.x = b.origin.x - b.origin.w;
			b.current.y = b.origin.y - b.origin.h / 2;
			break;
		case AnchorType::ANCHOR_BOT_RIGHT:
			b.current.x = b.origin.x - b.origin.w;
			b.current.y = b.origin.y - b.origin.h;
			break;
		case AnchorType::ANCHOR_BOT_MID:
			b.current.x = b.origin.x - b.origin.w / 2;
			b.current.y = b.origin.y - b.origin.h;
			break;
		case AnchorType::ANCHOR_BOT_LEFT:
			b.current.x = b.origin.x;
			b.current.y = b.origin.y - b.origin.h;
			break;
		case AnchorType::ANCHOR_LEFT_MID:
			b.current.x = b.origin.x;
			b.current.y = b.origin.y - b.origin.h / 2;
			break;
		case AnchorType::ANCHOR_CENTER:
			b.current.x = b.origin.x - b.origin.w / 2;
			b.current.y = b.origin.y - b.origin.h / 2;
			break;
	}
}

const Bound& zbj::getBound() const {
	return bounds[ID].origin;
}

const Color& zbj::getColor() const { 
	return color; 
}

const size_t& zbj::getID() const {
	return ID;
}

const std::vector<Anchor>& zbj::getBounds() const {
	return bounds;
}

const std::vector<Texture>& zbj::getTextures() const {
	return textures;
}