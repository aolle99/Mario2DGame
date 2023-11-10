#ifndef GREENMUSHROOM_INCLUDE
#define GREENMUSHROOM_INCLUDE

#include "Item.h"

class GreenMushroom : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);

private:
};
#endif // !GREENMUSHROOM_INCLUDE
