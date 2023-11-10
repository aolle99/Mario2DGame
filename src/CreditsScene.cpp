#include "CreditsScene.h"
#include "Game.h"



void CreditsScene::init()
{
	background_image = "credits";
	Scene::init();
}

void CreditsScene::render() {
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
	text.render("- https://github.com/PfAndrey/supermariohd", glm::vec2(30, 170), 14, glm::vec4(0, 0, 0, 1));
	text.render("Music from:", glm::vec2(20, 200), 14, glm::vec4(0, 0, 0, 1));
	text.render("- https://github.com/PfAndrey/supermariohd", glm::vec2(30, 220), 14, glm::vec4(0, 0, 0, 1));

	text.render("Return to Main Menu", glm::vec2(20, SCREEN_HEIGHT - 20), 14, glm::vec4(0, 0, 0, 1));
}

void CreditsScene::mouseRelease(int button, int xMouse, int yMouse)
{
	if (xMouse >= 20 && xMouse < 278 && yMouse > SCREEN_HEIGHT - 20 - 14 && yMouse < SCREEN_HEIGHT - 20) {
		Game::instance().showMainMenu();
	}
}

void CreditsScene::keyReleased(int key)
{
	if (key == 27) {
		Game::instance().showMainMenu();
	}
}