// zevent.h
#pragma once
#include "zenv.h"
#include "zbj.h"
#include <functional>

using std::function;
using zFunctions = std::pair<std::function<bool()>, std::function<bool()>>;
using zFollowUp = std::pair<Bound, std::function<void()>>;
using zFollowUps = std::pair<Bound, zFunctions>;

FPoint getMousePos(){
	FPoint coord;
	SDL_PumpEvents();
	SDL_GetMouseState(&coord.x, &coord.y);
	return coord;
}

void isHover(FPoint coord, std::vector<zFollowUps>& funcs){
	Bound b;
	for(auto& f : funcs){
		b = f.first;
		if(coord.x >= b.x && coord.x <= b.x + b.w && coord.y >= b.y && coord.y <= b.y + b.h){
			f.second.second();
			continue;
		}
		f.second.first();
	}
}

void isOnClick(const Event& event, std::vector<zFollowUp>& func){
	for(auto& f : func){
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			int mx = event.button.x;
			int my = event.button.y;
			if(mx >= f.first.x && mx <= f.first.x + f.first.w && my >= f.first.y && my <= f.first.y + f.first.h){
				f.second();
				return;
			}
    	}
	}
}
