#pragma once

#ifndef _ITEM_INCLUDE
#define _ITEM_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Item
{
public:
	void move(bool direction);
	void die();
	void update(int deltaTime);
	void setPlayer(Player* player);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

protected:
	bool bFalling;
	bool bLeft;
	int currentTime;
	glm::ivec2 posItem, sizeItem;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Player* mario;
	ShaderProgram shaderProgram;
};



class Mushroom : public Item
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);

private:
};

class Star : public Item
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	//void update(int deltaTime);

protected:
};


#endif // _ITEM_INCLUDE
