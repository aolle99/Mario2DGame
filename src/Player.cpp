#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define FALL_STEP 4


enum PlayerAnims
{
	STAND, MOVE, JUMP, DIE, FIRE, SHIFT
};

void Player::init(glm::vec2 &startPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	bLeft = false;
	bDying = false;
	bDead = false;
	speed = 2;
	star = 0.f;
	hp = 1;
	currentTime = 0;
	posPlayer = startPos;
	this->shaderProgram = shaderProgram;
	this->changeToMario();
	sprite->changeAnimation(0);
	sprite->setPosition(posPlayer);
}

void Player::changeToMario() {
	this->size = glm::vec2(32, 32);
	this->hitbox = glm::vec2(25, 32);

	posPlayer.y += 32;
	hp = 1;

	spritesheet.loadFromFile("res/textures/mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(STAND, 0);
	sprite->addKeyframe(STAND, glm::vec2(0.0625f * 0, 0.75f));

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f, 0.75f));
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 2, 0.75f));

	sprite->setAnimationSpeed(JUMP, 3);
	sprite->addKeyframe(JUMP, glm::vec2(0.0625f * 3, 0.75f));
	sprite->addKeyframe(JUMP, glm::vec2(0.0625f * 5, 0.75f));

	sprite->setAnimationSpeed(SHIFT, 0);
	sprite->addKeyframe(SHIFT, glm::vec2(0.0625f * 6, 0.75f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.0625f * 6, 0.75f));
}

void Player::changeToSuperMario() {
	this->size = glm::vec2(32, 64);
	this->hitbox = glm::vec2(25, 64);
	posPlayer.y -= 32;
	hp = 2;
	spritesheet.loadFromFile("res/textures/mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.0625f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(8);

	sprite->setAnimationSpeed(STAND, 0);
	sprite->addKeyframe(STAND, glm::vec2(0.0625f * 0, 0.25f));

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f, 0.25f));
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 2, 0.25f));

	sprite->setAnimationSpeed(JUMP, 3);
	sprite->addKeyframe(JUMP, glm::vec2(0.0625f * 3, 0.25f));
	sprite->addKeyframe(JUMP, glm::vec2(0.0625f * 5, 0.25f));

	sprite->setAnimationSpeed(SHIFT, 0);
	sprite->addKeyframe(SHIFT, glm::vec2(0.0625f * 6, 0.25f));

	sprite->setAnimationSpeed(FIRE, 8);
	sprite->addKeyframe(FIRE, glm::vec2(0.0625f * 5, 0.25f));
	sprite->addKeyframe(FIRE, glm::vec2(0.0625f * 7, 0.25f));
}

void Player::move(bool direction)
{
	if (!bJumping) {
		if (sprite->animation() != MOVE)
			sprite->changeAnimation(MOVE);
	}

	if (direction) // right
	{
		bLeft = false;
		if (map->collisionMoveRight(posPlayer, size))
		{
			sprite->changeAnimation(STAND);
		}
		else {
			posPlayer.x += speed;
		}
	}
	else // left
	{
		bLeft = true;
		if (map->collisionMoveLeft(posPlayer, size))
		{
			sprite->changeAnimation(STAND);
		}
		else {
			posPlayer.x -= speed;
		}
	}

}

void Player::die()
{
	sprite->changeAnimation(DIE);
}

void Player::jump()
{
	if (bJumping) return;
	bJumping = true;
	jumpAngle = 0;
	startY = posPlayer.y;
	sprite->changeAnimation(JUMP);
}

void Player::bend()
{
	speed = 1;
	if (sprite->animation() != SHIFT) {
		sprite->changeAnimation(SHIFT);
	}
	
}

void Player::run() {
	speed = 4;
	sprite->setAnimationSpeed(MOVE, 10);
}

