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
	MOVE, DIE, STOP
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

void Enemy::die()
{
	sprite->changeAnimation(DIE);
}


void Enemy::update(int deltaTime)
{
	sprite->update(deltaTime);
	currentTime += deltaTime;
	
	if (bDying) {
		bDead = true;
		sprite->changeAnimation(DIE);

		if (currentTime > 10) {
			posEnemy.y += 1;
			currentTime = 0;
		}
	}
	else
	{
		this->move(bLeft);

		if (!bDead && !mario->isInvulnerable()) {
			if (mario->collisionRight(posEnemy, sizeEnemy) || mario->collisionLeft(posEnemy, sizeEnemy)) {
				if (mario->isMarioStar()) {
					this->die();
					bDying = true;
				}
				else {
					mario->damagePlayer();
				}
			}
			else if (mario->collisionDown(posEnemy, sizeEnemy)) {
				this->die();
				bDying = true;
			}
		}
	}
	

	sprite->setPosition(posEnemy);
}

void Enemy::setPlayer(Player* player)
{
	this->mario = player;
}

void Koopa::update(int deltaTime)
{
	sprite->update(deltaTime);	
	
	if (bDying) { // Koopa morint
		bDead = true;
		sprite->changeAnimation(DIE);

		if (currentTime > 10) {
			posEnemy.y += 1;
			currentTime = 0;
		}
	}
	else { 
		if (!bShell) { // Koopa en modo tortuga
			move(bLeft);

			if (!bDead && !mario->isInvulnerable()) {
				if (mario->collisionRight(posEnemy, sizeEnemy) || mario->collisionLeft(posEnemy, sizeEnemy)) {
					if (mario->isMarioStar()) {
						this->die();
						bDying = true;
					}
					else {
						mario->damagePlayer();
					}
				}
				else if (mario->collisionDown(posEnemy, sizeEnemy)) {
					change_to_shell();
					bShell = true;
					bStop = true;
					sprite->changeAnimation(STOP);
					// mario has to bounce from the shell
					mario->jump();
				}
			}
		}
		
		else { // Koopa en modo caparazon
			
			if (!bStop) { // Koopa en moviment
				this->move(bLeft);
				
				if (mario->collisionLeft(posEnemy, sizeEnemy) || mario->collisionRight(posEnemy, sizeEnemy)) {
					if (mario->isMarioStar()) {
						this->die();
						bDying = true;
					}
					else {
						mario->damagePlayer();
					}
				}
				printf("Koopa en moviment\n");
				if (mario->collisionDown(posEnemy, sizeEnemy)) {
					bStop = true;
					sprite->changeAnimation(STOP);
					return;
				}
			}
			if (bStop) { // Koopa parat
				printf("Koopa parat\n");
				
				if (mario->collisionLeft(posEnemy, sizeEnemy) || mario->collisionRight(posEnemy, sizeEnemy) || mario->collisionDown(posEnemy, sizeEnemy)) {
					bStop = false;
					sprite->changeAnimation(MOVE);
				}

			}
			

		}
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
	bDying = false;
	bDead = false;
	currentTime = 0;
	sizeEnemy = glm::ivec2(32, 32);

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
	bShell = false;
	bStop = false;
	bDying = false;
	bDead = false;
	this->shaderProgram = shaderProgram;
	this->change_to_turtle();

	sprite->changeAnimation(0);
	posEnemy = tileMapPos;
	sprite->setPosition(posEnemy);
}

void Koopa::change_to_shell() {
	sizeEnemy = glm::vec2(32, 32);
	posEnemy.y += 16;

	spritesheet.loadFromFile("res/textures/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeEnemy, glm::vec2(0.0625f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(STOP, 8);
	sprite->addKeyframe(STOP, glm::vec2(0.0625f * 2, 0.1875f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.0625f * 2, 0.1875f));

	sprite->setAnimationSpeed(MOVE, 10);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 3, 0.1875f));
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 4, 0.1875f));
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 5, 0.1875f));

}

void Koopa::change_to_turtle() {
	sizeEnemy = glm::vec2(32, 48);
	posEnemy.y -= 16;

	spritesheet.loadFromFile("res/textures/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeEnemy, glm::vec2(0.0625f, 0.1875f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 0, 0.125f));
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 1, 0.125f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.0625f * 6, 0.125f));
}
