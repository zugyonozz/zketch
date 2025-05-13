#include "zmain.h"

zmain::zmain(){
	window = nullptr; 
	renderer = nullptr;
}

zmain::zmain(const WinData& wd) {
	if (!initZketch(wd)) {
		std::cerr << "Error: Could not initialize zmain\n";
	} else {
		std::cout << "zmain initialized successfully\n";
		this->wd = wd;
	}
}

bool zmain::initZketch(const WinData& wd) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "Error: Could not initialize SDL: " << SDL_GetError() << "\n";
		return false;
	}
	if (!TTF_Init()) {
		std::cerr << "Error: Could not initialize SDL_TTF: " << SDL_GetError() << "\n";
		SDL_Quit();
		return false;
	}
	window = SDL_CreateWindow(wd.title, wd.w, wd.h, wd.flags);
	if (!window) {
		std::cerr << "Error: Could not create window: " << SDL_GetError() << "\n";
		TTF_Quit();
		SDL_Quit();
		return false;
	}
	renderer = SDL_CreateRenderer(window, nullptr);
	if (!renderer) {
		std::cerr << "Error: Could not create renderer: " << SDL_GetError() << "\n";
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return false;
	}
	return true;
}

bool zmain::present(){
	return SDL_RenderPresent(renderer);
}

WinData zmain::getWinSize() const {
	return wd;
}

Renderer& zmain::getRenderer() {
	return renderer;
}

bool zmain::clearRender(Color color){
	if(!SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)){
		std::cerr << "Error: Could not set color: " << SDL_GetError() << "\n";
		return false;
	}
	if(!SDL_RenderClear(renderer)){
		std::cerr << "Error: Could not render clear!: " << SDL_GetError() << "\n";
		return false;
	}
	return true;
}

void zmain::delay(Uint32 ms){
	SDL_Delay(ms);
}

void zmain::updateWinSize(){
	SDL_GetWindowSize(window, &wd.w, &wd.h);
}

void zmain::setWindowIcon(const char* path){
	SDL_Surface* icon = IMG_Load(path);
	if (icon) {
		SDL_SetWindowIcon(window, icon);
		SDL_DestroySurface(icon);
	} else {
		std::cerr << "Error: Could not load icon!: " << SDL_GetError() << "\n";
	}
}

void zmain::startTextInput(){
	if(!SDL_StartTextInput(window)){
		std::cerr << "Error: Could not start text input: " << SDL_GetError() << "\n";
		return;	
	}
}

void zmain::stopTextInput(){
	if(!SDL_StopTextInput(window)){
		std::cerr << "Error: Could not stop text input: " << SDL_GetError() << "\n";
		return;	
	}
}

zmain::~zmain() {
	if (renderer) {
		SDL_DestroyRenderer(renderer);
	}
	if (window) {
		SDL_DestroyWindow(window);
	}
	TTF_Quit();
	SDL_Quit();
}