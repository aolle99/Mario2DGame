#include "BrickTile.h"

#include "SoundManager.h"
#include "Player.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "Mushroom.h"
#include "Star.h"
#include "Coin.h"

#define PUNCT_BLOCK 50

BrickTile::BrickTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item) : Tile(glm::ivec2(32, 0), pos) {
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
	if (item != nullptr) item->init(position - glm::vec2(0, 32), shaderProgram);
}

void BrickTile::update(int deltaTime)
{
	if (!bDestroyed) {
		if (collisionDown()) {
			if (item != nullptr) {
				item->show();
				if (!item->isCoin()) SoundManager::instance().playSound("res/sounds/powerup_appears.wav");
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
	if (!bDestroyed) Tile::render();
}

void BrickTile::destroy() {
	if (item == nullptr) {
		Player::instance().removeCollisionBlock(position.x, position.y);
		bDestroyed = true;
		PunctuationDisplay::instance().addDisplay(to_string(PUNCT_BLOCK), position);
		GameManager::instance().addScore(PUNCT_BLOCK);
		SoundManager::instance().playSound("res/sounds/breakblock.wav");
	}
	else {
		texturePos = glm::ivec2(96, 0);
		Tile::init(*shaderProgram, tilesheet);
	}
}

Item* BrickTile::getItem() {
	return item;
}