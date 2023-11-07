#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include <json/json.h>
#include <memory>

using namespace std;

enum BlockTypes {
	BRICK = 1,
	BRICK_COIN = 2,
	QUESTION_COIN = 10,
	QUESTION_MUSHROOM = 11,
	BRICK_STAR = 12,
	INVISIBLE_COIN = 18,
	INVISIBLE_MUSHROOM = 19,
	BRICK_MUSHROOM = 27,
	COIN = 24,
};

enum ItemTypes {
	MUSHROOM = 1,
	STAR = 2,
	COIN_ITEM = 3,
};


TileMap* TileMap::createTileMap(const Json::Value& layerMap, const glm::ivec2& size, ShaderProgram &program)
{
	TileMap *map = new TileMap(layerMap, size, program);
	
	return map;
}


TileMap::TileMap(const Json::Value &layerMap, const glm::ivec2& size, ShaderProgram &program)
{
	loadLevel(layerMap,size);
	prepareArrays(program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
	if(blocks.size() > 0)
		blocks.clear();
		
}


void TileMap::render() const
{
	for (const auto& block : blocks)
	{
		block->render();
	}
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const Json::Value layerMap, const glm::ivec2& mapSize)
{
	this->mapSize = mapSize;
	this->tileSize = 32;
	this->blockSize = 32;
	tilesheet.loadFromFile("res/Textures/Tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	tilesheetAnim.loadFromFile("res/Textures/AnimTiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheetAnim.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheetAnim.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheetAnim.setMinFilter(GL_NEAREST);
	tilesheetAnim.setMagFilter(GL_NEAREST);

	map = new bool[mapSize.x * mapSize.y];

	for (int i = 0; i < (mapSize.x * mapSize.y); i++)
	{
		map[i] = false;
	}
	for (Json::Value block : layerMap)
	{
		glm::ivec2 pos = glm::vec2(block["px"][0].asInt(), block["px"][1].asInt());
		glm::ivec2 tileTextPos = glm::ivec2(block["src"][0].asInt(), block["src"][1].asInt());
		//blocks.push_back(make_unique<Tile>(tileTextPos, pos));
		createTile(pos, tileTextPos, block["t"].asInt());
		map[(pos.y / 32) * mapSize.x + (pos.x / 32)] = true;
	}
	
	return true;
}

void TileMap::prepareArrays( ShaderProgram& program)
{
	for (const auto& block : blocks)
	{
		if (typeid(*block) == typeid(BrickTile))
		{
			block->init(program, tilesheet);
		}
		else if (typeid(*block) == typeid(QuestionTile))
		{
			block->init(program, tilesheetAnim);
		}
		else
		{
			block->init(program, tilesheet);
		}
		Item* item = block->getItem();
		if (item != nullptr) items.push_back(item);
	}
}

void TileMap::createTile(const glm::ivec2& pos, const glm::ivec2& texPos, int type)
{
	switch (type)
	{
	case BlockTypes::BRICK:
		blocks.push_back(new BrickTile(texPos, pos));
		break;
	case BlockTypes::BRICK_COIN:
		blocks.push_back(new BrickTile(texPos, pos, ItemTypes::COIN_ITEM));
		break;
		/*
	case BlockTypes::QUESTION_COIN:
		blocks.push_back(new QuestionTile(texPos, pos, ItemTypes::COIN_ITEM));
		break;
	case BlockTypes::QUESTION_MUSHROOM:
		blocks.push_back(new QuestionTile(texPos, pos, ItemTypes::MUSHROOM));
		break;*/
	case BlockTypes::BRICK_STAR:
		blocks.push_back(new BrickTile(texPos, pos, ItemTypes::STAR));
		break;
	case BlockTypes::INVISIBLE_COIN:
		blocks.push_back(new InvisibleTile(texPos, pos, ItemTypes::COIN_ITEM));
		break;
	case BlockTypes::INVISIBLE_MUSHROOM:
		blocks.push_back(new InvisibleTile(texPos, pos, ItemTypes::MUSHROOM));
		break;
	case BlockTypes::BRICK_MUSHROOM:
		blocks.push_back(new BrickTile(texPos, pos, ItemTypes::MUSHROOM));
		break;
	case BlockTypes::COIN:
		blocks.push_back(new CoinTile(texPos, pos));
		break;
	default:
		blocks.push_back(new Tile(texPos, pos));
		
	}
	
}

glm::ivec2 TileMap::getSize() {
	return mapSize;
}

vector<Item*> TileMap::getItems()
{
	return items;
}

bool TileMap::checkOutOfBoundsLeft(float posX) {
	if (posX < leftBound) {
		return true;
	}
	return false;
}
bool TileMap::checkOutOfBoundsRight(float posX) {
	if (posX > mapSize.x * 32) {
		return true;
	}
	return false;
}

bool TileMap::checkOutOfBoundsDown(float posY) {
	return posY > mapSize.y * 32;
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

void TileMap::setLeftBound(float leftBound)
{
	this->leftBound = leftBound;
}



bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x])
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
		{
			if(*posY - tileSize * y + size.y <= 10)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}
	
	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if(map[y*mapSize.x+x] != 0)
			return true;
	}
	
	return false;
}






























