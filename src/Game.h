#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Screen.h"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	void setCurrentScreen(string currentScreen);
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button, int xMouse, int yMouse);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	bool bExit;                       // Continue to play game?
	Scene scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	MainMenu mainMenu;
	Instructions instructions;
	Credits credits;
	LoadLevel loadLevel;
	string currentScreen;

};


#endif // _GAME_INCLUDE


