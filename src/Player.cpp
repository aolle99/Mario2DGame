#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include <cmath>
#include "GameManager.h"
#include "SoundManager.h"

#define JUMP_ANGLE_STEP 4.f
#define FALL_STEP 8
#define MAX_WALK_SPEED 2
#define MIN_JUMP_INTERVAL 500
#define MAX_RUN_SPEED 4



enum PlayerAnims
{
	STAND, MOVE, JUMP, DIE, SHIFT, END, CHANGE_DIR
};

void Player::init(glm::vec2 &startPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	bLeft = false;
	bDying = false;
	bDead = false;
	bInvulnerable = false;
	bBounce = false;
	bShow = true;
	bCanMove = true;
	bChanging = false;
	bGrowing = false;
	bounceTime = 0;
	invTime = 0;
	star = 0.f;
	hp = 1;
	currentTime = 0;
	score = 0;
	posPlayer = startPos;
	animStep = 0;
	jumpTimer = 0;
	this->shaderProgram = shaderProgram;
	this->changeToMario();
	sprite->changeAnimation(0);
	sprite->setPosition(posPlayer);
	moveSpeed = 0;
	maxSpeed = MAX_WALK_SPEED;
}

void Player::changeToMario() {
	this->size = glm::vec2(32, 32);
	this->hitbox = glm::vec2(25, 32);

	posPlayer.y += 32;
	hp = 1;

	spritesheet.loadFromFile("res/textures/mario.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.0625f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(9);

	sprite->setAnimationSpeed(STAND, 0);
	sprite->addKeyframe(STAND, glm::vec2(0.0625f * 0, 0.75f));

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f, 0.75f));
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 2, 0.75f));

	sprite->setAnimationSpeed(JUMP, 3);
	sprite->addKeyframe(JUMP, glm::vec2(0.0625f * 3, 0.75f));
	sprite->addKeyframe(JUMP, glm::vec2(0.0625f * 5, 0.75f));

	sprite->setAnimationSpeed(SHIFT, 0);
	sprite->addKeyframe(SHIFT, glm::vec2(0.0625f * 0, 0.75f));

	sprite->setAnimationSpeed(DIE, 8);
	sprite->addKeyframe(DIE, glm::vec2(0.0625f * 6, 0.75f));

	sprite->setAnimationSpeed(END, 8);
	sprite->addKeyframe(END, glm::vec2(0.0625f * 9, 0.75f));

	sprite->setAnimationSpeed(CHANGE_DIR, 8);
	sprite->addKeyframe(CHANGE_DIR, glm::vec2(0.0625f * 4, 0.75f));
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

	sprite->setAnimationSpeed(END, 8);
	sprite->addKeyframe(END, glm::vec2(0.0625f * 9, 0.25f));

	sprite->setAnimationSpeed(CHANGE_DIR, 8);
	sprite->addKeyframe(CHANGE_DIR, glm::vec2(0.0625f * 4, 0.25f));

}

void Player::move(bool direction)
{
	if (!bJumping) {
		if (sprite->animation() != MOVE)
			sprite->changeAnimation(MOVE);
	}

	posPlayer.x += moveSpeed;
	if (map->collisionMoveRight(posPlayer, size, &posPlayer.x) || map->checkOutOfBoundsRight(&posPlayer.x))
	{
		moveSpeed = 0;
		sprite->changeAnimation(STAND);
	}
	if (map->collisionMoveLeft(posPlayer, size, &posPlayer.x) || map->checkOutOfBoundsLeft(&posPlayer.x))
	{
		moveSpeed = 0;
		sprite->changeAnimation(STAND);
	}
	if (direction) // right
	{
		bLeft = false;
		if (moveSpeed < maxSpeed) moveSpeed += 0.1;
		if(moveSpeed < 0) sprite->changeAnimation(CHANGE_DIR);
	}
	else // left
	{
		bLeft = true;
		if (moveSpeed > -maxSpeed) moveSpeed -= 0.1;
		if (moveSpeed > 0) sprite->changeAnimation(CHANGE_DIR);
	}



}

