#include <zketch.h>
using namespace std;

int main(){
	WindowData wd = {
		"zketch Test",
		1280, 960,
		0
	};

	zmain zk(wd);

	zbj rect({100, 200, 300, 400}, {0, 255, 255, 255}, zk.getRenderer());
	rect.draw();
	rect.show();
	zfont font;
	font.loadFont("notoSans", 48, "res/NotoSans-Regular.ttf");
	zbj text({0, 0, 0, 0}, {0, 255, 0, 255}, zk.getRenderer());
	text.draw(font.getFont("notoSans", 48), "Hello World", {150, 250});
	text.show();
	zk.present();

	SDL_Delay(5000);

	return 0;
}