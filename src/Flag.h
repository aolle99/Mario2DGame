#ifndef FLAG_INCLUDE
#define FLAG_INCLUDE

#include "Item.h"

class Flag : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, glm::ivec2 minCoords);
	virtual void update(int deltaTime);
private:
	glm::ivec2 minCoords;
};

#endif // !FLAG_INCLUDE