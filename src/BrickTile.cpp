#include "BrickTile.h"

#include "SoundManager.h"
#include "Player.h"
#include "GameManager.h"
#include "PunctuationDisplay.h"
#include "Mushroom.h"
#include "Star.h"
#include "Coin.h"
#include "Game.h"

#define PUNCT_BLOCK 50
#define JUMP_ANGLE_STEP 10.f

BrickTile::BrickTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item) : Tile(glm::ivec2(32, 0), pos) {
	bDestroyed = false;
	bDestroying = false;
	bJumping = false;
	startY = 0;
	jumpAngle = 0;

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
		if (bJumping) {
			jumpAnimation();
		}
		else if (bDestroying) {
			destroyAnimation();
		}
		else if (collisionDown()) {
			if (item != nullptr) {
				item->show();

				if (!item->isCoin()) SoundManager::instance().playSound("res/sounds/powerup_appears.wav");
			}
			if (!bDestroying && Player::instance().isSuperMario()) {
				startY = position.y;
				if(item != nullptr) destroy();
				else bDestroying = true;
				PunctuationDisplay::instance().addDisplay(to_string(PUNCT_BLOCK), position);
				SoundManager::instance().playSound("res/sounds/breakblock.wav");
				GameManager::instance().addScore(PUNCT_BLOCK);
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

void BrickTile::destroyAnimation() {
	if (position.y < SCREEN_HEIGHT) {
		position.y += 7;
		Tile::init();
	}
	else {
		bDestroying = false;
		destroy();
	}
		
}

void BrickTile::jumpAnimation() {
	if (jumpAngle < 180.f) {

		position.y = int(startY - 15 * sin(3.14159f * jumpAngle / 180.f));
		jumpAngle += JUMP_ANGLE_STEP;

	}
	else {
		position.y = startY;
		bJumping = false;
		jumpAngle = 0;
	}
	Tile::init();
}

void BrickTile::destroy() {
	if (item == nullptr) {
		Player::instance().removeCollisionBlock(position.x, startY);
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