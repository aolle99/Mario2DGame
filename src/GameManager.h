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
	void addLive();
	void substractLive();
	void setTime(int time);
	void setLevel(int level);
	void setScrollX(glm::vec2 scrollX);
	void addCoin();
	void setGameOver(int bEnd);
	void setLevelCompleted(bool bCompleted);
	void setGameOverSoundPlayed(bool bPlayed);
	void setPaused(bool bPaused);
	void resetLevel();
	int getMaxScrollX();
	int getMinScrollX();
	int getScore();
	int getLives();
	int getTime();
	int getMaxTime();
	int getLevel();
	int getCoins();
	

	bool isPaused();
	bool isLevelStarted();
	bool isLevelCompleted();
	int isGameOver();
	bool hasNextLevel();
	bool levelExists(int level);
	bool isGameOverSoundPlayed();
	


private:
	int score, lives, time, level, coins, scrollXmin, scrollXmax, bGameOver;
	std::chrono::time_point<std::chrono::high_resolution_clock>  startTime;
	bool bPaused, bLevelCompleted, bGameOverSoundPlayed;
	string textToRender;
	glm::vec2 textCoords;
	

};


#endif // _GAMEMANAGER_INCLUDE

