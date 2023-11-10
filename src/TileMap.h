#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Tile.h"
#include <memory>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class TileMap
{

private:
	TileMap(const Json::Value &layerMap,  const glm::ivec2& size, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const Json::Value &layerMap, const glm::ivec2 &size, ShaderProgram &program);

	~TileMap();

	void render() const;
	void update(int deltaTime);
	void free();
	
	int getTileSize() const { return tileSize; }
	void setLeftBound(float leftBound);

	bool checkOutOfBoundsDown(float posX);

	bool checkOutOfBounds(glm::ivec2 pos) const;

	glm::ivec2 TileMap::getSize();

	vector<Item*> getItems();

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, int* posX) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, int *posX) const;
	bool nearFall(const glm::ivec2& pos, const glm::ivec2& size, bool direction) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;

	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;

	void removeCollisionBlock(int x, int y);
	bool checkOutOfBoundsLeft(int* posX) const;

	bool checkOutOfBoundsRight(int* posX) const;
	
private:
	bool loadLevel(const Json::Value layerMap, const glm::ivec2& mapSize);
	void prepareArrays(ShaderProgram& program);

	void createTile(const glm::ivec2& pos, const glm::ivec2& texPos, int type);



private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	Texture tilesheetAnim;
	glm::vec2 tileTexSize;
	bool *map;
	//vector< std::unique_ptr<Tile >> blocks;
	vector<Tile*> blocks;
	float leftBound = 0.f;
	vector<Item*> items;
};


#endif // _TILE_MAP_INCLUDE


