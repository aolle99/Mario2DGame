#ifndef CASTLEFLAG_INCLUDE
#define CASTLEFLAG_INCLUDE

#include "Item.h"

class CastleFlag : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
protected:
};

#endif // !CASTLEFLAG_INCLUDE
