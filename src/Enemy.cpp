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
	// if collision between enemies	return true
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

void Goomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bDying = false;
	bDead = false;
	bSmashed = false;
	bTextRendered = false;
	currentTime = 0;
	speed = 2;
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

void Goomba::update(int deltaTime)
{
	if(GameManager::instance().getMaxScrollX() + 50 < posEnemy.x || GameManager::instance().getMinScrollX() -50 > posEnemy.x) return;
	sprite->update(deltaTime);

	if (bDying) {
		if (bSmashed) {
			this->smashedDeath();
		}
		else {
			this->collisionDeath();
		}
	}
	else
	{
		this->move();

		if (!bDead && !Player::instance().isInvulnerable()) {
			if (Player::instance().collisionRight(posEnemy, sizeEnemy) || Player::instance().collisionLeft(posEnemy, sizeEnemy)) {
				if (Player::instance().isMarioStar()) {
					bDying = true;
					SoundManager::instance().playSound("res/sounds/kick.wav");
				}
				else {
					Player::instance().damagePlayer();
				}
			}
			else if (Player::instance().collisionDown(posEnemy, sizeEnemy, false)) {
				bSmashed = true;
				bDying = true;
			}
		}
	}

	sprite->setPosition(posEnemy);
}

void Goomba::collisionDeath()
{
	bDying = true;

	if (!bTextRendered) {
		PunctuationDisplay::instance().addDisplay(to_string(PUNCT_ENEMY), posEnemy);
		GameManager::instance().addScore(PUNCT_ENEMY);
		bTextRendered = true;
	}

	if (currentTime <= 10) {
		posEnemy.y -= 2;
	}

	if (currentTime > 10) {
		posEnemy.y += 2;
	}

	if (posEnemy.y > 518) {
		bDead = true;
		bDying = false;
		currentTime = 0;
	}
	currentTime += 1;
}

void Goomba::smashedDeath()
{
	sprite->changeAnimation(DIE);
	
	currentTime += 1;
	if (!bTextRendered) {
		PunctuationDisplay::instance().addDisplay(to_string(PUNCT_ENEMY), posEnemy);
		GameManager::instance().addScore(PUNCT_ENEMY);
		bTextRendered = true;
		SoundManager::instance().playSound("res/sounds/stomp.wav");
	}
	if (currentTime == 20) {
		bDead = true;
		currentTime = 0;
	}
}

void Koopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bShell = false;
	bStop = false;
	bDying = false;
	bDead = false;
	bTextRendered = false;
	speed = 2;
	currentTime = 0;
	this->shaderProgram = shaderProgram;
	this->change_to_turtle();

	sprite->changeAnimation(0);
	posEnemy = tileMapPos;
	sprite->setPosition(posEnemy);
}

void Koopa::update(int deltaTime)
{
	if (GameManager::instance().getMaxScrollX() + 50 < posEnemy.x || GameManager::instance().getMinScrollX() - 50 > posEnemy.x) return;
	sprite->update(deltaTime);

	if (bDying) { // Koopa morint
		collisionDeath();
	}
	else {
		if (!bShell) { // Koopa en modo tortuga
			this->turtleMode();
		}

		else { // Koopa en modo caparazon

			this->shellMode();
		}
	}

	sprite->setPosition(posEnemy);
}

void Koopa::shellMode() {
	if (!bStop) { // Koopa en moviment
		this->move();

		if (Player::instance().collisionLeft(posEnemy, sizeEnemy) || Player::instance().collisionRight(posEnemy, sizeEnemy)) {
			if (Player::instance().isMarioStar()) {
				bDying = true;
				SoundManager::instance().playSound("res/sounds/kick.wav");
			}
			else if (!Player::instance().isInvulnerable()) {
				Player::instance().damagePlayer();
			}
		}

		if (Player::instance().collisionDown(posEnemy, sizeEnemy, false)) {
			bStop = true;
			sprite->changeAnimation(STOP);
			SoundManager::instance().playSound("res/sounds/kick.wav");
			return;
		}
	}
	if (bStop) { // Koopa parat

		if (Player::instance().collisionLeft(posEnemy, sizeEnemy) || Player::instance().collisionRight(posEnemy, sizeEnemy) || Player::instance().collisionDown(posEnemy, sizeEnemy, false)) {
			bStop = false;
			sprite->changeAnimation(MOVE);
			Player::instance().setInvulnerable(true);
			Player::instance().setInvTime(50);
			SoundManager::instance().playSound("res/sounds/kick.wav");
		}

	}
}

void Koopa::turtleMode()
{
	move();

	if (!bDead && !Player::instance().isInvulnerable()) {
		if (Player::instance().collisionRight(posEnemy, sizeEnemy) || Player::instance().collisionLeft(posEnemy, sizeEnemy)) {
			if (Player::instance().isMarioStar()) {
				bDying = true;
				SoundManager::instance().playSound("res/sounds/kick.wav");
			}
			else {
				Player::instance().damagePlayer();
			}
		}
		else if (Player::instance().collisionDown(posEnemy, sizeEnemy, false)) {
			change_to_shell();
			bShell = true;
			bStop = true;
			sprite->changeAnimation(STOP);
			SoundManager::instance().playSound("res/sounds/kick.wav");
		}
	}
}

void Koopa::collisionDeath()
{
	sprite->changeAnimation(DIE);
	bStop = true;
	bDying = true;

	if (!bTextRendered) {
		PunctuationDisplay::instance().addDisplay(to_string(PUNCT_ENEMY), posEnemy);
		GameManager::instance().addScore(PUNCT_ENEMY);
		bTextRendered = true;
	}

	if (currentTime <= 10) {
		posEnemy.y -= 2;
	}

	if (currentTime > 10) {
		posEnemy.y += 2;
	}

	if (posEnemy.y > 518) {
		bDead = true;
		bDying = false;
		currentTime = 0;
	}
	currentTime += 1;
}

void Koopa::change_to_shell() {
	sizeEnemy = glm::vec2(32, 32);
	posEnemy.y += 16;
	speed = 4;

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
	speed = 2;

	spritesheet.loadFromFile("res/textures/enemies.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeEnemy, glm::vec2(0.0625f, 0.1875f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 0, 0.125f));
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 1, 0.125f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.0625f * 2, 0.125f));
}

bool Koopa::isKoopaShellMove()
{
	if(bShell && !bStop) return true;
	return false;
}

bool Koopa::isModeTurtle()
{
	if (!bShell) return true;
	return false;
}
