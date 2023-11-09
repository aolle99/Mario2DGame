#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "GameScene.h"
#include "Game.h"
#include "json/json.h"
#include <json/value.h>
#include <fstream>
#include "Item.h"
#include "Player.h"
#include "SoundManager.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 20

#define INIT_ENEMY_X_TILES 10
#define INIT_ENEMY_Y_TILES 20


GameScene::GameScene(int lvl)
{
	this->level = lvl;
	map = NULL;
	mapDecoration = NULL;
}

GameScene::~GameScene()
{
	if(map != NULL)
		delete map;

	if (mapDecoration != NULL)
		delete mapDecoration;

	for (auto& enemy : enemies) {
		delete enemy;
	}
	enemies.clear();

	for (auto& item : items) {
		delete item;
	}
	items.clear();
}


void GameScene::init()
{
	Scene::init();

	PunctuationDisplay::instance().clear();
	buildLevel("res/levels/Level_" + to_string(level) + ".ldtkl");
	cameraX = 0.f;
	
	GameManager::instance().setScrollX(glm::vec2(cameraX,SCREEN_WIDTH-1));

	for (auto& item : items) {
		item->setTileMap(map);
	}

	text.init("res/Fonts/main_font.ttf");



}

void GameScene::update(int deltaTime)
{
	if (!GameManager::instance().isPaused() && !GameManager::instance().isGameOver()) {
		currentTime += deltaTime;
		Player::instance().update(deltaTime);
		GameManager::instance().update(deltaTime);
		PunctuationDisplay::instance().update(deltaTime);

		updateCamera();

		updateEnemies(deltaTime);

		updateItems(deltaTime);

		checkPlayerAlive();

		map->update(deltaTime);
	}
}

void GameScene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	mapDecoration->render();
	map->render();

	if(!Player::instance().isDead())Player::instance().render();

	for (auto& enemy : enemies) {
		enemy->render();
	}

	for (const auto& item : items) {
		item->render();
	}

	textRenderer();
}



void GameScene::keyReleased(int key)
{
	if (key == 80 || key == 112) { // P or p
		GameManager::instance().setPaused(true);
        SoundManager::instance().stopMusic();
        SoundManager::instance().playSound("res/sounds/pause.wav");
	}
	else if (key == 82 || key == 114) {
		Game::instance().resetGame();
	}
	else if (key == 81 || key == 113) {
		GameManager::instance().init();
		Game::instance().showMainMenu();
	}
	else if (GameManager::instance().isPaused() && key == 32) {
		GameManager::instance().setPaused(false);
		if(GameManager::instance().isLevelStarted())
			SoundManager::instance().playMusic("res/music/overworld_resumed.ogg");
		else
			SoundManager::instance().playMusic("res/music/overworld.ogg");
	}
	else if (key == 49) {
		Game::instance().changeLevel(0);
	}
	else if (key == 50) {
		Game::instance().changeLevel(1);
	}
	
}

bool GameScene::calculateCameraPosition()
{
	glm::vec2 newPosPlayer = Player::instance().getPosition();
	if (newPosPlayer.x > playerStartPos.x && newPosPlayer.x > (SCREEN_WIDTH - 1) / 2) {
		float oldCameraX = cameraX;
		cameraX = newPosPlayer.x - (SCREEN_WIDTH - 1) / 2;
		if ((float(SCREEN_WIDTH - 1) + cameraX) > (map->getSize().x * 32))
		{
			cameraX = oldCameraX;
			return false;
		}
		GameManager::instance().setScrollX(glm::vec2(cameraX, float(SCREEN_WIDTH - 1) + cameraX));
		playerStartPos.x = newPosPlayer.x;
		return true;
	}
	return false;

}

void GameScene::buildLevel(const string& levelFile)
{
	string tilesheetFile;

	std::ifstream map_file(levelFile, std::ifstream::binary);
	Json::Value root;
	map_file >> root;

	glm::ivec2 mapSize = glm::ivec2(root["pxWid"].asInt() / 32, root["pxHei"].asInt() / 32);

	Json::Value entities, decoration, main, items_map;
	for (Json::Value type : root["layerInstances"])
	{
		if (type["__identifier"].asString() == "Entities")
		{
			entities = type;
		}
		else if (type["__identifier"].asString() == "Main")
		{
			main = type;
		}
		else if (type["__identifier"].asString() == "Decoration")
		{
			decoration = type;
		}
		else if (type["__identifier"].asString() == "Items")
		{
			items_map = type;
		}

	}

	if (main) map = TileMap::createTileMap(main["gridTiles"], mapSize, texProgram);
	if (decoration) mapDecoration = TileMapStatic::createTileMap(decoration["gridTiles"], mapSize, texProgram);
	if (entities) this->createEntities(entities["entityInstances"]);
	if (items_map) this->createItemsMap(items_map["entityInstances"]);

	vector<Item*> map_items = map->getItems();

	for (auto& item : map_items) {
		items.push_back(item);
	}

	return;
}

