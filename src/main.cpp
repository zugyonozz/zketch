#include "zketch.h"
using namespace std;

// create window
WinData wd = {"Demo 2", 900, 700, SDL_WINDOW_RESIZABLE};
zmain app(wd);

// create font objects
zfont fonts;

// create event objects
Event e;

// function to laod fonts
void loadFonts(){	
	fonts.loadFont("NotoSans", 24, "res/fonts/NotoSans-Regular.ttf");
}

// parameters app run
bool isRun = true;

// function Build child of form
void buildForm(zbj& _zbj){
	Bound b;
	_zbj.addItem();
	_zbj.setActiveID(_zbj.getBounds().size()-1);
	_zbj.setColor({220, 0, 0, 255});
	_zbj.draw(fonts.getFont("NotoSans", 24), "Username", {0, 0});
	b = _zbj.getBound();
	b.x = _zbj.getBounds().front().origin.x;
	b.y = (float)_zbj.getBounds().front().origin.y - (float)_zbj.getBound().h * 1.5;
	_zbj.setBound(b);
	_zbj.setAnchorPt(AnchorType::ANCHOR_RIGHT_MID);
}

// function set pos for responsive
void updatePosForm(zbj& _zbj){
	Bound b = _zbj.getBound();
	b.x = app.getWinSize().w / 2;
	b.y = app.getWinSize().h / 2;
	_zbj.setBound(b);
}

int main(){
	// load fonts
	loadFonts();

	// create Form Obj
	zbj form({app.getWinSize().w / 2, app.getWinSize().h/ 2, app.getWinSize().w / 3, 64}, {60, 60, 60, 255}, app.getRenderer());
	form.setAnchorPt(AnchorType::ANCHOR_CENTER);
	form.draw(0.75);
	buildForm(form);

	while(isRun){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				isRun = false;
			}if(e.type == SDL_EVENT_WINDOW_MAXIMIZED){
				app.setWinSize();
				// updatePosForm(form);
			}if(e.type == SDL_EVENT_WINDOW_RESTORED){
				app.setWinSize();
				// updatePosForm(form);
			}if(e.type == SDL_EVENT_WINDOW_MINIMIZED){
				continue;
			}
		}
		app.clearRender({30, 30, 30, 255});
		form.show();
		app.present();
		app.delay(1000/7);
	}

	return 0;
}