void Player::die()
{
	sprite->changeAnimation(DIE);
}

void Player::jump()
{
	if (bJumping || jumpTimer < MIN_JUMP_INTERVAL) return;
	bJumping = true;
	jumpAngle = 0;
	startY = posPlayer.y;
	sprite->changeAnimation(JUMP);
	jumpTimer = 0;
	SoundManager::instance().playSound("res/sounds/jump_super.wav");
}

void Player::bend()
{
	if (sprite->animation() != SHIFT) {
		sprite->changeAnimation(SHIFT);
	}
	
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
		else if (map->collisionMoveUp(getHitboxPosition(), hitbox, &posPlayer.y))
		{
			bFalling = true;
		}
		else
		{
			posPlayer.y = int(startY - 140 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(getHitboxPosition(), hitbox, &posPlayer.y);
		}
		return true;
	}
	else if (bJumping && bFalling)
	{
		posPlayer.y += FALL_STEP;
		sprite->changeAnimation(JUMP);
		if (map->collisionMoveDown(getHitboxPosition(), hitbox, &posPlayer.y))
		{
			bFalling = false;
			bJumping = false;
			jumpAngle = 0;
			sprite->changeAnimation(STAND);
		}
		return true;
	}
	else if(!bBounce) {
		posPlayer.y += FALL_STEP;
		if (!map->collisionMoveDown(getHitboxPosition(), hitbox, &posPlayer.y)) {
			sprite->changeAnimation(JUMP);
			bJumping = true;
			bFalling = true;
			return true;
		}
	}
	return false;
}

void Player::marioDying() {
	sprite->changeAnimation(DIE);

	if (currentTime <= 10) {
		posPlayer.y -= 4;
	}

	else if (currentTime > 10) {
		posPlayer.y += 4;

	}
	currentTime += 1;

	if ((!isSuperMario() && posPlayer.y > 518) || (isSuperMario() && posPlayer.y > 500)) {
		bDead = true;
		bDying = false;
		currentTime = 0;
	}
	
}

