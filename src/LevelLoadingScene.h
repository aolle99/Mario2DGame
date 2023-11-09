#ifndef LEVELLOADINGSCENE
#define LEVELLOADINGSCENE

#include "Scene.h"

class LevelLoadingScene : public Scene
{
public:
	virtual void init();
	virtual void render();
	virtual void update(int deltaTime);
	int getLoadingTime();

private:
	int loading_time;
};
#endif // !LEVELLOADINGSCENE
