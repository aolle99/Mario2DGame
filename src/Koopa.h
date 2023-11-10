#ifndef KOOPA_INCLUDE
#define KOOPA_INCLUDE

#include "Enemy.h"

class Koopa : public Enemy
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	virtual void update(int deltaTime);
	virtual void collisionDeath();
	void shellMode();
	void turtleMode();
	void change_to_shell();
	void change_to_turtle();
	virtual bool isKoopaShellMove();
	virtual bool isModeTurtle();

protected:
	bool bStop;
	bool bShell;
};
#endif // !KOOPA_INCLUDE
