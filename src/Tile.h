#ifndef _TILE_INCLUDE
#define _TILE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "json/json.h"

class Tile
{
public:
	Tile(const glm::ivec2& tileMapPos, const glm::vec2 pos);
	~Tile();

	void init(ShaderProgram &shaderProgram,Texture &tilesheet);
	void update(int deltaTime);
	void render() const;

	void free();

	void setPosition(const glm::vec2 &pos);

	bool collisionDown(const glm::ivec2& pos, const glm::ivec2& size, int object_type);

	bool collisionUp(const glm::ivec2& object_pos, const glm::ivec2& object_size, int object_type);

	bool collisionLeft(const glm::ivec2& object_pos, const glm::ivec2& object_size, int object_type);

	bool collisionRight(const glm::ivec2& object_pos, const glm::ivec2& object_size, int object_type);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	Texture tilesheet;
	glm::vec2 position;
	glm::ivec2 texturePos;
};


#endif // _PLAYER_INCLUDE
