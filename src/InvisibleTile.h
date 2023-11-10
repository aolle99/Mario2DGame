#ifndef INVISIBLETILE_INCLUDE
#define INVISIBLETILE_INCLUDE

#include "Tile.h"

class InvisibleTile : public Tile
{
public:
	InvisibleTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item);
	virtual void init(ShaderProgram& shaderProgram, Texture& tilesheet);
	virtual void update(int deltaTime);
	virtual void render();

	Item* getItem();

private:
	Item* item;
	bool bUsed;
};

#endif // INVISIBLETILE_INCLUDE