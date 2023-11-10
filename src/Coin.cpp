#include "Coin.h"
#include "Player.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "SoundManager.h"


#define PUNCT_COIN 200

enum ItemAnims
{
	MOVE, DIE, STOP
};

void Coin::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Item::init(tileMapPos, shaderProgram);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 8);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 0, 0.125f * 0));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 1, 0.125f * 0));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 2, 0.125f * 0));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 3, 0.125f * 0));

	sprite->changeAnimation(0);
	sprite->setPosition(posItem);
}

void Coin::update(int deltaTime)
{
	if (!bVisible) return;
	sprite->update(deltaTime);
	if (Player::instance().collision(posItem, sizeItem)) {
		GameManager::instance().addCoin();
		GameManager::instance().addScore(PUNCT_COIN);
		SoundManager::instance().playSound("res/Sounds/coin.wav");
		bVisible = false;
		bUsed = true;
	}
}

bool Coin::isCoin()
{
	return true;
}