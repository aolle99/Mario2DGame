#include "Enemy.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Player.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "SoundManager.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 150
#define FALL_STEP 4
#define PUNCT_ENEMY 100

enum EnemyAnims
{
	MOVE, DIE, STOP
};


void Enemy::update(int deltaTime)
{
	
}

void Enemy::render()
{
	sprite->render(bLeft);
}

void Enemy::move()
{
	posEnemy.y += FALL_STEP;
	
	
		if (map->collisionMoveDown(posEnemy, sizeEnemy, &posEnemy.y)) {
			if (this->isModeTurtle()) {
				if (map->nearFall(posEnemy, sizeEnemy, bLeft)) {
					bLeft = !bLeft;
				}
			}
		}
		else {
			return;
		}
		

	if (bLeft) { // left
		posEnemy.x += speed;
		if (map->collisionMoveRight(posEnemy, sizeEnemy, &posEnemy.x))
		{
			bLeft = !bLeft;
		}

	}
	else { // right
		posEnemy.x -= speed;
		if (map->collisionMoveLeft(posEnemy, sizeEnemy, &posEnemy.x)) // mira si hi ha colisio a l'esquerra
		{
			bLeft = !bLeft;
		}
	}
}

void Enemy::die()
{
	sprite->changeAnimation(DIE);
}

void Enemy::collisionDeath()
{
	
}

void Enemy::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2& pos)
{
	sprite->setPosition(pos);
}

void Enemy::changeDirection()
{
	bLeft = !bLeft;
}

bool Enemy::isDead()
{
	return bDead;
}

bool Enemy::isKoopaShellMove()
{
	return false;
}

bool Enemy::isModeTurtle()
{
	return false;
}

bool Enemy::collisionEnemies(const glm::vec2& pos2, const glm::ivec2& size2)
{
	if ((posEnemy.x + sizeEnemy.x >= pos2.x && posEnemy.x <= pos2.x + size2.x) && posEnemy.y + sizeEnemy.y >= pos2.y && posEnemy.y <= pos2.y + size2.y) {
		return true;
	}
	return false;
}

glm::ivec2 Enemy::getPosition()
{
	return posEnemy;
}

glm::ivec2 Enemy::getSize()
{
	return sizeEnemy;
}

