#include "Star.h"
#include "Player.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"

#define PUNCT_STAR 1500

enum ItemAnims
{
	MOVE, DIE, STOP
};


void Star::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Item::init(tileMapPos, shaderProgram);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.1172f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(MOVE, 5);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 4, 0.125f * 3));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 5, 0.125f * 3));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 6, 0.125f * 3));

	sprite->changeAnimation(0);
	sprite->setPosition(posItem);
}

void Star::update(int deltaTime)
{
	if (!bVisible) return;
	Item::update(deltaTime);

	if (Player::instance().collisionRight(posItem, sizeItem) || Player::instance().collisionLeft(posItem, sizeItem) || Player::instance().collisionDown(posItem, sizeItem, true) || Player::instance().collisionUp(posItem, sizeItem)) {
		Player::instance().giveStar();
		GameManager::instance().addScore(PUNCT_STAR);
		PunctuationDisplay::instance().addDisplay(to_string(PUNCT_STAR), Player::instance().getPosition());
		bVisible = false;
		bUsed = true;
	}
}