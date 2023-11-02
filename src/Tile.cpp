#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Tile.h"

#define gridSize 32
#define tilesheetWidth 256.f
#define tilesheetHeight 512.f

Tile::Tile(const glm::ivec2& tileMapPos, const glm::vec2 pos)
{
	this->position = pos;
	this->texturePos = tileMapPos;
}

Tile::~Tile()
{
}

void Tile::init(ShaderProgram &shaderProgram, Texture &tilesheet)
{
	this->tilesheet = tilesheet;
	glm::vec2 posTile, texCoordTile[2];
	vector<float> vertices;
	posTile = glm::vec2(position.x,position.y);
	texCoordTile[0] = glm::vec2(texturePos.x/tilesheetWidth,texturePos.y/tilesheetHeight);
	texCoordTile[1] = texCoordTile[0] + glm::vec2(gridSize/tilesheetWidth, gridSize/tilesheetHeight);
	// First triangle
	vertices.push_back(posTile.x); vertices.push_back(posTile.y);
	vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
	vertices.push_back(posTile.x + gridSize); vertices.push_back(posTile.y);
	vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
	vertices.push_back(posTile.x + gridSize); vertices.push_back(posTile.y + gridSize);
	vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
	// Second triangle
	vertices.push_back(posTile.x); vertices.push_back(posTile.y);
	vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
	vertices.push_back(posTile.x + gridSize); vertices.push_back(posTile.y + gridSize);
	vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
	vertices.push_back(posTile.x); vertices.push_back(posTile.y + gridSize);
	vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);



	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = shaderProgram.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = shaderProgram.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Tile::update(int deltaTime)
{
}

void Tile::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Tile::free()
{
	glDeleteBuffers(1, &vbo);
}