void Player::update(int deltaTime)
{
	if(bDead) return;

	sprite->update(deltaTime);
	if (bInvulnerable) {
		invTime -= 1;
		if (invTime == 0) {
			bInvulnerable = false;
		}
	}

	if (bDying) {
		this->marioDying();
	}
	else if (GameManager::instance().isLevelCompleted() || GameManager::instance().isPaused()) {
		if(GameManager::instance().isLevelCompleted()) this->animationEnd(deltaTime);
	}
	else {
		int textureChanged = 2;
		jumpTimer += deltaTime;

		if (bBounce) {
			if (bounceTime < 8) {
				if (!map->collisionMoveUp(posPlayer, size, &posPlayer.y)) {
					posPlayer.y -= FALL_STEP - bounceTime / 8.f * FALL_STEP;
					bounceTime += 1;
				}				
			}
			else if (bounceTime < 8){
				if (!map->collisionMoveDown(posPlayer, size, &posPlayer.y)) {
					posPlayer.y += FALL_STEP - (8 - bounceTime) / 8.f * FALL_STEP;
					bounceTime += 1;
				}
			}
			else {
				bounceTime = 0;
				bBounce = false;
			}
		}

		if (!this->checkJumping())  textureChanged -= 1;

		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) this->move(false);
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) this->move(true);
		else {
			if (moveSpeed > 1) {
				if (moveSpeed > MAX_WALK_SPEED)
				{
					moveSpeed -= 0.05;
				}
				moveSpeed -= 0.1;
			}
			else if (moveSpeed < -1) {
				if (moveSpeed < -MAX_WALK_SPEED)
				{
					moveSpeed += 0.05;
				}
				moveSpeed += 0.1;
			}
			else {
				moveSpeed = 0;
				textureChanged -= 1;
			}
			posPlayer.x += moveSpeed;
		}

		if (bCanMove) {
			if (Game::instance().getKey(32) || Game::instance().getSpecialKey(GLUT_KEY_UP)) this->jump();
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) this->bend();
			else if (Game::instance().getSpecialKey(112)) { //shift
				maxSpeed = MAX_RUN_SPEED;
				sprite->setAnimationSpeed(MOVE, 10);
			}
			else {
				maxSpeed = MAX_WALK_SPEED;
				sprite->setAnimationSpeed(MOVE, 7);
			}

			if (Game::instance().getKey('m') || Game::instance().getKey('M')) {
				this->giveMushroom();
			}
			if (Game::instance().getKey('g') || Game::instance().getKey('G')) {
				this->giveStar();
			}
		}

		if (star > 0) {
			// Restar 1 cada segundo
			star -= 0.01;
		}

		if (star < 0.5 && star != 0) {
			star = 0;
			SoundManager::instance().stopMusic();
			SoundManager::instance().playMusic("res/music/overworld_resumed.ogg");
		}

		if (map->checkOutOfBoundsDown(posPlayer.y)) {
			this->die();
		}

		if (bChanging) {
			this->animateTransformationMario(bGrowing);
		}

		if (!textureChanged) sprite->changeAnimation(STAND);
	}	
	
	if (!bDead && !bDying && !isSuperMario()){
		if (posPlayer.y + 32 >= 511 && posPlayer.y + 32 <= 513) {
			bDying = true;
			SoundManager::instance().stopMusic();
			SoundManager::instance().playSound("res/sounds/mario_die.wav");
		}
	}
	else if (!bDead && !bDying && isSuperMario()) {
		if (posPlayer.y + 64 >= 511 && posPlayer.y + 64 <= 513) {
			bDying = true;
			SoundManager::instance().stopMusic();
			SoundManager::instance().playSound("res/sounds/mario_die.wav");
			this->changeToMario();
		}
	}

	sprite->setPosition(posPlayer);
}

void Player::render()
{
	if (bShow)
	sprite->render(bLeft, star > 0);
}

void Player::animateTransformationMario(bool toSupermario)
{
	if (currentTime == 0 || currentTime == 6 || currentTime == 12 || currentTime == 18 || currentTime == 24) {
		if(bGrowing) this->changeToSuperMario();
		else this->changeToMario();
	}
	else if (currentTime == 3 || currentTime == 9 || currentTime == 15 || currentTime == 21) {
		if(bGrowing) this->changeToMario();
		else this->changeToSuperMario();
	}
	else if (currentTime == 27) {
		bChanging = false;
		currentTime = 0;
		bCanMove = true;
	}
	currentTime += 1;
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

bool Player::collision(const glm::ivec2& object_pos, const glm::ivec2& object_size)
{
	int px0 = posPlayer.x;
	int px1 = posPlayer.x + size.x;
	int py0 = posPlayer.y;
	int py1 = posPlayer.y + size.y;

	int ox0 = object_pos.x;
	int ox1 = object_pos.x + object_size.x;
	int oy0 = object_pos.y;
	int oy1 = object_pos.y + object_size.y;

	if (px0 < ox1 && px1 > ox0 && py0 < oy1 && py1 > oy0) {
		return true; // Colisi�n
	}

	return false; // No hay colisi�n
}

bool Player::collisionDown(const glm::ivec2& object_pos, const glm::ivec2& object_size, bool isItem)
{
	int player_bottom = posPlayer.y + size.y;
	int object_top = object_pos.y;

	if ((player_bottom == object_top || player_bottom == object_top + 1 || player_bottom == object_top + 2) &&
		posPlayer.x + size.x >= object_pos.x &&
		posPlayer.x <= object_pos.x + object_size.x) {
		if (!isItem) {
			bJumping = false;
			bounceTime = 0;
			startY = posPlayer.y + object_size.y;
			bBounce = true;
		}
		return true; // Colisi�n hacia abajo
	}

	return false; // No hay colisi�n hacia abajo
}

bool Player::collisionUp(const glm::ivec2& object_pos, const glm::ivec2& object_size)
{
	int player_top = posPlayer.y;
	int object_bottom = object_pos.y + object_size.y;

	if (player_top == object_bottom &&
		posPlayer.x + hitbox.x >= object_pos.x &&
		posPlayer.x <= object_pos.x + object_size.x) {
		return true; // Colisi�n hacia arriba
	}

	return false; // No hay colisi�n hacia arriba
}

bool Player::collisionLeft(const glm::ivec2& object_pos, const glm::ivec2& object_size)
{
	int player_left = posPlayer.x;
	int object_right = object_pos.x + object_size.x;

	if ((player_left == object_right || player_left == object_right - 1 || player_left == object_right - 2) &&
		posPlayer.y + hitbox.y >= object_pos.y &&
		posPlayer.y <= object_pos.y + object_size.y) {
		return true; // Colisi�n hacia la izquierda
	}

	return false; // No hay colisi�n hacia la izquierda
}

bool Player::collisionRight(const glm::ivec2& object_pos, const glm::ivec2& object_size)
{
	int player_right = posPlayer.x + size.x;
	int object_left = object_pos.x;

	if ((player_right == object_left || player_right == object_left+1 || player_right == object_left + 2) &&
		posPlayer.y + hitbox.y >= object_pos.y &&
		posPlayer.y <= object_pos.y + object_size.y) {
		return true; // Colisi�n hacia la derecha
	}

	return false; // No hay colisi�n hacia la derecha
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
		bDying = true;
		this->die();
		SoundManager::instance().stopMusic();
		SoundManager::instance().playSound("res/sounds/mario_die.wav");
	}
	else if (hp == 2) {
		bInvulnerable = true;
		invTime = 50;
		bChanging = true;
		bCanMove = false;
		bGrowing = false;
		SoundManager::instance().playSound("res/Sounds/powerup.wav");
	}
}

