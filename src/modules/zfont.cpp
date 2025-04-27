#include "zfont.h"

zfont::zfont(){}
zfont::~zfont(){
	cleanUpFonts();
}

bool zfont::loadFont(const std::string& name, size_t size, const std::string& path) {
	std::string key = name + "_" + std::to_string(size);
	if (fonts.count(key)) return true;
	TTF_Font* font = TTF_OpenFont(path.c_str(), size);
	if (!font) { SDL_Log("Failed to load font %s: %s", path.c_str(), SDL_GetError()); return false; }
	fonts[key] = { font, size };
	return true;
}

const Font zfont::getFont(const std::string& name, int size) const{
	std::string key = name + "_" + std::to_string(size);
	if (fonts.count(key)) { return fonts.at(key).font; }
	return nullptr;
}

bool zfont::cleanUpFonts() {
	for (auto& [key, data] : fonts) { TTF_CloseFont(data.font); }
	fonts.clear();
	return true;
}