void GameScene::createEntities(const Json::Value entities)
{
	for (Json::Value entity : entities)
	{
		if (entity["__identifier"].asString() == "Mario")
		{
			Player::instance().init(glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt()), texProgram);
			Player::instance().setTileMap(map);
			playerStartPos = glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt());
			break;
		}
	}
	for (Json::Value entity : entities)
	{

		if (entity["__identifier"].asString() == "Goomba")
		{

			Goomba* goomba = new Goomba();
			goomba->init(glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt()), texProgram);
			goomba->setTileMap(map);
			enemies.push_back(goomba);
		}
		else if (entity["__identifier"].asString() == "Koopa")
		{
			Koopa* koopa = new Koopa();
			koopa->init(glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt()), texProgram);
			koopa->setTileMap(map);
			enemies.push_back(koopa);
		}
	}

}

void GameScene::createItemsMap(const Json::Value items_map) {
	for (Json::Value item : items_map)
	{
		if (item["__identifier"].asString() == "Mushroom")
		{
			Mushroom* mushroom = new Mushroom();
			mushroom->init(glm::vec2(item["px"][0].asInt(), item["px"][1].asInt()), texProgram);
			mushroom->show();
			items.push_back(mushroom);
		}
		else if (item["__identifier"].asString() == "Star")
		{
			Star* star = new Star();
			star->init(glm::vec2(item["px"][0].asInt(), item["px"][1].asInt()), texProgram);
			star->show();
			items.push_back(star);
		}
		else if (item["__identifier"].asString() == "Coin")
		{
			Coin* coin = new Coin();
			coin->init(glm::vec2(item["px"][0].asInt(), item["px"][1].asInt()), texProgram);
			coin->show();
			items.push_back(coin);
		}
		else if (item["__identifier"].asString() == "Flag")
		{
			glm::ivec2 minCoords = glm::ivec2(item["fieldInstances"][0]["__value"]["cx"].asInt(), item["fieldInstances"][0]["__value"]["cy"].asInt());
			Flag* flag = new Flag();
			flag->init(glm::vec2(item["px"][0].asInt(), item["px"][1].asInt()), texProgram, minCoords);
			flag->show();
			items.push_back(flag);
		}
		else if (item["__identifier"].asString() == "Castle_Flag")
		{
			CastleFlag* castleFlag = new CastleFlag();
			castleFlag->init(glm::vec2(item["px"][0].asInt(), item["px"][1].asInt()), texProgram);
			items.push_back(castleFlag);
		}
		else if (item["__identifier"].asString() == "End_pivot")
		{
			int mult = item["fieldInstances"][0]["__value"].asInt();
			EndPivot* endpivot = new EndPivot();
			endpivot->init(glm::vec2(item["px"][0].asInt(), item["px"][1].asInt()), texProgram, mult);
			endpivot->show();
			items.push_back(endpivot);
		}
		else if (item["__identifier"].asString() == "EndLevel")
		{
			Player::instance().setEndPos(glm::vec2(item["px"][0].asInt(), item["px"][1].asInt()));
		}

	}
}

bool GameScene::isGameLevel() const
{
	return true;
}

void GameScene::updateEnemies(int deltatime) {
	for (auto& enemy : enemies) {
		if (enemy->isDead())
			enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy* enemy) {return enemy->isDead(); }), enemies.end());
		else {
			enemy->update(deltatime);
			if (enemy->isKoopaShellMove()) {
				for (auto& enemy2 : enemies) {
					if (enemy != enemy2) {
						if (enemy->collisionEnemies(enemy2->getPosition(), enemy2->getSize())) enemy2->collisionDeath();
					}
				}
			}
			else if (enemy->isModeTurtle()) {
				for (auto& enemy2 : enemies) {
					if (enemy != enemy2) {
						if (enemy2->isModeTurtle()) {
							if (enemy->collisionEnemies(enemy2->getPosition(), enemy2->getSize())) {
								enemy->changeDirection();
								enemy2->changeDirection();
							}
						}
					}
				}
			}
		}
	}
}

