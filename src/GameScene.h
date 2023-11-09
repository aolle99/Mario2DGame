#ifndef _GAMESCENE_INCLUDE
#define _GAMESCENE_INCLUDE

#include "Scene.h"
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


class GameScene: public Scene
{

public:
	GameScene(int lvl=0);
	~GameScene();

	virtual void init();
	virtual void update(int deltaTime);
	virtual void render();

	virtual void keyReleased(int key);

	virtual bool isGameLevel() const;

	vector<Enemy*>* getEnemies();
	
private:
	bool calculateCameraPosition();

	void buildLevel(const string& levelFile);

	void createEntities(const Json::Value entities);
	void createItemsMap(const Json::Value items_map);


	void updateEnemies(int deltatime);

	void updateItems(int deltaTime);

	void updateCamera();

	void textRenderer();

private:
	bool bGameOver;
	TileMap *map;
	TileMapStatic* mapDecoration;	
	glm::vec2 playerStartPos;
	
	int level;
	vector<Enemy*> enemies;
	vector<Item*> items;

protected:
	float cameraX = 0.f;

};


#endif // _GAMESCENE_INCLUDE

