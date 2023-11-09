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



class Mushroom : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

private:
};

class Star : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

protected:
};

class Coin : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

protected:
};

class Flag : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, glm::ivec2 minCoords);
	virtual void update(int deltaTime);
private:
	glm::ivec2 minCoords;
};

class CastleFlag : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
protected:
};

class EndPivot : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int multiplier);
	virtual void update(int deltaTime);
protected:
	int multiplier = 10;
};


#endif // _ITEM_INCLUDE
