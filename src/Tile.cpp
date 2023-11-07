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
BrickTile::BrickTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item) : Tile(glm::ivec2(32,0), pos) {
	bDestroyed = false;

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
	if (item != nullptr) item->init(position-glm::vec2(0,32), shaderProgram);
}

void BrickTile::update(int deltaTime)
{
	if (!bDestroyed) {
		if (collisionDown()) {
			if (item != nullptr) {
				item->show();
			}
			if (Player::instance().isSuperMario()) {
				destroy();
			}
			else {
				//jump
				if (!bJumping) {
					bJumping = true;
					startY = position.y;
				}
			}
		}
	}
}

void BrickTile::render()
{
	if(!bDestroyed) Tile::render();
}

void BrickTile::destroy() {
	if (item == nullptr) {
		Player::instance().removeCollisionBlock(position.x, position.y);
		bDestroyed = true;
	}
	else {
		texturePos = glm::ivec2(96, 0);
		Tile::init(*shaderProgram, tilesheet);
	}
}

Item* BrickTile::getItem() {
	return item;
}

// --------------QUESTION TILE----------------
QuestionTile::QuestionTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, Texture& tilesheetAnim, int item) : Tile(glm::ivec2(0, 0), pos)
{
	bUsed = false;
	this->tilesheetAnim = tilesheetAnim;
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
	this->shaderProgram = &shaderProgram;
	this->tilesheet = tilesheet;
	sprite = Sprite::createSprite(glm::vec2(32,32), glm::vec2(0.25f, 0.125f), &tilesheetAnim, &shaderProgram);
	sprite->setNumberAnimations(4);
	sprite->setAnimationSpeed(ROTATE, 2);
	sprite->addKeyframe(ROTATE, glm::vec2(0.25f * 0, 0.f));
	sprite->addKeyframe(ROTATE, glm::vec2(0.25f * 1, 0.f));
	sprite->addKeyframe(ROTATE, glm::vec2(0.25f * 2, 0.f));
	sprite->addKeyframe(ROTATE, glm::vec2(0.25f * 3, 0.f));

	sprite->changeAnimation(0);
	sprite->setPosition(position);

	if (item != nullptr) item->init(position - glm::vec2(0, 32), shaderProgram);
}

void QuestionTile::update(int deltaTime)
{
	if (!bUsed) {
		sprite->update(deltaTime);
		if (collisionDown()) {

			bUsed = true;
			texturePos = glm::ivec2(96, 0);
			Tile::init(*shaderProgram, tilesheet);
			if (item != nullptr) {
				item->show();
			}

		}
	}
}

void QuestionTile::render()
{
	if(!bUsed) sprite->render();
	else {
		Tile::render();
	}
}

Item* QuestionTile::getItem() {
	return item;
}

// --------------InvisibleTile----------------
InvisibleTile::InvisibleTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item) : Tile(glm::ivec2(96, 0), pos) {
	bUsed = false;
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

void InvisibleTile::init(ShaderProgram& shaderProgram, Texture& tilesheet)
{
	Tile::init(shaderProgram, tilesheet);
	if (item != nullptr) item->init(position - glm::vec2(0, 32), shaderProgram);
}

void InvisibleTile::update(int deltaTime)
{
	if (!bUsed) {
		if (collisionDown()) {
			if (item != nullptr) {
				item->show();
			}
			bUsed = true;
		}
	}
}

void InvisibleTile::render()
{
	if(bUsed) Tile::render();
}

Item* InvisibleTile::getItem() {
	return item;
}

