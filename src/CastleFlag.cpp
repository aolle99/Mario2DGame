#include "CastleFlag.h"
#include "Player.h"
#include "GameManager.h"

enum ItemAnims
{
	MOVE
};

void CastleFlag::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Item::init(tileMapPos, shaderProgram);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 10);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 0, 0.125f * 1));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 1, 0.125f * 1));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 2, 0.125f * 1));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 3, 0.125f * 1));

	sprite->changeAnimation(0);
	posItem = tileMapPos;
	sprite->setPosition(posItem);
}

void CastleFlag::update(int deltaTime)
{
	if (GameManager::instance().isLevelCompleted()) bVisible = true;

	if (!bVisible) return;
	sprite->update(deltaTime);
}