#include "InstructionsScene.h"
#include "Game.h"


void InstructionsScene::init()
{
	background_image = "instructions";
	Scene::init();
}

void InstructionsScene::render() {
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
	text.render("- Jump: SPACE / Arrow Up", glm::vec2(30, 170), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Bend: Arrow Down", glm::vec2(30, 190), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Run: Shift", glm::vec2(30, 210), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Pause: 'P'", glm::vec2(30, 230), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Restart: 'R'", glm::vec2(30, 250), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Quit: 'Q'", glm::vec2(30, 270), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Go to level 1: '1'", glm::vec2(30, 290), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Go to level 2: '2'", glm::vec2(30, 310), 12, glm::vec4(0, 0, 0, 1));

	// write the objectives next to the controls
	text.render("OBJECTIVES:", glm::vec2(SCREEN_WIDTH / 2 + 20, 100), 14, glm::vec4(0, 0, 0, 1));
	text.render("- Collect all the coins", glm::vec2(SCREEN_WIDTH / 2 + 30, 130), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Reach the flag", glm::vec2(SCREEN_WIDTH / 2 + 30, 150), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Defeat enemies by jumping on them", glm::vec2(SCREEN_WIDTH / 2 + 30, 170), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Don't fall", glm::vec2(SCREEN_WIDTH / 2 + 30, 190), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Don't run out of time", glm::vec2(SCREEN_WIDTH / 2 + 30, 210), 12, glm::vec4(0, 0, 0, 1));
	text.render("- Collect all the coins", glm::vec2(SCREEN_WIDTH / 2 + 30, 230), 12, glm::vec4(0, 0, 0, 1));

	text.render("Return to Main Menu", glm::vec2(20, SCREEN_HEIGHT - 20), 14, glm::vec4(0, 0, 0, 1));
}

void InstructionsScene::mouseRelease(int button, int xMouse, int yMouse)
{
	if (xMouse >= 20 && xMouse < 278 && yMouse > SCREEN_HEIGHT - 20 - 14 && yMouse < SCREEN_HEIGHT - 20) {
		Game::instance().showMainMenu();
	}
}