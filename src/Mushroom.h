#ifndef MUSHROOM_INCLUDE
#define MUSHROOM_INCLUDE

#include "Item.h"

class Mushroom : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

private:
};
#endif // !MUSHROOM_INCLUDE
