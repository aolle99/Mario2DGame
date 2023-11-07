#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "SoundManager.h"


void Game::init()
{
	bExit = false;
	glClearColor(0.5f, 0.7686f, 1.f, 1.0f);
	SoundManager::instance().init();
	currentScreen = "load_level";
	mainMenu.init("main_menu");
	instructions.init("instructions");
	loadLevel.init("load_level");
	scene.init();
	credits.init("credits");
}

bool Game::update(int deltaTime)
{
	if (currentScreen == "main_menu") {
		mainMenu.update(deltaTime);
	}
	else if (currentScreen == "instructions") {
		instructions.update(deltaTime);
	}
	else if (currentScreen == "load_level") {
		loadLevel.update(deltaTime);
	}
	else if (currentScreen == "game") {
		scene.update(deltaTime);
	}
	else if (currentScreen == "credits") {
		credits.update(deltaTime);
	}
	
	return bExit;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (currentScreen == "main_menu") {
		mainMenu.render();
	}
	else if (currentScreen == "instructions") {
		instructions.render();
	}
	else if (currentScreen == "load_level") {
		loadLevel.render();
	}
	else if (currentScreen == "game") {
		scene.render();
	}
	else if (currentScreen == "credits") {
		credits.render();
	}
}

void Game::setCurrentScreen(string currentScreen)
{
	this->currentScreen = currentScreen;
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bExit = true;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	if (currentScreen == "game") {
		scene.keyReleased(key);
	}
	
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
	if (currentScreen == "main_menu") mainMenu.mouseMove(x, y);
	else if (currentScreen == "instructions") instructions.mouseMove(x, y);
	else if (currentScreen == "load_level") loadLevel.mouseMove(x, y);
	else if (currentScreen == "credits") credits.mouseMove(x, y);
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button, int xMouse, int yMouse)
{

	if (currentScreen == "main_menu") {
		string cScreen = mainMenu.mouseRelease(button, xMouse, yMouse);
		if (cScreen != "main_menu") {
			currentScreen = cScreen;
		}
	}

	else if (currentScreen == "instructions") {
		string cScreen = instructions.mouseRelease(button, xMouse, yMouse);
		if(cScreen != "instructions") {
			currentScreen = cScreen;
		}
	}
	/*
	else if (currentScreen == "load_level") loadLevel.mouseRelease(button);
	else if (currentScreen == "game") scene.mouseRelease(button);
	else if (currentScreen == "credits") credits.mouseRelease(button);*/
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





