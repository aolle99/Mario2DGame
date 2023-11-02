#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include <json/json.h>
#include <memory>

using namespace std;


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
	map = new bool[mapSize.x * mapSize.y];

	for (int i = 0; i < (mapSize.x * mapSize.y); i++)
	{
		map[i] = false;
	}
	for (Json::Value block : layerMap)
	{
		glm::ivec2 pos = glm::vec2(block["px"][0].asInt(), block["px"][1].asInt());
		glm::ivec2 tileTextPos = glm::ivec2(block["src"][0].asInt(), block["src"][1].asInt());
		blocks.push_back(make_unique<Tile>(tileTextPos, pos));
		map[(pos.y / 32) * mapSize.x + (pos.x / 32)] = true;
	}
	
	return true;
}

void TileMap::prepareArrays( ShaderProgram& program)
{
	for (const auto& block : blocks)
	{
		block->init(program, tilesheet);
	}
}

glm::ivec2 TileMap::getSize() {
	return mapSize;
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






























