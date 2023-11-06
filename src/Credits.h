#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"

class Credits
{

public:
	Credits();
	~Credits();

	void init();
	void update(int deltaTime);
	void render();

	void hover(int which, bool isHover);
	void clicked(int which);

private:
	void initShaders();
	void initBackground();
	void printTexts();

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	float currentTime;
	Texture spritesheetBackground;
	Sprite* background;
	int currentScreen;
};


#endif // _CREDITS_INCLUDE