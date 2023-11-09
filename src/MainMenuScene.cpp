#include "MainMenuScene.h"
#include "Game.h"

void MainMenuScene::init()
{
	background_image = "main_menu";
	Scene::init();
}

void MainMenuScene::render()
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

void MainMenuScene::mouseRelease(int button, int xMouse, int yMouse)
{
	if (xMouse > SCREEN_WIDTH / 2 - 50 && xMouse < SCREEN_WIDTH / 2 + 50 && yMouse > 276 && yMouse < 296) {
		Game::instance().showLoadLevel();
	}
	else if (xMouse > SCREEN_WIDTH / 2 - 140 && xMouse < SCREEN_WIDTH / 2 + 140 && yMouse > 316 && yMouse < 336) {
		Game::instance().showInstructions();
	}
	else if (xMouse > SCREEN_WIDTH / 2 - 80 && xMouse < SCREEN_WIDTH / 2 + 80 && yMouse > 356 && yMouse < 376) {
		Game::instance().showCredits();
	}
	else if (xMouse > SCREEN_WIDTH / 2 - 50 && xMouse < SCREEN_WIDTH / 2 + 50 && yMouse > 396 && yMouse < 416) {
		Game::instance().exitGame();
	}
}

void MainMenuScene::keyReleased(int key)
{
	if (key == 27) {
		Game::instance().exitGame();
	}
	else if (key == 13 || key == 32) {
		Game::instance().showLoadLevel();
	}
	else if (key == 105) {
		Game::instance().showInstructions();
	}
	else if (key == 99) {
		Game::instance().showCredits();
	}
	else if (key == 101) {
		Game::instance().exitGame();
	}
}