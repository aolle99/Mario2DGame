#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Enemy.h"
#include "json/json.h"
#include "TileMapStatic.h"
#include <memory>
#include "Item.h"
#include "Text.h"
#include <irrKlang.h>
using namespace irrklang;


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene(int lvl=0);
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

	void keyReleased(int key);

	vector<Enemy*>* getEnemies();

	void quit();
	void pause();
	void resume();
	void restart();



	
private:
	void initShaders();
	bool calculateCameraPosition();

	void buildLevel(const string& levelFile);

	void createEntities(const Json::Value entities);
	void createItemsMap(const Json::Value items_map);


	void updateEnemies(int deltatime);

	void updateItems(int deltaTime);

	void updateCamera();

	void textRenderer();

private:
	bool bPlay; 
	bool bGameOver;
	TileMap *map;
	TileMapStatic* mapDecoration;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	
	glm::vec2 playerStartPos;
	Text text;
	
	int level;
	vector<Enemy*> enemies;
	vector<Item*> items;

protected:
	float cameraX = 0.f;

};


#endif // _SCENE_INCLUDE

