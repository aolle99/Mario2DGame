#include "Item.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Player.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 150
#define FALL_STEP 4

enum ItemAnims
{
	MOVE, DIE, STOP
};

void Item::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bLeft = false;
	bVisible = false;
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

void Item::show()
{
	bVisible = true;
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
		bVisible = false;	
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
		bVisible = false;
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
	if (Player::instance().collisionRight(posItem, sizeItem) || Player::instance().collisionLeft(posItem, sizeItem) || Player::instance().collisionDown(posItem, sizeItem, true) || Player::instance().collisionUp(posItem, sizeItem)) {
		Player::instance().giveCoin();
		bVisible = false;
	}

}

void Flag::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{

	Item::init(tileMapPos, shaderProgram);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 10);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 0, 0.125f * 2));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 1, 0.125f * 2));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 2, 0.125f * 2));
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 3, 0.125f * 2));

	sprite->changeAnimation(0);
	posItem = tileMapPos - glm::ivec2(0,16);
	sprite->setPosition(posItem);
}