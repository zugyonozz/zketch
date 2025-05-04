#include <zketch.h>
#include <ctime>
#include <cstdlib>
using namespace std;

WindowData wd = {"Demo", 720, 720, 0};
// init app
zmain app(wd);

bool isRun = true;
int tile = 40;
Event e;

int main() {
	srand(time(0));

	// player
    zbj p({app.getWindowSize().w / 4, app.getWindowSize().h - 80, tile, tile}, {240, 0, 240, 255}, app.getRenderer());
	p.draw();
	Bound newBound = p.getBound();
	int jumpId = 0;
	int jumpPattern[] = {-80, -60, -40, -30, -20, -15, -10, -7, -5, 0, 5, 7, 10, 15, 20, 30, 40, 60, 80};
	bool isJump = false;

	// land
	int gap = 80;
	zbj land({0, app.getWindowSize().h - 40, app.getWindowSize().w, 40}, {0, 240, 0, 255}, app.getRenderer());
	land.draw();
	bool isClearLand = false;

    while(isRun){
		SDL_SetRenderDrawColor(app.getRenderer(), 0, 0, 0, 255);
		SDL_RenderClear(app.getRenderer());
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				isRun = false;
			}
			if(e.type == SDL_EVENT_KEY_DOWN){
				if(e.key.key == SDLK_SPACE){
					isJump = true;
				}
			}
		}
		if(isJump == true){
			newBound.y += jumpPattern[jumpId];
			p.setBound(newBound);
			jumpId++;
			if(jumpId == 19){
				isJump = false;
				jumpId = 0;
			}
		}
		for(size_t i = 0; i < land.getBounds().size(); i++){
			land.setActiveID(i);
			Bound landNewBound = land.getBound();
			landNewBound.x -= 10;
			land.setBound(landNewBound);
		}
		if(land.getBounds().front().x + land.getBounds().front().w < 0){
			land.setActiveID(0);
			land.removeItem(0);
		}
		if(land.getBounds().empty() || land.getBounds().back().x + land.getBounds().back().w + gap < app.getWindowSize().w){
			land.addItem();
			land.setActiveID(land.getTextures().size()-1);
			int minWidth = tile * 4;
			int maxWidth = app.getWindowSize().w / 2;
			int width = minWidth + (rand() % ((maxWidth - minWidth) / tile + 1)) * tile;
			land.setBound({app.getWindowSize().w, app.getWindowSize().h - tile, width, tile});
			land.draw();
		}
		
		p.show();
		for(size_t i = 0; i < land.getBounds().size(); i++){
			land.setActiveID(i);
			land.show();
		}
		app.present();
		SDL_Delay(1000/60);
	}

    return 0;
}