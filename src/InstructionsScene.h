#ifndef _INSTRUCTIONSSCENE_INCLUDE
#define _INSTRUCTIONSSCENE_INCLUDE

#include "Scene.h"

class InstructionsScene : public Scene
{
public:
	virtual void init();
	virtual void render();
	virtual void mouseRelease(int button, int xMouse, int yMouse);
	void keyReleased(int key);
};

#endif // _INSTRUCTIONSSCENE_INCLUDE