#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	void SceneFactory(const int scene_id);
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();

	// Scene management
	void showMainMenu();
	void showInstructions();
	void showLoadLevel();
	void showGame();
	void showCredits();
	void showWinScene();


	// Game state management
	void resetGame();
	void nextLevel();
	void changeLevel(int level);

	
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

	void exitGame();

private:
	bool bExit;                       // Continue to play game?
	Scene* scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time


};


#endif // _GAME_INCLUDE


