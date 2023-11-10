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


#define FALL_STEP 4

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
		if (map->collisionMoveRight(posItem, sizeItem, &posItem.x))
		{
			bLeft = !bLeft;
		}

	}
	else { // right
		posItem.x -= 2;
		if (map->collisionMoveLeft(posItem, sizeItem, &posItem.x)) // mira si hi ha colisio a l'esquerra
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