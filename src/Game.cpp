#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.5f, 0.7686f, 1.f, 1.0f);
	currentScreen = "game";
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
	
	return bPlay;
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
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
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
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





