#ifndef _TILE_MAP_STATIC_INCLUDE
#define _TILE_MAP_STATIC_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <json/json.h>
#include "Tile.h"
#include <memory>

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMapStatic
{

private:
	TileMapStatic(const Json::Value& layerMap, const glm::ivec2& size, ShaderProgram& program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMapStatic* createTileMap(const Json::Value& layerMap, const glm::ivec2& size, ShaderProgram& program);

	~TileMapStatic();

	void render() const;
	void free();

private:
	bool loadLevel(const Json::Value layerMap, const glm::ivec2& mapSize);
	void prepareArrays();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, tilesheetSize;
	int blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	vector< glm::ivec2> tiles;
	ShaderProgram* shaderProgram;

};


#endif // _TILE_MAP_INCLUDE


