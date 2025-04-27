#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
#include "zenv.h"

using Font = TTF_Font*;

struct FontData{
	Font font;
	size_t size;
};

using Fonts = std::unordered_map<std::string, FontData>;

class zfont{
	private:
		Fonts fonts;

	public:
		zfont();
		bool loadFont(const std::string& name, size_t size, const std::string& path);
		const Font getFont(const std::string& name, int size) const;
		bool cleanUpFonts();
		~zfont();
};