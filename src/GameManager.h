#ifndef _GAMEMANAGER_INCLUDE
#define _GAMEMANAGER_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "json/json.h"
#include <memory>
#include <chrono>
#include "Text.h"


// GameManager contains all the entities of our game.
// It is responsible for updating and render them.


class GameManager
{

public:
	static GameManager& instance()
	{
		static GameManager GM;

		return GM;
	}

	void init();
	void update(int deltaTime);
	void render();

	void addScore(int score);
	void substractLive();
	void setTime(int time);
	void setLevel(int level);
	void addCoin();
	void setLevelEnd(bool bEnd);
	void setLevelCompleted(bool bCompleted);
	void setPaused(bool bPaused);
	void setScrollX(int scrollX);
	int getScore();
	int getLives();
	int getTime();
	int getLevel();
	int getCoins();
	int getScrollX();

	bool isLevelEnd();
	bool isPaused();




private:
	int score, lives, time, level, coins, scrollX;
	std::chrono::time_point<std::chrono::high_resolution_clock>  startTime;
	bool bLevelEnd, bPaused, bLevelCompleted;
	string textToRender;
	glm::vec2 textCoords;
	

};


#endif // _GAMEMANAGER_INCLUDE

