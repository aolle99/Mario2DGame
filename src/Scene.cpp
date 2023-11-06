#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "json/json.h"
#include <json/value.h>
#include <fstream>
#include "Item.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 20

#define INIT_ENEMY_X_TILES 10
#define INIT_ENEMY_Y_TILES 20


Scene::Scene()
{
	map = NULL;
	player = NULL;
	mushroom = NULL;
	star = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	buildLevel("res/levels/Level_0.ldtkl");
	cameraX = 0.f;
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	
	currentTime = 0.0f;

	mushroom = new Mushroom();
	mushroom->init(glm::vec2(260, 416), texProgram);
	mushroom->setPosition(glm::vec2(10 * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	mushroom->setTileMap(map);
	mushroom->setPlayer(player);

	star = new Star();
	star->init(glm::vec2(360, 416), texProgram);
	star->setPosition(glm::vec2(10 * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	star->setTileMap(map);
	star->setPlayer(player);
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	mushroom->update(deltaTime);
	star->update(deltaTime);

	if (calculateCameraPosition()) {
		projection = glm::ortho(cameraX, float(SCREEN_WIDTH - 1) + cameraX, float(SCREEN_HEIGHT - 1), 0.f);
		this->map->setLeftBound(cameraX);
	}
	
	for (auto& enemy : enemies) {
		enemy->update(deltaTime);
	}
}

void Scene::render()
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

	player->render();

	if (mushroom->isVisible()) mushroom->render();
	if (star->isVisible()) star->render();
	
	for (auto& enemy : enemies) {
		if(!enemy->isDead())enemy->render();
	}
}

bool Scene::calculateCameraPosition()
{
	glm::vec2 newPosPlayer = player->getPosition();
	if (newPosPlayer.x > playerStartPos.x && newPosPlayer.x > (SCREEN_WIDTH-1)/2) {
		float oldCameraX = cameraX;
		cameraX = newPosPlayer.x - (SCREEN_WIDTH - 1) / 2;
		if ((float(SCREEN_WIDTH - 1) + cameraX) > (map->getSize().x*32))
		{
			cameraX = oldCameraX;
			return false;
		}
		playerStartPos.x = newPosPlayer.x;
		return true;
	}
	return false;
	
}

void Scene::buildLevel(const string& levelFile)
{
	string tilesheetFile;

	std::ifstream map_file(levelFile, std::ifstream::binary);
	Json::Value root;
	map_file >> root;

	glm::ivec2 mapSize = glm::ivec2(root["pxWid"].asInt()/32, root["pxHei"].asInt()/32);
	//@TODO: Hi haura problemes ja que entitats necessiten el map
	Json::Value entities, decoration, main;
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
	}

	if (main) map = TileMap::createTileMap(main["gridTiles"], mapSize, texProgram);
	if (decoration) mapDecoration = TileMap::createTileMap(decoration["gridTiles"], mapSize, texProgram);
	if (entities) this->createEntities(entities["entityInstances"]);


	return;
}

void Scene::createEntities(const Json::Value entities)
{
	for (Json::Value entity : entities)
	{
		if (entity["__identifier"].asString() == "Mario")
		{
			player = new Player();
			player->init(glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt()), texProgram);
			player->setTileMap(map);
			playerStartPos = glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt());
		}
		else if (entity["__identifier"].asString() == "Goomba")
		{

			Goomba *goomba = new Goomba();
			goomba->init(glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt()), texProgram);
			goomba->setTileMap(map);
			goomba->setPlayer(player);
			enemies.push_back(std::unique_ptr<Enemy>(goomba));
		}
		else if (entity["__identifier"].asString() == "Koopa")
		{
			Koopa *koopa = new Koopa();
			koopa->init(glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt()), texProgram);
			koopa->setTileMap(map);
			koopa->setPlayer(player);
			enemies.push_back(std::unique_ptr<Enemy>(koopa));
		}
	}	

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



