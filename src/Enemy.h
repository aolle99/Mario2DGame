#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Enemy is basically a Sprite that represents an enemy. As such it has
// all properties it needs to track its movement and collisions.

class Enemy
{
public:
	void move();
	void die();
	virtual void update(int deltaTime);
	void render();
	virtual void collisionDeath();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	void changeDirection();
	bool isDead();
	virtual bool isKoopaShellMove();
	virtual bool isModeTurtle();
	bool collisionEnemies(const glm::vec2& pos, const glm::ivec2& size);

	glm::ivec2 getPosition();
	glm::ivec2 getSize();

protected:
	bool bFalling;
	bool bLeft;
	bool bDying;
	bool bDead;
	int currentTime;
	int speed;
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
	virtual void update(int deltaTime);
	virtual void collisionDeath();
	void smashedDeath();

private:
	bool bSmashed;
};

class Koopa : public Enemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void collisionDeath();
	void shellMode();
	void turtleMode();
	void change_to_shell();
	void change_to_turtle();
	virtual bool isKoopaShellMove();
	virtual bool isModeTurtle();

protected:
	bool bStop;
	bool bShell;
};


#endif // _ENEMY_INCLUDE