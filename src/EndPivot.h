#ifndef ENDPIVOT_INCLUDE
#define ENDPIVOT_INCLUDE

#include "Item.h"

class EndPivot : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int multiplier);
	virtual void update(int deltaTime);
protected:
	int multiplier = 10;
};


#endif // !ENDPIVOT_INCLUDE
