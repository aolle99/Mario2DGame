#ifndef _GAMEMANAGER_INCLUDE
#define _GAMEMANAGER_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "json/json.h"
#include <memory>
#include <chrono>


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

	void setScore(int score);
	void substractLive();
	void setTime(int time);
	void setLevel(int level);
	void addCoin();

	int getScore();
	int getLives();
	int getTime();
	int getLevel();
	int getCoins();




private:
	int score, lives, time, level, coins;
	std::chrono::time_point<std::chrono::high_resolution_clock>  startTime;
	

};


#endif // _GAMEMANAGER_INCLUDE

