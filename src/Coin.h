#ifndef COIN_INCLUDE
#define COIN_INCLUDE

#include "Item.h"

class Coin : public Item
{
public:
	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual bool isCoin();

protected:
};
#endif // !COIN_INCLUDE
