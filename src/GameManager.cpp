#include "GameManager.h"


void GameManager::init()
{
	score = 0;
	lives = 3;
	time = 100;
	level = 1;
	coins = 0;
	startTime = std::chrono::high_resolution_clock::now();
}

void GameManager::update(int deltaTime)
{
	// Each second , time is reduced by 1
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto timePassed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
	if (timePassed >= 1) {
		time--;
		startTime = std::chrono::high_resolution_clock::now();
	}

}

void GameManager::render()
{
}

void GameManager::setScore(int score)
{
	this->score = score;
}

void GameManager::substractLive(int lives)
{
	--lives;
}

void GameManager::setTime(int time)
{
	this->time = time;
}

void GameManager::setLevel(int level)
{
	this->level = level;
}

void GameManager::setCoins(int coins)
{
	this->coins = coins;
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

int GameManager::getLevel()
{
	return level;
}

int GameManager::getCoins()
{
	return coins;
}