#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Text.h"

class Scene
{

public:
	Scene();
	~Scene();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render();

	virtual void mouseMove(int x, int y);
	virtual void mouseRelease(int button, int x, int y);
	virtual void keyReleased(int key);

	virtual bool isGameLevel() const;

private:
	void initShaders();
	void initBackground();

protected:
	ShaderProgram texProgram;
	glm::mat4 projection;
	float currentTime;
	Texture spritesheetBackground;
	Sprite* background;
	string background_image;
	bool pressedLeft, pressedRight, releasedLeft, releasedRight;
	Text text;
};

#endif // _SCENE_INCLUDE

