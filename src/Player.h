#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	static Player& instance()
	{
		static Player P;

		return P;
	}
	void init(glm::vec2& startPos, ShaderProgram &shaderProgram);
	void changeToMario();
	void changeToSuperMario();
	void move(bool direction);
	void die();
	void jump();
	void bend();
	void run();
	bool checkJumping();
	void marioDying();
	void update(int deltaTime);
	void render();
	void animateTransformationMario(bool toSupermario);
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	bool collision(const glm::ivec2& object_pos, const glm::ivec2& object_size);

	bool collisionDown(const glm::ivec2& pos, const glm::ivec2& size, bool isItem);

	bool collisionUp(const glm::ivec2& object_pos, const glm::ivec2& object_size);

	bool collisionLeft(const glm::ivec2& object_pos, const glm::ivec2& object_size);

	bool collisionRight(const glm::ivec2& object_pos, const glm::ivec2& object_size);

	glm::ivec2 getPosition();

	glm::ivec2 getSize();

	void damagePlayer();

	void giveStar();

	void giveMushroom();

	bool isMarioStar();

	bool isSuperMario();

	bool isInvulnerable();

	bool isDead();

	bool isDying();

	int getHp();

	void setDying(bool dying);

	void setInvulnerable(bool invulnerable);

	void setInvTime(int time);

	void setEndPos(glm::ivec2 pos);

	void removeCollisionBlock(int x, int y);

	void resetCurrentTime();


	
private:
	Player() {};
	glm::ivec2 getHitboxPosition();

	void animationEnd(int deltaTime);
	
private:
	bool bJumping;
	bool bFalling;
	bool bLeft;
	bool bDying;
	bool bDead;
	bool bInvulnerable;
	bool bBounce;
	bool bShow;
	bool bCanMove;
	bool bChanging;
	bool bGrowing;
	float star;
	int invTime;
	int bounceTime;
	glm::ivec2 posPlayer;
	glm::ivec2 size;
	glm::ivec2 hitbox;
	int jumpAngle, startY;
	Texture spritesheet;
	ShaderProgram shaderProgram;
	Sprite *sprite;
	TileMap *map;
	int currentTime;
	int hp;
	int score;
	int animStep;
	glm::ivec2 endLevelPos;
	float moveSpeed;
	int maxSpeed;
	int jumpTimer;
};


#endif // _PLAYER_INCLUDE


