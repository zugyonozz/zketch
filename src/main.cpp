// zketch Demo
#include "zketch.h"
using namespace std;

// create window
WinData wd = {"Demo 2", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED};
zmain app(wd);

// create font objects
zfont fonts;

// create event objects
Event e;

// function to load fonts
void loadFonts(){	
	fonts.loadFont("NotoSans", 24, "res/fonts/NotoSans-Regular.ttf");
}

// parameters app run
bool isRun = true;

// paths of nav btn assets
vector<string> path = {
	"res/assets/home.png",
	"res/assets/call.png",
	"res/assets/discover.png",
	"res/assets/setting.png"
};

// func to create layout
void buildLayout(zbj& _zbj) {
	_zbj.setAnchorPt(AnchorType::ANCHOR_BOT_RIGHT);
	_zbj.setBound({app.getWinSize().w + 16, app.getWinSize().h + 16, app.getWinSize().w - 72, app.getWinSize().h + 8});
	_zbj.setColor(Hex("#1E1E1E"));
	_zbj.draw(0.05); // Background
}

// func to create nav btn
void buildNav(zbj& _zbj, int id){
	_zbj.setColor(Hex("#3c3c3c"));
	_zbj.setBound({43, 86*(id + 1), 86, 86});
	_zbj.draw(0.25);
	_zbj.setAnchorPt(AnchorType::ANCHOR_CENTER);

	_zbj.addItem();
	_zbj.draw(path[id].c_str());
	Bound b = _zbj.getBounds().back().origin;
	b.x = _zbj.getBounds().front().origin.x;
	b.y = _zbj.getBounds().front().origin.y;
	b.w *= 1.5; b.h *= 1.5;
	_zbj.setBound(b);
}

// func to set pos and create nav Active
void setPosNavActive(zbj& _zbj, int id){
	_zbj.setBound({0, 86 * (id + 1), 24, 86});
	if(!_zbj.getTextures().front()){
		_zbj.setColor(Hex("#65E10F"));
		_zbj.draw(1);
		_zbj.setAnchorPt(AnchorType::ANCHOR_CENTER);
	}
}

int main(){
	// load fonts
	loadFonts();

	// update window size
	app.updateWinSize();
	app.setWindowIcon("res/assets/app-icon.png");

	// create layout obj
	zbj layout(app.getRenderer());
	buildLayout(layout);

	// create nav btn obj
	zbj navHome(app.getRenderer());
	buildNav(navHome, 0);

	zbj navCall(app.getRenderer());
	buildNav(navCall, 1);

	zbj navDiscover(app.getRenderer());
	buildNav(navDiscover, 2);

	zbj navSetting(app.getRenderer());
	buildNav(navSetting, 3);

	// create nav active obj
	zbj navActive(app.getRenderer());
	setPosNavActive(navActive, 0);

	// create sequence follow up for nav btn
	vector<zFollowUps> navHovers;
	navHovers.push_back({
		navHome.getRealBound(0), {
			[&](){return navHome.show(1);}, 
			[&](){return navHome.show();}}});
	navHovers.push_back({
		navCall.getRealBound(0), {
			[&](){return navCall.show(1);}, 
			[&](){return navCall.show();}}});
	navHovers.push_back({
		navDiscover.getRealBound(0), {
			[&](){return navDiscover.show(1);}, 
			[&](){return navDiscover.show();}}});
	navHovers.push_back({
		navSetting.getRealBound(0), {
			[&](){return navSetting.show(1);}, 
			[&](){return navSetting.show();}}});

	// create sequence follow up for nav Active
	vector<zFollowUp> navActiveOnClicks = {
		{navHome.getRealBound(0),
		[&](){setPosNavActive(navActive, 0);}},
		{navCall.getRealBound(0),
		[&](){setPosNavActive(navActive, 1);}},
		{navDiscover.getRealBound(0),
		[&](){setPosNavActive(navActive, 2);}},
		{navSetting.getRealBound(0),
		[&](){setPosNavActive(navActive, 3);}}
	};

	// main loop of app
	while(isRun){
		app.clearRender(Hex("#111111"));
		layout.show();
		isHover(getMousePos(), navHovers);
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				isRun = false;
			}if(e.type == SDL_EVENT_WINDOW_MAXIMIZED){
				app.updateWinSize();
			}if(e.type == SDL_EVENT_WINDOW_RESTORED){
				app.updateWinSize();
			}if(e.type == SDL_EVENT_WINDOW_MINIMIZED){
				continue;
			}
			isOnClick(e, navActiveOnClicks);
		}
		navActive.show();
		app.present();
		app.delay(1000/60);
	}

	return 0;
}