bool Player::checkJumping()
{
	
	if (bJumping && !bFalling)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bFalling = true;
			posPlayer.y = startY;
		}
		else if (map->collisionMoveUp(posPlayer, hitbox))
		{
			bFalling = true;
		}
		else
		{
			posPlayer.y = int(startY - 140 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, hitbox, &posPlayer.y);
		}
		return true;
	}
	else if (bJumping && bFalling)
	{
		posPlayer.y += FALL_STEP;
		sprite->changeAnimation(JUMP);
		if (map->collisionMoveDown(posPlayer, hitbox, &posPlayer.y))
		{
			bFalling = false;
			bJumping = false;
			jumpAngle = 0;
			sprite->changeAnimation(STAND);
		}
		return true;
	}
	else {
		posPlayer.y += FALL_STEP;
		if (!map->collisionMoveDown(posPlayer, hitbox, &posPlayer.y)) {
			sprite->changeAnimation(JUMP);
			bJumping = true;
			bFalling = true;
			return true;
		}
	}
	return false;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	currentTime += deltaTime;

	if (bDying) {
		sprite->changeAnimation(DIE);

		if (currentTime > 10) {
			posPlayer.y += 1;
			currentTime = 0;
		}
		
	}
	else {
		int textureChanged = 3;
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) this->move(false);
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) this->move(true);
		else textureChanged -= 1;

		if (Game::instance().getSpecialKey(GLUT_KEY_UP)) this->jump();
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) this->bend();
		else if (Game::instance().getKey('z')) this->run();
		else {
			textureChanged -= 1;
			speed = 2;
			sprite->setAnimationSpeed(MOVE, 7);
		}

		if (Game::instance().getKey('g')) {
			this->giveStar();
		}

		if (!this->checkJumping())  textureChanged -= 1;
		if (star > 0) {
			// Restar 1 cada segundo
			star -= 0.01;
		}

		if (Game::instance().getKey('m')) {
			this->giveMushroom();
		}

		if (!textureChanged) sprite->changeAnimation(STAND);
	}	

	sprite->setPosition(posPlayer);
}

void Player::render()
{
	sprite->render(bLeft, star > 0);
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(posPlayer);
}

bool Player::collisionDown(const glm::ivec2& object_pos, const glm::ivec2& object_size)
{
	int player_bottom = posPlayer.y + size.y;
	int object_top = object_pos.y;

	if (player_bottom >= object_top &&
		posPlayer.x + size.x >= object_pos.x &&
		posPlayer.x <= object_pos.x + object_size.x) {
		return true; // Colisión hacia abajo
	}

	return false; // No hay colisión hacia abajo
}

bool Player::collisionUp(const glm::ivec2& object_pos, const glm::ivec2& object_size)
{
	int player_top = posPlayer.y;
	int object_bottom = object_pos.y + object_size.y;

	if (player_top <= object_bottom &&
		posPlayer.x + hitbox.x >= object_pos.x &&
		posPlayer.x <= object_pos.x + object_size.x) {
		return true; // Colisión hacia arriba
	}

	return false; // No hay colisión hacia arriba
}

bool Player::collisionLeft(const glm::ivec2& object_pos, const glm::ivec2& object_size)
{
	int player_left = posPlayer.x;
	int object_right = object_pos.x + object_size.x;

	if (player_left == object_right &&
		posPlayer.y + hitbox.y >= object_pos.y &&
		posPlayer.y <= object_pos.y + object_size.y) {
		return true; // Colisión hacia la izquierda
	}

	return false; // No hay colisión hacia la izquierda
}

bool Player::collisionRight(const glm::ivec2& object_pos, const glm::ivec2& object_size)
{
	int player_right = posPlayer.x + size.x;
	int object_left = object_pos.x;

	if (player_right == object_left &&
		posPlayer.y + hitbox.y >= object_pos.y &&
		posPlayer.y <= object_pos.y + object_size.y) {
		return true; // Colisión hacia la derecha
	}

	return false; // No hay colisión hacia la derecha
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

glm::ivec2 Player::getSize() {
	return size;
}

void Player::damagePlayer() {
	if (star > 0) return;
	if (hp == 1) {
		hp = 0;
		this->die();
	}
	else if (hp == 2) {
		changeToMario();
	}
}

void Player::giveStar() {
	star = 10;
}

void Player::giveMushroom() {
	if (hp == 1) {
		changeToSuperMario();
	}
}

bool Player::isMarioStar() {
	return star > 0;
}

int Player::getHp() {
	return hp;
}

void Player::setDying(bool dying)
{
	bDying = dying;
}




