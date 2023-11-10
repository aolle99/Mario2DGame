#include "GameManager.h"


#define GAME_LEVELS 2
#define LEVEL_TIME 151
#define START_LIVES 3

void GameManager::init()
{
	score = 0;
	lives = START_LIVES;
	time = LEVEL_TIME;
	level = 0;
	coins = 0;
	startTime = std::chrono::high_resolution_clock::now();
	bPaused = true;
	bGameOver = false;
	bLevelCompleted = false;
	scrollXmin = 0;
}

void GameManager::update(int deltaTime)
{
	if (!bPaused && !bLevelCompleted && !bGameOver) {
		// Each second , time is reduced by 1
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto timePassed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
		if (timePassed >= 1) {
			time--;
			startTime = std::chrono::high_resolution_clock::now();
		}
	}
	else if (bLevelCompleted) {
		if (time > 0) {
			--time;
			++score;
		}
		

	}
}

void GameManager::render()
{		
}

void GameManager::addScore(int score)
{
	this->score += score;
}

void GameManager::substractLive()
{
	--lives;
}

void GameManager::setTime(int time)
{
	this->time = time;
}

void GameManager::setLevel(int level)
{
	if(level != this->level)
		this->level = level;
		this->resetLevel();
}

void GameManager::setScrollX(glm::vec2 scrollX)
{
	scrollXmin = scrollX.x;
	scrollXmax = scrollX.y;
}

void GameManager::addCoin()
{
	++coins;
}

void GameManager::setGameOver(int bEnd)
{
	bGameOver = bEnd;
}

void GameManager::setLevelCompleted(bool bCompleted)
{
	bLevelCompleted = bCompleted;
}

void GameManager::setPaused(bool bPaused)
{
	this->bPaused = bPaused;
}

void GameManager::resetLevel()
{
	score = 0;
	time = LEVEL_TIME;
	coins = 0;
	startTime = std::chrono::high_resolution_clock::now();
	bPaused= true;
	bLevelCompleted = false;
	bGameOver = false;
}

int GameManager::getScore()
{
	return score;
}

int GameManager::getLives()
{
	return lives;
}

int GameManager::getTime()
{
	return time;
}

int GameManager::getMaxTime()
{
	return LEVEL_TIME;
}

int GameManager::getLevel()
{
	return level;
}

int GameManager::getCoins()
{
	return coins;
}



int GameManager::getMaxScrollX()
{
	return scrollXmax;
}

int GameManager::getMinScrollX()
{
	return scrollXmin;
}

bool GameManager::isPaused()
{
	return bPaused;
}

bool GameManager::isLevelStarted()
{
	return LEVEL_TIME > time;
}

bool GameManager::isLevelCompleted()
{
	return bLevelCompleted;
}

int GameManager::isGameOver()
{
	return bGameOver;
}

bool GameManager::hasNextLevel()
{
	if(level+1 < GAME_LEVELS)
		return true;
	else
		return false;
}

bool GameManager::levelExists(int level)
{
	return level < GAME_LEVELS;
}
