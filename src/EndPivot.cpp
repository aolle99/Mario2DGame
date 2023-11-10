#include "EndPivot.h"
#include "Player.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "SoundManager.h"

enum ItemAnims
{
	MOVE
};

void EndPivot::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int multiplier)
{
	this->multiplier = multiplier;
	Item::init(tileMapPos, shaderProgram);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(MOVE, 0);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 7, 0.125f * 1));

	sprite->changeAnimation(0);
	posItem = tileMapPos;
	sprite->setPosition(posItem);
}

void EndPivot::update(int deltaTime)
{
	if (!bVisible) return;
	glm::ivec2 pos = posItem + glm::ivec2(14, 0);
	glm::ivec2 size = sizeItem - glm::ivec2(28, 0);
	if (!GameManager::instance().isLevelCompleted() && Player::instance().collision(pos, size)) {
		GameManager::instance().addScore((multiplier + 1) * 10);
		PunctuationDisplay::instance().addDisplay(to_string((multiplier + 1) * 10), pos);
		GameManager::instance().setLevelCompleted(true);
		SoundManager::instance().stopMusic();
		SoundManager::instance().playSound("res/Sounds/flagpole.wav");
		SoundManager::instance().playSound("res/Sounds/fireworks.wav");
		SoundManager::instance().playSound("res/Sounds/world_clear.wav");
		Player::instance().resetCurrentTime();
	}
}