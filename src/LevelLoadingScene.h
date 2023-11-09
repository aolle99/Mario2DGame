#ifndef LEVELLOADINGSCENE_INCLUDE
#define LEVELLOADINGSCENE_INCLUDE

#include "Scene.h"

class LevelLoadingScene : public Scene
{
public:
	virtual void init();
	virtual void render();
	virtual void update(int deltaTime);

private:
	int loading_time;
};
#endif // LEVELLOADINGSCENE_INCLUDE
