#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "json/json.h"
#include "TileMapStatic.h"
#include <memory>
#include "Item.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

	bool calculateCameraPosition();

	void buildLevel(const string& levelFile);

	void createEntities(const Json::Value entities);

	

	

private:
	void initShaders();

private:
	TileMap *map;
	TileMapStatic* mapDecoration;
	Player *player;
	vector< std::unique_ptr<Enemy >> enemies;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	float cameraX = 0.f;
	glm::vec2 playerStartPos;

	Mushroom *mushroom;
	Star *star;



};


#endif // _SCENE_INCLUDE