void Player::giveStar() {
	SoundManager::instance().stopMusic();
	SoundManager::instance().playMusic("res/music/invincibility.ogg");
	star = 5;
}

void Player::giveMushroom() {
	if (hp == 1) {
		bChanging = true;
		bCanMove = false;
		bGrowing = true;
	}
}

bool Player::isMarioStar() {
	return star > 0;
}

bool Player::isSuperMario()
{
	return hp == 2;
}

bool Player::isInvulnerable()
{
	return bInvulnerable;
}

bool Player::isDead()
{
	return bDead;
}

bool Player::isDying()
{
	return bDying;
}

int Player::getHp() {
	return hp;
}

void Player::setDying(bool dying)
{
	bDying = dying;
}

void Player::setInvulnerable(bool invulnerable)
{
	bInvulnerable = invulnerable;
}

void Player::setInvTime(int time)
{
	invTime = time;
}

void Player::setEndPos(glm::ivec2 pos) {
	endLevelPos = pos;
}

void Player::removeCollisionBlock(int x, int y) {
		map->removeCollisionBlock(x, y);
}

void Player::resetCurrentTime()
{
		currentTime = 0;
}


glm::ivec2 Player::getHitboxPosition() {
	return posPlayer + glm::ivec2(4, 0);
}


void Player::animationEnd(int deltaTime)
{
	currentTime += deltaTime;
	if (animStep==0) {
		sprite->changeAnimation(END);
		if (!map->collisionMoveDown(posPlayer, size, &posPlayer.y))
		{
			posPlayer.y += 2;
		}
		else {
			animStep = 1;
			sprite->changeAnimation(MOVE);
		}
	}
	else if (animStep==1) {
		
		bLeft = false;
		if (posPlayer.x < endLevelPos.x) {
			posPlayer.x += 1;
			posPlayer.y += 4;
			if (!map->collisionMoveDown(posPlayer, size, &posPlayer.y))
			{}
		}
		else {
			sprite->changeAnimation(STAND);
			bShow=false;
			if(currentTime > 6000)
				Game::instance().nextLevel();
		}
	}
}

