#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "json/json.h"
#include "Sprite.h"

class Item;

class Tile
{
public:
	Tile(const glm::ivec2 tileMapPos, const glm::vec2 pos);
	~Tile();

	virtual void init(ShaderProgram &shaderProgram,Texture &tilesheet);
	virtual void update(int deltaTime){};
	virtual void render();

	virtual Item* getItem();
	virtual void setAnimatedTileSheet(Texture& tilesheetAnim) {};

	void free();

	void setPosition(const glm::vec2 &pos);

	bool collisionDown();

protected:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	Texture tilesheet;
	glm::vec2 position;
	glm::ivec2 texturePos;
	ShaderProgram* shaderProgram;
};

class BrickTile : public Tile
{
public:
	BrickTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item=0);

	virtual void init(ShaderProgram& shaderProgram, Texture& tilesheet);
	virtual void update(int deltaTime);
	virtual void render();

	void destroy();

	virtual Item* getItem();

private:
	Item* item;
	bool bJumping;
	float startY;
	bool bDestroyed;
};

class QuestionTile : public Tile
{
public:
	QuestionTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, Texture& tilesheetAnim, int item = 0 );

	virtual void init(ShaderProgram& shaderProgram, Texture& tilesheet);
	virtual void update(int deltaTime);
	virtual void render();
	Item* getItem();
private:
	Item* item;
	Sprite* sprite;
	bool bUsed;
	Texture tilesheetAnim;
};

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

#endif // _PLAYER_INCLUDE
