#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Screen.h"

Screen::Screen()
{

}

Screen::~Screen()
{

}

void Screen::init(string currentScreenStr) {
	initShaders();
	currentTime = 0.0f;
	this->currentScreen = currentScreenStr;
	initBackground();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	if (!text.init("res/Fonts/main_font.ttf")) cout << "Could not load font!!!" << endl;
}

void Screen::initBackground() {
	spritesheetBackground.loadFromFile("res/images/" + currentScreen + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1), glm::vec2(1.0, 1.0), &spritesheetBackground, &texProgram);
	background->setNumberAnimations(1);

	background->setAnimationSpeed(0, 8);
	background->addKeyframe(0, glm::vec2(0.f, 0.f));

	background->changeAnimation(0);
	background->setPosition(glm::vec2(0, 0));
}

void Screen::update(int deltaTime) {
	currentTime += deltaTime;

	background->update(deltaTime);
}

void Screen::render()
{
}

void Screen::mouseMove(int x, int y)
{

}

void MainMenu::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();

	text.render("PLAY", glm::vec2(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 40), 24, glm::vec4(1, 1, 1, 1));
	text.render("INSTRUCTIONS", glm::vec2(SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 80), 24, glm::vec4(1, 1, 1, 1));
	text.render("CREDITS", glm::vec2(SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 120), 24, glm::vec4(1, 1, 1, 1));
	text.render("EXIT", glm::vec2(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 160), 24, glm::vec4(1, 1, 1, 1));
	
}

string MainMenu::mouseRelease(int button, int xMouse, int yMouse)
{
	if (xMouse > SCREEN_WIDTH / 2 - 50 && xMouse < SCREEN_WIDTH / 2 + 50 && yMouse > 276 && yMouse < 296) {
		return "game";
	}
	else if(xMouse > SCREEN_WIDTH / 2 - 140 && xMouse < SCREEN_WIDTH / 2 + 140 && yMouse > 316 && yMouse < 336) {
		return "instructions";
	}
	else if (xMouse > SCREEN_WIDTH / 2 - 80 && xMouse < SCREEN_WIDTH / 2 + 80 && yMouse > 356 && yMouse < 376) {
		return "credits";
	}
	else if (xMouse > SCREEN_WIDTH / 2 - 50 && xMouse < SCREEN_WIDTH / 2 + 50 && yMouse > 396 && yMouse < 416) {
		return "exit";
	}
	else {
		return "main_menu";
	}
}

void Instructions::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();

	text.render("Screen Super Mario Bros", glm::vec2(20, 50), 20, glm::vec4(0, 0, 0, 1));
	text.render("CONTROLS:", glm::vec2(20, 100), 14, glm::vec4(0, 0, 0, 1));
	text.render("- Move left: Arrow Left", glm::vec2(30, 130), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Move right: Arrow Right", glm::vec2(30, 150), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Jump: Arrow Up", glm::vec2(30, 170), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Bend: Arrow Down", glm::vec2(30, 190), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Run: Shift", glm::vec2(30, 210), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Pause: 'P'", glm::vec2(30, 230), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Restart: 'R'", glm::vec2(30, 250), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Quit: 'Q'", glm::vec2(30, 270), 12, glm::vec4(0, 0, 0, 1));

	text.render("OBJECTIVES:", glm::vec2(SCREEN_WIDTH / 2 + 20, 100), 14, glm::vec4(0, 0, 0, 1));
	text.render("- Collect all the coins", glm::vec2(SCREEN_WIDTH / 2 + 30, 130), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Reach the flag", glm::vec2(SCREEN_WIDTH / 2 + 30, 150), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Defeat enemies by jumping on them", glm::vec2(SCREEN_WIDTH / 2 + 30, 170), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Don't fall", glm::vec2(SCREEN_WIDTH / 2 + 30, 190), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Don't run out of time", glm::vec2(SCREEN_WIDTH / 2 + 30, 210), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Collect all the coins", glm::vec2(SCREEN_WIDTH / 2 + 30, 230), 12, glm::vec4(0, 0, 0, 1));

	text.render("Return to Main Menu", glm::vec2(20, SCREEN_HEIGHT - 20), 14, glm::vec4(0, 0, 0, 1));
}

string Instructions::mouseRelease(int button, int xMouse, int yMouse)
{
	if(xMouse >= 20 && xMouse < 278 && yMouse > SCREEN_HEIGHT - 20 - 14 && yMouse < SCREEN_HEIGHT - 20) {
		return "main_menu";
	}
	else {
		return "instructions";
	}
}

void Credits::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();

	text.render("CREDITS:", glm::vec2(20, 50), 20, glm::vec4(0, 0, 0, 1));
	text.render("Game created by:", glm::vec2(20, 100), 14, glm::vec4(0, 0, 0, 1));
	text.render("- Alex Olle and Cristina Migo", glm::vec2(30, 120), 14, glm::vec4(0, 0, 0, 1));
	text.render("Sprites from:", glm::vec2(20, 150), 14, glm::vec4(0, 0, 0, 1));
	text.render("- https://www.spriters-resource.com/nes/supermariobros/", glm::vec2(30, 170), 14, glm::vec4(0, 0, 0, 1));
	text.render("Music from:", glm::vec2(20, 200), 14, glm::vec4(0, 0, 0, 1));
	text.render("- https://www.youtube.com/watch?v=7q5VUcaDOZI", glm::vec2(30, 220), 14, glm::vec4(0, 0, 0, 1));
	text.render("- https://www.youtube.com/watch?v=7q5VUcaDOZI", glm::vec2(30, 240), 14, glm::vec4(0, 0, 0, 1));



	text.render("Return to Main Menu", glm::vec2(20, SCREEN_HEIGHT - 20), 14, glm::vec4(0, 0, 0, 1));
}

string Credits::mouseRelease(int button, int xMouse, int yMouse)
{
	if (xMouse >= 20 && xMouse < 278 && yMouse > SCREEN_HEIGHT - 20 - 14 && yMouse < SCREEN_HEIGHT - 20) {
		return "main_menu";
	}
	else {
		return "credits";
	}
}

void LoadLevel::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();

	text.render("WORLD 1-1", glm::vec2(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2), 24, glm::vec4(1, 1, 1, 1));
	text.render("LIVES: 5", glm::vec2(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + 40), 16, glm::vec4(1, 1, 1, 1));
}

void LoadLevel::update(int deltaTime)
{
	loading_time += 1;
	background->update(deltaTime);
}

int LoadLevel::getLoadingTime() {
	return loading_time;
}

void Screen::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


