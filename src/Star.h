#ifndef STAR_INCLUDE
#define STAR_INCLUDE

#include "Item.h"


class Star : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

protected:
};
#endif // !STAR_INCLUDE

