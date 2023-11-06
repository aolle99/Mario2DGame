#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Enemy is basically a Sprite that represents an enemy. As such it has
// all properties it needs to track its movement and collisions.

class Enemy
{
public:
	void move(bool direction);
	void die();
	virtual void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool isDead();

protected:
	bool bFalling;
	bool bLeft;
	bool bDying;
	bool bDead;
	int currentTime;
	glm::ivec2 posEnemy, sizeEnemy;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	ShaderProgram shaderProgram;
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
	void change_to_shell();
	void change_to_turtle();

protected:
	bool bStop;
	bool bShell;
};


#endif // _ENEMY_INCLUDE