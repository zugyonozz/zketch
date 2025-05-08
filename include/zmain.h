// zk_init.h
#pragma once
#include <iostream>
#include "zenv.h"

struct WinData {
    const char* title;
    int w, h;
    SDL_WindowFlags flags;
};

class zmain {
private:
    Window window;
    Renderer renderer;
    WinData wd;

public:
    zmain();

    zmain(const WinData& wd);
    bool initZketch(const WinData& wd);
    WinData getWinSize() const ;
    Renderer& getRenderer();
	bool present();
	bool clearRender(Color color);
	void delay(Uint32 ms);
	void setWinSize();
    ~zmain();
};