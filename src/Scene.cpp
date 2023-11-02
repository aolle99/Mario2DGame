#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "json/json.h"
#include <json/value.h>
#include <fstream>


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
	
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	
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
	
	for (auto& enemy : enemies) {
		enemy->render();
	}
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
		}
		else if (entity["__identifier"].asString() == "Goomba")
		{

			Goomba *goomba = new Goomba();
			goomba->init(glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt()), texProgram);
			goomba->setTileMap(map);
			enemies.push_back(std::unique_ptr<Enemy>(goomba));
		}
		else if (entity["__identifier"].asString() == "Koopa")
		{
			Koopa *koopa = new Koopa();
			koopa->init(glm::vec2(entity["px"][0].asInt(), entity["px"][1].asInt()), texProgram);
			koopa->setTileMap(map);
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



