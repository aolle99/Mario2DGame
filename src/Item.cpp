#include "Item.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Player.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "SoundManager.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 150
#define FALL_STEP 4
#define PUNCT_COIN 200
#define PUNCT_MUSHROOM 1000
#define PUNCT_STAR 1500

enum ItemAnims
{
	MOVE, DIE, STOP
};

void Item::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bLeft = false;
	bVisible = false;
	bUsed = false;
	currentTime = 0;
	sizeItem = glm::ivec2(32, 32);
	
	spritesheet.loadFromFile("res/textures/items.png", TEXTURE_PIXEL_FORMAT_RGBA);

	posItem = tileMapPos;

}

void Item::update(int deltaTime)
{
	if(!bVisible) return;
	sprite->update(deltaTime);
	currentTime += deltaTime;
	this->move(bLeft);
	sprite->setPosition(posItem);
}

void Item::render()
{
	if (!bVisible) return;
	sprite->render();
}

void Item::move(bool direction)
{
	posItem.y += FALL_STEP;
	if (!map->collisionMoveDown(posItem, sizeItem, &posItem.y))
		return;

	if (direction) { // left
		posItem.x += 2;
		if (map->collisionMoveRight(posItem, sizeItem))
		{
			bLeft = !bLeft;
		}

	}
	else { // right
		posItem.x -= 2;
		if (map->collisionMoveLeft(posItem, sizeItem)) // mira si hi ha colisio a l'esquerra
		{
			bLeft = !bLeft;
		}
	}
}

void Item::die()
{
	sprite->changeAnimation(DIE);
}

bool Item::isVisible()
{
	return bVisible;
}

bool Item::isUsed()
{
	return bUsed;
}

void Item::show()
{
	bVisible = true;
}

bool Item::isCoin()
{
	return false;
}

void Item::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Item::setPosition(const glm::vec2& pos)
{
	sprite->setPosition(pos);
}

void Mushroom::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Item::init(tileMapPos, shaderProgram);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 4, 0.125f * 1));

	sprite->changeAnimation(0);
	sprite->setPosition(posItem);

}

void Mushroom::update(int deltaTime)
{
	if (!bVisible) return;
	Item::update(deltaTime);
	if (Player::instance().collisionRight(posItem, sizeItem) || Player::instance().collisionLeft(posItem, sizeItem) || Player::instance().collisionDown(posItem, sizeItem, true) || Player::instance().collisionUp(posItem, sizeItem)) {
		Player::instance().giveMushroom();
		GameManager::instance().addScore(PUNCT_MUSHROOM);
		PunctuationDisplay::instance().addDisplay(to_string(PUNCT_MUSHROOM), Player::instance().getPosition());
		SoundManager::instance().playSound("res/Sounds/powerup.wav");
		bVisible = false;
		bUsed = true;
	}
}

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
	posItem = tileMapPos - glm::ivec2(14,0);
	sprite->setPosition(posItem);
}

void Flag::update(int deltaTime)
{
	if (!bVisible) return;
	sprite->update(deltaTime);
	if (GameManager::instance().isLevelEnd() && posItem.y < minCoords.y) {
		posItem = glm::vec2(posItem.x, posItem.y + 4);
		sprite->setPosition(posItem);
	}
}

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
	if(GameManager::instance().isLevelEnd()) bVisible = true;
	
	if (!bVisible) return;
	sprite->update(deltaTime);
}

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
	glm::ivec2 pos = posItem + glm::ivec2(14,0);
	glm::ivec2 size = sizeItem - glm::ivec2(28,0);
	if (!GameManager::instance().isLevelEnd() && Player::instance().collision(pos, size)) {
		GameManager::instance().addScore((multiplier+1) * 10);
		PunctuationDisplay::instance().addDisplay(to_string((multiplier + 1) * 10), pos);
		GameManager::instance().setLevelEnd(true);
		SoundManager::instance().stopMusic();
		SoundManager::instance().playSound("res/Sounds/flagpole.wav");
		SoundManager::instance().playSound("res/Sounds/fireworks.wav");
		SoundManager::instance().playSound("res/Sounds/world_clear.wav");
	}
}

