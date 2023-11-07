#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Tile.h"
#include "Item.h"
#include "Player.h"
#include "Sprite.h"

#define gridSize 32
#define tilesheetWidth 256.f
#define tilesheetHeight 512.f

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

// --------------BLOCK TILE----------------
BrickTile::BrickTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item) : Tile(glm::ivec2(32,0), pos) {
	switch (item)
	{
	case 1: // mushroom
		this->item = new Mushroom();
		break;
	case 2: // Star
		this->item = new Star();
		break;
	case 3: // Coin
		this->item = new Coin();
		break;
	default:
		this->item = nullptr;
		break;
	}	
}


void BrickTile::init(ShaderProgram& shaderProgram, Texture& tilesheet)
{
	Tile::init(shaderProgram, tilesheet);
	if (item != nullptr) item->init(position, shaderProgram);
}

void BrickTile::update(int deltaTime)
{
}

void BrickTile::render()
{
	Tile::render();
}

Item* BrickTile::getItem() {
	return item;
}

bool BrickTile::collisionDown() {
	glm::vec2 posMario = Player::instance().getPosition();

	if (posMario.x + 32 > position.x && posMario.x < position.x + 32 && posMario.y + 32 > position.y && posMario.y < position.y + 32) {
		if (item != NULL) {
			//item->setActive(true);
			item->setPosition(glm::vec2(position.x, position.y - 32));
		}
		return true;
	}

	return false;
}

// --------------QUESTION TILE----------------
QuestionTile::QuestionTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item) : Tile(glm::ivec2(0, 0), pos) {
	switch (item)
	{
	case 1: // mushroom
		this->item = new Mushroom();
		break;
	case 2: // Star
		this->item = new Star();
		break;
	case 3: // Coin
		this->item = new Coin();
		break;
	}
}

enum QuestionTileAnims
{
	ROTATE
};

void QuestionTile::init(ShaderProgram& shaderProgram, Texture& tilesheet)
{

	this->tilesheet = tilesheet;
	sprite = Sprite::createSprite(glm::vec2(32,32), glm::vec2(0.25f, 0.125f), &tilesheet, &shaderProgram);
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(ROTATE, 7);
	sprite->addKeyframe(ROTATE, glm::vec2(0.25f * 0, 0.f));
	sprite->addKeyframe(ROTATE, glm::vec2(0.25f * 1, 0.f));
	sprite->addKeyframe(ROTATE, glm::vec2(0.25f * 2, 0.f));
	sprite->addKeyframe(ROTATE, glm::vec2(0.25f * 3, 0.f));

	sprite->changeAnimation(0);
	sprite->setPosition(position);

	if (item != NULL) item->init(position, shaderProgram);
}

void QuestionTile::update(int deltaTime)
{
	
}

void QuestionTile::render()
{
	sprite->render();
}

bool QuestionTile::collisionDown() {
	glm::vec2 posMario = Player::instance().getPosition();

	if (posMario.x + 32 > position.x && posMario.x < position.x + 32 && posMario.y + 32 > position.y && posMario.y < position.y + 32) {
		if (item != NULL) {
			//item->setActive(true);
			item->setPosition(glm::vec2(position.x, position.y - 32));
		}
		return true;
	}

	return false;
}

// --------------COIN TILE----------------
CoinTile::CoinTile(const glm::ivec2& tileMapPos, const glm::vec2 pos) : Tile(glm::ivec2(64, 0), pos) {}

void CoinTile::init(ShaderProgram& shaderProgram, Texture& tilesheet)
{
	Tile::init(shaderProgram, tilesheet);
}

void CoinTile::update(int deltaTime)
{
}

void CoinTile::render()
{
	Tile::render();
}

bool CoinTile::collision() {
	glm::vec2 posMario = Player::instance().getPosition();

	if (posMario.x + 32 > position.x && posMario.x < position.x + 32 && posMario.y + 32 > position.y && posMario.y < position.y + 32) {
		return true;
	}
	return false;

}

// --------------InvisibleTile----------------
InvisibleTile::InvisibleTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item) : Tile(glm::ivec2(96, 0), pos) {}

void InvisibleTile::init(ShaderProgram& shaderProgram, Texture& tilesheet)
{
	Tile::init(shaderProgram, tilesheet);
}

void InvisibleTile::update(int deltaTime)
{
}

void InvisibleTile::render()
{
	Tile::render();
}

bool InvisibleTile::collisionDown() {
	glm::vec2 posMario = Player::instance().getPosition();

	if (posMario.x + 32 > position.x && posMario.x < position.x + 32 && posMario.y + 32 > position.y && posMario.y < position.y + 32) {
		return true;
	}
	return false;

}


