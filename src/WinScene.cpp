#include "WinScene.h"
#include "Game.h"
#include "GameManager.h"

# define WIN_TIME 2000

void WinScene::init()
{
	background_image = "win_screen";
	Scene::init();
	loading_time = 0;
}

void WinScene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();

	int score = GameManager::instance().getScore();
	text.render("congratulations", glm::vec2(SCREEN_WIDTH / 2 - 400, SCREEN_HEIGHT / 2 - 40), 28, glm::vec4(1, 1, 1, 1));
	text.render("you won", glm::vec2(SCREEN_WIDTH / 2 - 290, SCREEN_HEIGHT / 2), 28, glm::vec4(1, 1, 1, 1));
	text.render("TOTAL SCORE: " + std::to_string(score), glm::vec2(SCREEN_WIDTH / 2 - 330, SCREEN_HEIGHT / 2 + 60), 20, glm::vec4(1, 1, 1, 1));
	text.render("Press any key to continue", glm::vec2(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 200), 16, glm::vec4(1, 1, 1, 1));
}

void WinScene::keyReleased(int key)
{
	Game::instance().showCredits();
}