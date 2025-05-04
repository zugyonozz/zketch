// zk_init.h
#pragma once
#include <iostream>
#include "zenv.h"

struct WindowData {
    const char* title;
    int w, h;
    SDL_WindowFlags flags;
};

class zmain {
private:
    Window window;
    Renderer renderer;
    WindowData wd;

public:
    zmain();

    zmain(const WindowData& wd);
    bool initZketch(const WindowData& wd);
    WindowData getWindowSize() const ;
    Renderer& getRenderer();
	bool present();
    ~zmain();
};