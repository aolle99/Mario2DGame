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

	void mouseMove(int x, int y);

private:
	void initShaders();
	void initBackground();

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
		string mouseRelease(int button, int xMouse, int yMouse);
};

class Instructions : public Screen
{
	public:
		void render();
		string mouseRelease(int button, int xMouse, int yMouse);
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