#ifndef GOOMBA_INCLUDE
#define GOOMBA_INCLUDE

#include "Enemy.h"

class Goomba : public Enemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void collisionDeath();
	void smashedDeath();

private:
	bool bSmashed;
};

#endif // !GOOMBA_INCLUDE

