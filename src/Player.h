#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(glm::vec2& startPos, ShaderProgram &shaderProgram);
	void changeToMario();
	void changeToSuperMario();
	void move(bool direction);
	void die();
	void jump();
	void bend();
	void run();
	bool checkJumping();
	void update(int deltaTime);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	bool collisionDown(const glm::ivec2& pos, const glm::ivec2& size, int object_type);

	bool collisionUp(const glm::ivec2& object_pos, const glm::ivec2& object_size, int object_type);

	bool collisionLeft(const glm::ivec2& object_pos, const glm::ivec2& object_size, int object_type);

	bool collisionRight(const glm::ivec2& object_pos, const glm::ivec2& object_size, int object_type);

	glm::ivec2 getPosition();

	glm::ivec2 getSize();

	void damagePlayer();

	void giveStar();

	void giveMushroom();

	bool isMarioStar();
	
private:
	bool bJumping;
	bool bFalling;
	bool bLeft;
	float star;
	glm::ivec2 posPlayer;
	glm::ivec2 size;
	glm::ivec2 hitbox;
	int jumpAngle, startY;
	Texture spritesheet;
	ShaderProgram shaderProgram;
	Sprite *sprite;
	TileMap *map;
	int speed;
	int hp;

};


#endif // _PLAYER_INCLUDE


