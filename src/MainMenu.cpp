#include "MainMenu.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>


MainMenu::MainMenu()
{

}

MainMenu::~MainMenu()
{

}

void MainMenu::init() {
    initShaders();
    currentTime = 0.0f;
    initSpriteBackground();
    //initSpriteLogo();
    //printTextsActions();
    projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
    currentTime = 0.0f;
}

void MainMenu::update(int deltaTime)
{
    currentTime += deltaTime;
	background->update(deltaTime);

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
}

void MainMenu::initSpriteBackground() {
    spritesheetBackground.loadFromFile("res/images/main_menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
    background = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1), glm::vec2(1.0, 1.0), &spritesheetBackground, &texProgram);
    background->setNumberAnimations(1);

    background->setAnimationSpeed(0, 8);
    background->addKeyframe(0, glm::vec2(0.f, 0.f));

    background->changeAnimation(0);
    background->setPosition(glm::vec2(0, 0));
}


void MainMenu::initShaders()
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