#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 150
#define FALL_STEP 4

enum EnemyAnims
{
	MOVE, DIE, SHELL_STOP, SHELL_MOVE
};

void Enemy::move(bool direction)
{
	posEnemy.y += FALL_STEP;
	if (!map->collisionMoveDown(posEnemy, sizeEnemy, &posEnemy.y))
		return;
	
	if (direction) { // left
		posEnemy.x += 2;
		if (map->collisionMoveRight(posEnemy, sizeEnemy))
		{
			bLeft = !bLeft;
		}

	}
	else { // right
		posEnemy.x -= 2;
		if (map->collisionMoveLeft(posEnemy, sizeEnemy)) // mira si hi ha colisio a l'esquerra
		{
			bLeft = !bLeft;
		}
	}
}

void Koopa::move_shell(bool direction)
{
	sprite->changeAnimation(SHELL_MOVE);
}

void Enemy::die()
{
	sprite->changeAnimation(DIE);
}

void Enemy::update(int deltaTime)
{
	sprite->update(deltaTime);
	this->move(bLeft);

	sprite->setPosition(posEnemy);
}

void Koopa::update(int deltaTime)
{
	sprite->update(deltaTime);
	
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		bStop = false;
		sprite->changeAnimation(SHELL_MOVE);
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		bStop = true;
		sprite->changeAnimation(SHELL_STOP);
	}
	else {
		if (!bStop) {
			Enemy::move(bLeft);
		}
	}

	// Check if Player collides with Koopa
	
	//Player* p = mario;
	glm::vec2 playerPos = mario.getPosition();
	glm::vec2 playerSize = mario.getSize();
	if (playerPos.x + playerSize.x > 50) {
		// Player dies
		bStop = false;
		sprite->changeAnimation(SHELL_MOVE);
	}

	sprite->setPosition(posEnemy);
}

void Enemy::render()
{
	sprite->render(bLeft);
}

void Enemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2& pos)
{
	sprite->setPosition(pos);
}

void Goomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	this->sizeEnemy = glm::ivec2(32, 32);
	spritesheet.loadFromFile("res/textures/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeEnemy, glm::vec2(0.0625f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 0, 0.0f));
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 1, 0.0f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.0625f * 2, 0.0f));

	sprite->changeAnimation(0);
	posEnemy = tileMapPos;
	sprite->setPosition(posEnemy);
}

void Koopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	this->sizeEnemy = glm::ivec2(32, 48);
	spritesheet.loadFromFile("res/textures/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeEnemy, glm::vec2(0.0625f, 0.1875f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 0, 0.125f));
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 1, 0.125f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.0625f * 2, 0.125f));

	sprite->setAnimationSpeed(SHELL_STOP, 8);
	sprite->addKeyframe(SHELL_STOP, glm::vec2(0.0625f * 2, 0.125f));

	sprite->setAnimationSpeed(SHELL_MOVE, 10);
	sprite->addKeyframe(SHELL_MOVE, glm::vec2(0.0625f * 3, 0.125f));
	sprite->addKeyframe(SHELL_MOVE, glm::vec2(0.0625f * 4, 0.125f));
	sprite->addKeyframe(SHELL_MOVE, glm::vec2(0.0625f * 5, 0.125f));

	sprite->changeAnimation(0);
	posEnemy = tileMapPos;
	sprite->setPosition(posEnemy);
}
