#pragma once
#include "zenv.h"
#include "zbj.h"

bool isHover(const Event& event, const zbj& obj) {
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
		int mx = event.motion.x;
		int my = event.motion.y;
		const Bound& bound = obj.getBound();
		return (mx >= bound.x && mx <= bound.x + bound.w && my >= bound.y && my <= bound.y + bound.h);
    }
	return false; // Bukan event mouse, tidak hover
}

bool isOnClick(const Event& event, Bound& bound){
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		int mx = event.button.x;
		int my = event.button.y;
		return (mx >= bound.x && mx <= bound.x + bound.w && my >= bound.y && my <= bound.y + bound.h);
    }
	return false; // Bukan event mouse button down
}

int isKeyDown(const Event& event, const zbj& obj){
	if(event.type == SDL_EVENT_KEY_DOWN){
		if(event.key.key == SDLK_A){
			return 0;
		}
		if(event.key.key == SDLK_W){
			return 1;
		}
		if(event.key.key == SDLK_D){
			return 2;
		}
		if(event.key.key == SDLK_S){
			return 3;
		}
	}
	return -1; // bukan event keydown
}
