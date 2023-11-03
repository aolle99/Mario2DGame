#include "Item.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 150
#define FALL_STEP 4

enum ItemAnims
{
	MOVE, DIE, STOP
};

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

void Item::update(int deltaTime)
{
	sprite->update(deltaTime);
	currentTime += deltaTime;

	this->move(bLeft);

	if (mario->getHp() == 0) {
		mario->setDying(true);
		return;
	}

	if (mario->collisionRight(posItem, sizeItem) || mario->collisionLeft(posItem, sizeItem) || mario->collisionDown(posItem, sizeItem) || mario->collisionUp(posItem, sizeItem)) {
		mario->giveMushroom();
	}


	sprite->setPosition(posItem);
}

void Item::setPlayer(Player* player)
{
	this->mario = player;
}

void Mushroom::update(int deltaTime)
{
	sprite->update(deltaTime);
	this->move(bLeft);

	sprite->setPosition(posItem);
}

void Item::render()
{
	sprite->render(bLeft);
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
	currentTime = 0;
	sizeItem = glm::ivec2(32, 32);

	spritesheet.loadFromFile("res/textures/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.0625f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(MOVE, 7);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 0, 0.0f));

}

void Star::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	currentTime = 0;
	sizeItem = glm::ivec2(32, 32);

	spritesheet.loadFromFile("res/textures/items.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(sizeItem, glm::vec2(0.0625f, 0.125f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(MOVE, 10);
	sprite->addKeyframe(MOVE, glm::vec2(0.0625f * 3, 0.1875f));

	sprite->changeAnimation(0);
	posItem = tileMapPos;
	sprite->setPosition(posItem);
}
