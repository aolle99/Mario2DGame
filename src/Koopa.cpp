#include "Koopa.h"
#include "Player.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "SoundManager.h"
#include "Game.h"


#define PUNCT_ENEMY 100

enum EnemyAnims
{
	MOVE, DIE, STOP
};


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
	if (GameManager::instance().getMaxScrollX() + 200 < posEnemy.x || GameManager::instance().getMinScrollX() - 50 > posEnemy.x) return;
	sprite->update(deltaTime);

	if (bDying) { // Koopa morint
		this->collisionDeath();
	}
	else {
		if (!bShell) { // Koopa en modo tortuga
			this->turtleMode();
		}

		else { // Koopa en modo caparazon
			this->shellMode();

			if (posEnemy.y + sizeEnemy.y > SCREEN_HEIGHT - 5) {
				bDead = true;
			}
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
		else if (Player::instance().collisionDown(posEnemy, sizeEnemy, false) && !Player::instance().isDying()) {
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
	if (bShell && !bStop) return true;
	return false;
}

bool Koopa::isModeTurtle()
{
	if (!bShell) return true;
	return false;
}
