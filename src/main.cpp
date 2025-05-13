#include "zketch.h"
using namespace std;

WinData wd = {"mySAQ", 1200, 900, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED};

zmain app(wd);

zfont fonts;
Event e;
bool isRun = true;

Bound bBg;
Bound bLayout;
vector<Point> pTitle;
vector<Point> pTextinForm;
vector<pair<string, pair<string, int>>> dTextinForm;

void loadFonts(){
	string parent = "res/fonts/Montserrats/";
	vector<pair<string, pair<string, int>>> myfont = {
		{"Montserrat-SemiBold", {"Montserrat-SemiBold.ttf", 36}}, 
		{"Montserrat-Medium", {"Montserrat-Medium.ttf", 24}}, 
		{"Montserrat-Light", {"Montserrat-Light.ttf", 20}}
	};
	for(auto& f : myfont){
		string fullPath = parent + f.second.first;
		fonts.loadFont(f.first, f.second.second, fullPath.c_str());
	}
}

void setBg(zbj& z){
	bBg = {0, 0, app.getWinSize().w, app.getWinSize().h};
	z.setBound(bBg);
	z.setColor(Hex("7070FA"));
	z.draw();
}

void setLayout(zbj& z){
	bLayout = {app.getWinSize().w, 0, app.getWinSize().w / 3, app.getWinSize().h};
	z.setBound(bLayout);
	if(!z.getTextures().front()){
		z.setColor(Hex("303030"));
		z.setAnchorPt(AnchorType::ANCHOR_TOP_RIGHT);
		z.draw();
	}
}

void setTextinForm(zbj& z){
	pTextinForm = {
		{(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3},
		{(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 50}
	};

	dTextinForm = {
		{"Welcome to mySAQ!", {"Montserrat-SemiBold", 36}}, 
		{"Enter your account details below.", {"Montserrat-Medium", 24}},
	};

	vector<Point>& p = pTextinForm;
	vector<pair<string, pair<string, int>>>& d = dTextinForm;

	z.setColor(Hex("#F1F1F1"));
	if(!z.getTextures().front()){
		for(size_t i = 0; i < d.size(); i++){
			z.draw(fonts.getFont(d[i].second.first, d[i].second.second), d[i].first.c_str(), pTextinForm[i]);
			if(i < d.size() - 1){ z.addItem(); }
		}
	}else{
		for(size_t i = 0; i < z.getBounds().size(); i++){
			z.setActiveID(i);
			Bound b = z.getBounds()[i].origin;
			b.x = p[i].x;
			b.y = p[i].y;
			z.setBound(b);
		}
	}
}

void setTitlePH(zbj& z){
	pTitle = {
		{(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 100},
		{(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 200}
	};
	vector<string> s = {"Username", "Password"};

	z.setColor(Hex("f1f1f1"));
	for(size_t i = 0; i < s.size(); i++){
		z.draw(fonts.getFont("Montserrat-Light", 20), s.at(i).c_str(), pTitle.at(i));
		if(i < s.size() - 1){ z.addItem(); }
	}
}

void setPH(zbj& z, Bound b, bool isFocus = false, string s = ""){
	vector<Color> colors = {Hex("707070"), Hex("9090B0")};
	Color c = isFocus ? colors.at(1) : colors.at(0);

	if(!z.getTextures().front()){
		z.setColor(c);
		z.setBound(b);
		z.draw(0.5);
	}
	if(z.getBounds().size() == 2){
		z.removeItem(1);
	}
	z.addItem();
	z.setColor(Hex("f1f1f1"));
	z.draw(fonts.getFont("Montserrat-Light", 20), s.c_str(), {b.x + 15, b.y + int(b.h * 0.30)});
}

bool clickState2Bool(uint32_t cs){
	if(cs == 3 && cs == 0){
		return true;
	}
	return false;
}

int main(){
	loadFonts();
	app.updateWinSize();
	app.startTextInput();

	zbj bg(app.getRenderer());
	setBg(bg);

	zbj layout(app.getRenderer());
	setLayout(layout);

	zbj textinForm(app.getRenderer());
	setTextinForm(textinForm);

	zbj usnm(app.getRenderer());
	setTitlePH(usnm);

	zbj phusnm(app.getRenderer());
	Bound busnm = {(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 130, (int)(app.getWinSize().w * 0.275), 50};
	setPH(phusnm, busnm, 0);

	zbj phpass(app.getRenderer());
	Bound bpass = {(int)(app.getWinSize().w * 0.7), app.getWinSize().h / 3 + 230, (int)(app.getWinSize().w * 0.275), 50};
	setPH(phpass, bpass, 0);

	vector<pair<Bound, pair<bool, string>>> idPH = {{busnm, {0, ""}}, {bpass, {0, ""}}};
	zFuncs callUpdatePH = {
		[&](){ setPH(phusnm, busnm, idPH.at(0).second.first, idPH.at(0).second.second); },
		[&](){ setPH(phusnm, busnm, idPH.at(1).second.first, idPH.at(1).second.second); },
	};

	while(isRun){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				isRun = false;
			}
			if(e.type == SDL_EVENT_WINDOW_MAXIMIZED || e.type == SDL_EVENT_WINDOW_MINIMIZED || e.type == SDL_EVENT_WINDOW_RESTORED 
			|| (e.type == SDL_EVENT_WINDOW_RESIZED && app.getWinSize().w >= 1200 && app.getWinSize().h >= 900)){
				app.updateWinSize();
			}
			if(e.type == SDL_EVENT_TEXT_INPUT){
				for(auto& d : idPH){
					uint32_t isFocusedPH = getClickState(e, d.first);
					d.second.first = clickState2Bool(isFocusedPH);
					if(d.second.first){ d.second.second += e.text.text; break;}
				}
			}
		}
		callUpdatePH();
		app.clearRender(Hex("000"));
		bg.show();
		layout.show();
		textinForm.show();
		usnm.show();
		phusnm.show();
		phpass.show();
		app.present();
		app.delay(1000);
	}

	app.stopTextInput();
	return 0;
}