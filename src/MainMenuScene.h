#ifndef _MAINMENUSCENE_INCLUDE
#define _MAINMENUSCENE_INCLUDE

#include "Scene.h"

class MainMenuScene : public Scene
{
public:
	virtual void init();
	virtual void render();
	virtual void mouseRelease(int button, int xMouse, int yMouse);
	void keyReleased(int key);
};

#endif // _MAINMENUSCENE_INCLUDE
