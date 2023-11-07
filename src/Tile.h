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

	void free();

	void setPosition(const glm::vec2 &pos);

	bool collisionDown(const glm::ivec2& pos, const glm::ivec2& size, int object_type);

	bool collisionUp(const glm::ivec2& object_pos, const glm::ivec2& object_size, int object_type);

	bool collisionLeft(const glm::ivec2& object_pos, const glm::ivec2& object_size, int object_type);

	bool collisionRight(const glm::ivec2& object_pos, const glm::ivec2& object_size, int object_type);

protected:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	Texture tilesheet;
	glm::vec2 position;
	glm::ivec2 texturePos;
};

class BrickTile : public Tile
{
public:
	BrickTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item=0);

	virtual void init(ShaderProgram& shaderProgram, Texture& tilesheet);
	virtual void update(int deltaTime);
	virtual void render();

	Item* getItem();

	bool collisionDown();
private:
	Item* item;
};

class QuestionTile : public Tile
{
public:
	QuestionTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item = 0);

	virtual void init(ShaderProgram& shaderProgram, Texture& tilesheet);
	virtual void update(int deltaTime);
	virtual void render();

	bool collisionDown();
private:
	Item* item;
	Sprite* sprite;
};

class CoinTile : public Tile
{
public:
	CoinTile(const glm::ivec2& tileMapPos, const glm::vec2 pos);

	virtual void init(ShaderProgram& shaderProgram, Texture& tilesheet);
	virtual void update(int deltaTime);
	virtual void render();

	bool collision();

private:
	Sprite* sprite;
};

class InvisibleTile : public Tile
{
public:
	InvisibleTile(const glm::ivec2& tileMapPos, const glm::vec2 pos, int item);

	virtual void init(ShaderProgram& shaderProgram, Texture& tilesheet);
	virtual void update(int deltaTime);
	virtual void render();

	bool collisionDown();

private:
	Item* item;
};

#endif // _PLAYER_INCLUDE
