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
	void init();
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
#endif // _PLAYER_INCLUDE
