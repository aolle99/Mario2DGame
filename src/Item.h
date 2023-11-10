#pragma once

#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	void move(bool direction);
	void die();
	void render();
	bool isVisible();
	bool isUsed();
	void show();
	virtual bool isCoin();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

protected:
	bool bFalling;
	bool bLeft;
	bool bVisible;
	bool bUsed;
	int currentTime;
	glm::ivec2 posItem, sizeItem;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
};















#endif // _ITEM_INCLUDE
