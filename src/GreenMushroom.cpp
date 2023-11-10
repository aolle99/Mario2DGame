#include "GreenMushroom.h"
#include "Player.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "SoundManager.h"

#define PUNCT_MUSHROOM 1000

enum ItemAnims
{
	MOVE, DIE, STOP
};

void GreenMushroom::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Item::init(tileMapPos, shaderProgram);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 5, 0.125f * 1));

	sprite->changeAnimation(0);
	sprite->setPosition(posItem);

}

void GreenMushroom::update(int deltaTime)
{
	if (!bVisible) return;
	Item::update(deltaTime);
	if (Player::instance().collisionRight(posItem, sizeItem) || Player::instance().collisionLeft(posItem, sizeItem) || Player::instance().collisionDown(posItem, sizeItem, true) || Player::instance().collisionUp(posItem, sizeItem)) {
		GameManager::instance().addLive();
		GameManager::instance().addScore(PUNCT_MUSHROOM);
		PunctuationDisplay::instance().addDisplay(to_string(PUNCT_MUSHROOM), Player::instance().getPosition());
		SoundManager::instance().playSound("res/Sounds/1-up.wav");
		bVisible = false;
		bUsed = true;
	}
}