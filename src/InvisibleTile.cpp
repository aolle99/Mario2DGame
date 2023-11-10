#include "InvisibleTile.h"
#include "Game.h"
#include "SoundManager.h"
#include "Mushroom.h"
#include "Star.h"
#include "Coin.h"


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
				if (!item->isCoin()) SoundManager::instance().playSound("res/sounds/powerup_appears.wav");
			}
			bUsed = true;
		}
	}
}

void InvisibleTile::render()
{
	if (bUsed) Tile::render();
}

Item* InvisibleTile::getItem() {
	return item;
}

