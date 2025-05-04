#include <zketch.h>
#include <ctime>
#include <cstdlib>
#include <string>
using namespace std;

WindowData wd = {"Demo", 720, 720, 0};
zmain app(wd);
zfont fonts;

zbj scoreMsg({app.getWindowSize().w / 2 - 50, 50, 100, 100}, {240, 0, 240, 255}, app.getRenderer());

bool isRun = true;
int tile = 40;
int initScore = 0;
Event e;

void updateScore(int val);
void resetState(zbj &p, zbj &land, Bound &newBound, bool &isJump, bool &isFalling, int &jumpId, int &initScore, bool &gameOver, int tile);

int main() {
	srand(time(0));
	bool gameOver = false;
	fonts.loadFont("NotoSans", 48, "res/NotoSans-Regular.ttf");

	updateScore(initScore);

	// player
	zbj p({app.getWindowSize().w / 4, app.getWindowSize().h - 80, tile, tile}, {240, 0, 240, 255}, app.getRenderer());
	p.draw();
	Bound newBound = p.getBound();
	int jumpId = 0;
	int jumpPattern[] = {-80, -60, -40, -30, -20, -15, -10, -7, -5, 0, 5, 7, 10, 15, 20, 30, 40, 60, 80};
	bool isJump = false;
	bool isFalling = false;

	// land
	int gap = 80;
	zbj land({0, app.getWindowSize().h - 40, app.getWindowSize().w, 40}, {0, 240, 0, 255}, app.getRenderer());
	land.draw();

	while(isRun){
		SDL_SetRenderDrawColor(app.getRenderer(), 0, 0, 0, 255);
		SDL_RenderClear(app.getRenderer());

		while(SDL_PollEvent(&e)){
			if(e.type == SDL_EVENT_QUIT){
				isRun = false;
			}
			if(e.type == SDL_EVENT_KEY_DOWN){
				if(e.key.key == SDLK_SPACE && !isJump && !isFalling && !gameOver){
					isJump = true;
				}
				if (gameOver && e.key.key == SDLK_R) {
					resetState(p, land, newBound, isJump, isFalling, jumpId, initScore, gameOver, tile);
				}
			}
		}

		if(gameOver){
			scoreMsg.addItem();
			scoreMsg.setActiveID(scoreMsg.getTextures().size()-1);
			scoreMsg.draw(fonts.getFont("NotoSans", 48), "Game Over", {360, 360});
			Bound msg = scoreMsg.getBound();
			msg.x -= msg.w/2;
			msg.y -= msg.h/2;
			scoreMsg.setBound(msg);
			scoreMsg.show();
			app.present();
			SDL_Delay(1000/60);
			continue;
		}

		if(isJump){
			newBound.y += jumpPattern[jumpId];
			p.setBound(newBound);
			jumpId++;
			if(jumpId >= sizeof(jumpPattern)/sizeof(jumpPattern[0])){
				isJump = false;
				jumpId = 0;
			}
		}

		bool onLand = false;
		for(size_t i = 0; i < land.getBounds().size(); i++){
			land.setActiveID(i);
			Bound lb = land.getBound();
			lb.x -= 5;
			land.setBound(lb);

			Bound pb = p.getBound();
			if (pb.y + pb.h == lb.y &&
				pb.x + pb.w > lb.x &&
				pb.x < lb.x + lb.w) {
				onLand = true;
			}
		}

		if (!isJump && !onLand) {
			isFalling = true;
		} else {
			isFalling = false;
		}

		if(isFalling){
			newBound.y += 10;
			p.setBound(newBound);
			if (p.getBound().y > app.getWindowSize().h){
				gameOver = true;
				continue;
			}
		}

		if (!land.getBounds().empty() && land.getBounds().front().x + land.getBounds().front().w < 0){
			land.setActiveID(0);
			land.removeItem(0);
			initScore++;
			updateScore(initScore);
		}

		if(land.getBounds().empty() || land.getBounds().back().x + land.getBounds().back().w + gap < app.getWindowSize().w){
			land.addItem();
			land.setActiveID(land.getTextures().size()-1);
			int minWidth = tile * 3;
			int maxWidth = app.getWindowSize().w / 2;
			int width = minWidth + (rand() % ((maxWidth - minWidth) / tile + 1)) * tile;
			land.setBound({app.getWindowSize().w, app.getWindowSize().h - tile, width, tile});
			land.draw();
		}

		scoreMsg.show();
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

void updateScore(int val){
	if(initScore >= 0){
		scoreMsg.clearItems();
		scoreMsg.addItem();
		scoreMsg.setActiveID(0);
	}
	scoreMsg.draw(fonts.getFont("NotoSans", 48), to_string(val).c_str(), {30, 30});
}

void resetState(zbj &p, zbj &land, Bound &newBound, bool &isJump, bool &isFalling, int &jumpId, int &initScore, bool &gameOver, int tile) {
	newBound = {app.getWindowSize().w / 4, app.getWindowSize().h - 80, tile, tile};
	p.setBound(newBound);

	isJump = false;
	isFalling = false;
	jumpId = 0;

	land.clearItems();
	land.addItem();
	land.setActiveID(0);
	land.setBound({0, app.getWindowSize().h - tile, app.getWindowSize().w, tile});
	land.draw();

	initScore = 0;
	updateScore(initScore);

	gameOver = false;
}
