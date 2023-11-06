#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"

class Instructions
{

public:
	Instructions();
	~Instructions();

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
	Sprite *background;
	int currentScreen;
	bool pressedLeft, pressedRight, releasedLeft, releasedRight;
};


#endif // _CREDITS_INCLUDE