#ifndef WINSCENE_INCLUDE
#define WINSCENE_INCLUDE

#include "Scene.h"

class WinScene : public Scene
{
public:
	virtual void init();
	virtual void render();
	virtual void keyReleased(int deltaTime);

private:
	int loading_time;
};
#endif // WINSCENE_INCLUDE
