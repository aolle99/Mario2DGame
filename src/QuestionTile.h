#ifndef QUESTIONTILE_INCLUDE
#define QUESTIONTILE_INCLUDE

#include "Tile.h"

class QuestionTile : public Tile
{
public:
	QuestionTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, Texture& tilesheetAnim, int item = 0);

	virtual void init(ShaderProgram& shaderProgram, Texture& tilesheet);
	virtual void update(int deltaTime);
	virtual void render();
	Item* getItem();
	void jumpAnimation();
private:
	Item* item;
	Sprite* sprite;
	bool bUsed;
	Texture tilesheetAnim;
	bool bJumping;
	float startY;
	int jumpAngle;
};
#endif // QUESTIONTILE_INCLUDE

