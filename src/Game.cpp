#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "SoundManager.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "MainMenuScene.h"
#include "InstructionsScene.h"
#include "CreditsScene.h"
#include "LevelLoadingScene.h"
#include "WinScene.h"
#include "GameScene.h"

#define LOAD_LEVEL_TIME 80



void Game::init()
{
	bExit = false;
	glClearColor(0.5f, 0.7686f, 1.f, 1.0f);

	SoundManager::instance().init();
	GameManager::instance().init();
	PunctuationDisplay::instance().init();

	SoundManager::instance().setVolume(0.3f);

	showMainMenu();
}

bool Game::update(int deltaTime)
{
	scene->update(deltaTime);
	
	return bExit;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	scene->render();
}

void Game::showMainMenu()
{
	delete scene;
	scene = new MainMenuScene();
	scene->init();
}

void Game::showInstructions()
{
	delete scene;
	scene = new InstructionsScene();
	scene->init();
}

void Game::showLoadLevel()
{
	delete scene;
	scene = new LevelLoadingScene();
	scene->init();
}

void Game::showGame()
{
	delete scene;
	GameManager::instance().resetLevel();
	int level = GameManager::instance().getLevel();
	scene = new GameScene(level);
	scene->init();
}

void Game::showCredits()
{
	delete scene;
	scene = new CreditsScene();
	scene->init();
}

void Game::showWinScene()
{
	delete scene;
	scene = new WinScene();
	scene->init();
}

void Game::resetGame()
{
	GameManager::instance().init();
	showLoadLevel();
}

void Game::nextLevel()
{
	if (GameManager::instance().hasNextLevel()) {
		this->changeLevel(GameManager::instance().getLevel()+1);
	}
	else {
		showWinScene();
	}

}

void Game::changeLevel(int level)
{
	if (GameManager::instance().getLevel() != level && GameManager::instance().levelExists(level)) {
		GameManager::instance().setLevel(level);
		SoundManager::instance().stopMusic();
		showLoadLevel();
	}
		
}

void Game::keyPressed(int key)
{
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	scene->keyReleased(key);
	keys[key] = false;
	
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
	scene->mouseMove(x, y);
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button, int xMouse, int yMouse)
{
	scene->mouseRelease(button, xMouse, yMouse);
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::exitGame()
{
	bExit = true;
}





