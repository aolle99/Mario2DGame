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
	bVisible = true;
	currentTime = 0;
	sizeItem = glm::ivec2(32, 32);
	
	spritesheet.loadFromFile("res/textures/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 4, 0.5859f)); 

	sprite->changeAnimation(0);
	
	posItem = tileMapPos;
	sprite->setPosition(posItem);

}

void Item::update(int deltaTime)
{
	sprite->update(deltaTime);
	currentTime += deltaTime;
	this->move(bLeft);

	if (Player::instance().collisionRight(posItem, sizeItem) || Player::instance().collisionLeft(posItem, sizeItem) || Player::instance().collisionDown(posItem, sizeItem, true) || Player::instance().collisionUp(posItem, sizeItem)) {
		bVisible = false;
	}

	sprite->setPosition(posItem);
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
void Mushroom::update(int deltaTime)
{
	sprite->update(deltaTime);
	currentTime += deltaTime;
	this->move(bLeft);

	if (Player::instance().collisionRight(posItem, sizeItem) || Player::instance().collisionLeft(posItem, sizeItem) || Player::instance().collisionDown(posItem, sizeItem, true) || Player::instance().collisionUp(posItem, sizeItem)) {
		Player::instance().giveMushroom();
		bVisible = false;	
	}

	sprite->setPosition(posItem);
}

void Star::update(int deltaTime)
{
	sprite->update(deltaTime);
	currentTime += deltaTime;
	this->move(bLeft);

	if (Player::instance().collisionRight(posItem, sizeItem) || Player::instance().collisionLeft(posItem, sizeItem) || Player::instance().collisionDown(posItem, sizeItem, true) || Player::instance().collisionUp(posItem, sizeItem)) {
		Player::instance().giveStar();
		bVisible = false;
	}

	sprite->setPosition(posItem);
}

void Item::render()
{
	sprite->render();
}

bool Item::isVisible()
{
	return bVisible;
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
	bLeft = false;
	bVisible = true;
	currentTime = 0;
	sizeItem = glm::ivec2(32, 32);
	
	spritesheet.loadFromFile("res/textures/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 4, 0.5859f)); 

	sprite->changeAnimation(0);
	posItem = tileMapPos;
	sprite->setPosition(posItem);

}

void Star::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bLeft = false;
	bVisible = true;
	currentTime = 0;
	sizeItem = glm::ivec2(32, 32);

	spritesheet.loadFromFile("res/textures/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.1172f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(MOVE, 10);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 4, 0.828f));

	sprite->changeAnimation(0);
	posItem = tileMapPos;
	sprite->setPosition(posItem);
}

void Coin::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bLeft = false;
	bVisible = true;
	currentTime = 0;
	sizeItem = glm::ivec2(32, 32);

	spritesheet.loadFromFile("res/textures/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 10);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 4, 0.828f));

	sprite->changeAnimation(0);
	posItem = tileMapPos;
	sprite->setPosition(posItem);
}

void Coin::update(int deltaTime)
{
	sprite->update(deltaTime);
	currentTime += deltaTime;
	this->move(bLeft);

	if (Player::instance().collisionRight(posItem, sizeItem) || Player::instance().collisionLeft(posItem, sizeItem) || Player::instance().collisionDown(posItem, sizeItem, true) || Player::instance().collisionUp(posItem, sizeItem)) {
		Player::instance().giveCoin();
		bVisible = false;
	}

	sprite->setPosition(posItem);
}

void Flag::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bLeft = false;
	bVisible = true;
	currentTime = 0;
	sizeItem = glm::ivec2(32, 32);

	spritesheet.loadFromFile("res/textures/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.125f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 10);
	sprite->addKeyframe(MOVE, glm::vec2(0.125f * 4, 0.828f));

	sprite->changeAnimation(0);
	posItem = tileMapPos;
	sprite->setPosition(posItem);
}