void GameScene::updateItems(int deltaTime) {
	for (auto& item : items) {
		if(item->isUsed()) items.erase(std::remove_if(items.begin(), items.end(), [](Item* item) {return item->isUsed(); }), items.end());
		else
			item->update(deltaTime);
	}
}

void GameScene::updateCamera() {
	if (calculateCameraPosition()) {
		projection = glm::ortho(cameraX, float(SCREEN_WIDTH - 1) + cameraX, float(SCREEN_HEIGHT - 1), 0.f);
		this->map->setLeftBound(cameraX);
	}
}

void GameScene::checkPlayerAlive()
{
	if (Player::instance().isDead()) {
		GameManager::instance().substractLive();
		if (GameManager::instance().getLives() > 0) {
			Game::instance().showLoadLevel();
		}
		else {
			GameManager::instance().setGameOver(true);
		}
	}
	else if (GameManager::instance().getTime() <= 0) {
		GameManager::instance().setGameOver(2);
	}
}

void GameScene::textRenderer() {
	string score = to_string(GameManager::instance().getScore());
	string coins = to_string(GameManager::instance().getCoins());
	string time = to_string(GameManager::instance().getTime());
	string level = to_string(GameManager::instance().getLevel());
	string lives = to_string(GameManager::instance().getLives());

	text.render("LIVES: " + lives, glm::vec2(SCREEN_WIDTH / 5 * 0 + 30, 25), 16, glm::vec4(1, 1, 1, 1));
	text.render("SCORE: " + score, glm::vec2(SCREEN_WIDTH / 5 * 1 + 30, 25), 16, glm::vec4(1, 1, 1, 1));
	text.render("COINS: " + coins, glm::vec2(SCREEN_WIDTH / 5 * 2 + 30, 25), 16, glm::vec4(1, 1, 1, 1));
	text.render("WORLD: 1-" + level, glm::vec2(SCREEN_WIDTH / 5 * 3 + 30, 25), 16, glm::vec4(1, 1, 1, 1));
	text.render("TIME: " + time, glm::vec2(SCREEN_WIDTH / 5 * 4 + 30, 25), 16, glm::vec4(1, 1, 1, 1));

	if (GameManager::instance().isPaused()) {
		string textToRender = "start";
		if(GameManager::instance().isLevelStarted())
			textToRender = "continue";
			
		text.render("Press 'SPACE' to " + textToRender, glm::vec2(SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2), 16, glm::vec4(1, 1, 1, 1));
	}

	if (GameManager::instance().isGameOver() == 2) {
		GameManager::instance().setGameOver(2);
		text.render("GAME OVER", glm::vec2(SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT / 2 - 50), 32, glm::vec4(1, 0, 0, 1));
		text.render("you run out of time", glm::vec2(SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 20), 16, glm::vec4(1, 0, 0, 1));
		text.render("Press 'Q' to quit", glm::vec2(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 20), 16, glm::vec4(1, 1, 1, 1));
		text.render("Press 'R' to restart", glm::vec2(SCREEN_WIDTH / 2 - 145, SCREEN_HEIGHT / 2 + 40), 16, glm::vec4(1, 1, 1, 1));
        SoundManager::instance().stopMusic();
        SoundManager::instance().playSound("res/sounds/game_over.wav");
	}

	if (GameManager::instance().isGameOver() == 1) {
		GameManager::instance().setGameOver(1);
		text.render("GAME OVER", glm::vec2(SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT / 2 - 50), 32, glm::vec4(1, 0, 0, 1));
		text.render("you run out of lives", glm::vec2(SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 - 20), 16, glm::vec4(1, 0, 0, 1));
		text.render("Press 'Q' to quit", glm::vec2(SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 20), 16, glm::vec4(1, 1, 1, 1));
		text.render("Press 'R' to restart", glm::vec2(SCREEN_WIDTH / 2 - 145, SCREEN_HEIGHT / 2 + 40), 16, glm::vec4(1, 1, 1, 1));
        SoundManager::instance().stopMusic();
        SoundManager::instance().playSound("res/sounds/game_over.wav");
	}

	PunctuationDisplay::instance().render();
}

vector<Enemy*>* GameScene::getEnemies()
{
	return &enemies;
}



