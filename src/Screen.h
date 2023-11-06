#ifndef _SCREEN_INCLUDE
#define _SCREEN_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Sprite.h"
#include "Text.h"

class Screen
{

public:
	Screen();
	~Screen();

	void init(string currentScreen);
	void update(int deltaTime);
	virtual void render();

	void hover(int which, bool isHover);
	void clicked(int which);

private:
	void initShaders();
	void initBackground();
	void printTexts();

protected:
	ShaderProgram texProgram;
	glm::mat4 projection;
	float currentTime;
	Texture spritesheetBackground;
	Sprite* background;
	string currentScreen;
	bool pressedLeft, pressedRight, releasedLeft, releasedRight;
	Text text;
};

class MainMenu : public Screen
{
	public:
		void render();
};

class Instructions : public Screen
{
	public:
		void render();
};

class Credits : public Screen
{
	public:
		void render();
};

class LoadLevel : public Screen
{
public:
	void render();
};

#endif // _SCREEN_INCLUDE