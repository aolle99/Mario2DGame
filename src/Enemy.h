#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"


// Enemy is basically a Sprite that represents an enemy. As such it has
// all properties it needs to track its movement and collisions.

class Enemy
{
public:
	void move(bool direction);
	void die();
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

protected:
	bool bFalling;
	bool bLeft;
	glm::ivec2 posEnemy;
	int jumpAngle, startY;
	glm::vec2 sizeEnemy;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Player mario;
};

class Goomba : public Enemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);

private:
};

class Koopa : public Enemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void move_shell(bool direction);

protected:
	bool bStop;
};


#endif // _ENEMY_INCLUDE