#include "QuestionTile.h"
#include "Game.h"
#include "Player.h"
#include "SoundManager.h"
#include "Mushroom.h"
#include "Star.h"
#include "Coin.h"


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
	sprite = Sprite::createSprite(glm::vec2(32, 32), glm::vec2(0.25f, 0.125f), &tilesheetAnim, &shaderProgram);
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
				if (!item->isCoin()) SoundManager::instance().playSound("res/sounds/powerup_appears.wav");
			}

		}
	}
}

void QuestionTile::render()
{
	if (!bUsed) sprite->render();
	else {
		Tile::render();
	}
}

Item* QuestionTile::getItem() {
	return item;
}
