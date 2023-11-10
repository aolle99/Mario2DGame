#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Tile.h"
#include "Item.h"
#include "Player.h"
#include "Sprite.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "SoundManager.h"

#define gridSize 32
#define tilesheetWidth 256.f
#define tilesheetHeight 512.f
#define PUNCT_BLOCK 50

Tile::Tile(const glm::ivec2 tileMapPos, const glm::vec2 pos)
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
	this->shaderProgram = &shaderProgram;
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

void Tile::render()
{
	glm::mat4 modelview;

	shaderProgram->use();
	shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	modelview = glm::mat4(1.0f);
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", 0.f, 0.f);

	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

Item* Tile::getItem()
{
	return nullptr;
}

void Tile::free()
{
	glDeleteBuffers(1, &vbo);
}

bool Tile::collisionDown()
{
	glm::vec2 posMario = Player::instance().getPosition();
	float marioX = posMario.x + 16;
	if (marioX > position.x && marioX < position.x + 32 && posMario.y > position.y && posMario.y < position.y + 32) {
		return true;
	}

	return false;
}



// --------------BLOCK TILE----------------


// --------------QUESTION TILE----------------

// --------------InvisibleTile----------------
