#ifndef BRICKTILE_INCLUDE
#define BRICKTILE_INCLUDE

#include "Tile.h"
class BrickTile : public Tile
{
public:
	BrickTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item = 0);

	virtual void init(ShaderProgram& shaderProgram, Texture& tilesheet);
	virtual void update(int deltaTime);
	virtual void render();

	void destroy();

	virtual Item* getItem();

private:
	Item* item;
	bool bJumping;
	float startY;
	bool bDestroyed;
};
#endif // BRICKTILE_INCLUDE
