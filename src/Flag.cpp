#include "Flag.h"
#include "Player.h"
#include "GameManager.h"

enum ItemAnims
{
	MOVE
};

void Flag::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, glm::ivec2 minCoords)
{
	int x = minCoords.x * 32;
	int y = minCoords.y * 32;
	this->minCoords = glm::ivec2(x, y);
	Item::init(tileMapPos, shaderProgram);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 10);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 0, 0.125f * 2));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 1, 0.125f * 2));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 2, 0.125f * 2));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 3, 0.125f * 2));

	sprite->changeAnimation(0);
	posItem = tileMapPos - glm::ivec2(14, 0);
	sprite->setPosition(posItem);
}

void Flag::update(int deltaTime)
{
	if (!bVisible) return;
	sprite->update(deltaTime);
	if (GameManager::instance().isLevelCompleted() && posItem.y < minCoords.y) {
		posItem = glm::vec2(posItem.x, posItem.y + 4);
		sprite->setPosition(posItem);
	}
}
