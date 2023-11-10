#include "Goomba.h"
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
	if (GameManager::instance().getMaxScrollX() + 50 < posEnemy.x || GameManager::instance().getMinScrollX() - 50 > posEnemy.x) return;
	sprite->update(deltaTime);

	if (bDying) {
		if (Player::instance().isDying()) bDying = false;
		else if (bSmashed) {
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

		if (posEnemy.y + sizeEnemy.y > SCREEN_HEIGHT - 5) {
			bDead = true;
		}
	}

	sprite->setPosition(posEnemy);
}

void Goomba::collisionDeath()
{
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

