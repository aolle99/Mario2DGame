#include "LevelLoadingScene.h"
#include "Game.h"
#include "GameManager.h"

# define LOAD_LEVEL_TIME 1000

void LevelLoadingScene::init()
{
	background_image = "load_level";
	Scene::init();
	loading_time = 0;
}

void LevelLoadingScene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();

	int level = GameManager::instance().getLevel();
	int lives = GameManager::instance().getLives();
	text.render("WORLD 1-" + std::to_string(level+1), glm::vec2(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2), 24, glm::vec4(1, 1, 1, 1));
	text.render("LIVES: " + std::to_string(lives), glm::vec2(SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 + 40), 16, glm::vec4(1, 1, 1, 1));
}

void LevelLoadingScene::update(int deltaTime)
{
	loading_time += deltaTime;
	if(loading_time >= LOAD_LEVEL_TIME)
		Game::instance().showGame();
}

int LevelLoadingScene::getLoadingTime() {
	return loading_time;
}