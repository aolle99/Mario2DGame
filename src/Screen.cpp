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

void Screen::printTexts() {

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

void Screen::hover(int which, bool isHover) {

}

void Screen::clicked(int which) {
	
}

void Screen::update(int deltaTime) {
	currentTime += deltaTime;

	background->update(deltaTime);
}

void Screen::render()
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

	text.render("PLAY", glm::vec2(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2), 24, glm::vec4(1, 1, 1, 1));
	text.render("INSTRUCTIONS", glm::vec2(SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 40), 24, glm::vec4(1, 1, 1, 1));
	text.render("EXIT", glm::vec2(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 + 80), 24, glm::vec4(1, 1, 1, 1));
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

	text.render("OBJECTIVES:", glm::vec2(20, 320), 14, glm::vec4(0, 0, 0, 1));
	text.render("- Collect all the coins", glm::vec2(30, 350), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Reach the flag", glm::vec2(30, 370), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Defeat enemies by jumping on them", glm::vec2(30, 390), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Don't fall", glm::vec2(30, 410), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Don't run out of time", glm::vec2(30, 430), 12, glm::vec4(0, 0, 0, 1));

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


