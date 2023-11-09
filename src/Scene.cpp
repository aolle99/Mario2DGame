#include "Scene.h"
#include "Game.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/glut.h>

Scene::Scene()
{
	background = nullptr;
}

Scene::~Scene()
{
	if (background != nullptr) delete background;
}

void Scene::init() {
	initShaders();
	currentTime = 0.0f;
	initBackground();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	if (!text.init("res/Fonts/main_font.ttf")) cout << "Could not load font!!!" << endl;
}

void Scene::initBackground() {
	if(background_image == "") return;
	spritesheetBackground.loadFromFile("res/images/" + background_image + ".png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1), glm::vec2(1.0, 1.0), &spritesheetBackground, &texProgram);
	background->setNumberAnimations(1);

	background->setAnimationSpeed(0, 8);
	background->addKeyframe(0, glm::vec2(0.f, 0.f));

	background->changeAnimation(0);
	background->setPosition(glm::vec2(0, 0));
}

void Scene::update(int deltaTime) {
	currentTime += deltaTime;
}

void Scene::render()
{
}

void Scene::mouseMove(int x, int y)
{

}

void Scene::mouseRelease(int button, int x, int y)
{
}

void Scene::keyReleased(int key)
{
}

bool Scene::isGameLevel() const
{
	return false;
}

void Scene::initShaders()
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

