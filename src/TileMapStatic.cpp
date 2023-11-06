#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMapStatic.h"
#include <json/json.h>

#define gridSize 32
#define tilesheetWidth 256.f
#define tilesheetHeight 512.f


TileMapStatic* TileMapStatic::createTileMap(const Json::Value& layerMap, const glm::ivec2& size, ShaderProgram& program)
{
	TileMapStatic* map = new TileMapStatic(layerMap, size, program);

	return map;
}


TileMapStatic::TileMapStatic(const Json::Value& layerMap, const glm::ivec2& size, ShaderProgram& program)
{
	loadLevel(layerMap, size);
	prepareArrays(program);
}

TileMapStatic::~TileMapStatic()
{
	if(tiles.size() > 0)
		tiles.clear();
}


void TileMapStatic::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMapStatic::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMapStatic::loadLevel(const Json::Value layerMap, const glm::ivec2& mapSize)
{
	this->blockSize = 32;
	tilesheet.loadFromFile("res/Textures/Tiles.png", TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	for (Json::Value block : layerMap)
	{
		glm::ivec2 pos = glm::vec2(block["px"][0].asInt(), block["px"][1].asInt());
		glm::ivec2 tileTextPos = glm::ivec2(block["src"][0].asInt(), block["src"][1].asInt());
		tiles.push_back(pos);
		tiles.push_back(tileTextPos);
	}

	return true;
}

void TileMapStatic::prepareArrays(ShaderProgram& program)
{
	glm::vec2 posTile, texCoordTile[2];
	vector<float> vertices;

	nTiles = 0;
	for (int tile = 0; tile < tiles.size(); tile += 2) {
		// Non-empty tile
		nTiles++;
		posTile = glm::vec2(tiles[tile].x, tiles[tile].y);
		texCoordTile[0] = glm::vec2(tiles[tile+1].x / tilesheetWidth, tiles[tile + 1].y / tilesheetHeight);
		texCoordTile[1] = texCoordTile[0] + glm::vec2(gridSize / tilesheetWidth, gridSize / tilesheetHeight);
		// First triangle
		vertices.push_back(posTile.x); vertices.push_back(posTile.y);
		vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
		vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
		vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
		vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
		vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
		// Second triangle
		vertices.push_back(posTile.x); vertices.push_back(posTile.y);
		vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
		vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
		vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
		vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
		vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
	}
	tiles.clear();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}






























