#ifndef _CREDITSSCREEN_INCLUDE	
#define _CREDITSSCREEN_INCLUDE

#include "Scene.h"

class CreditsScene : public Scene
{
public:
	virtual void init();
	virtual void render();
	virtual void mouseRelease(int button, int xMouse, int yMouse);
};


#endif // _CREDITSSCREEN_INCLUDE

