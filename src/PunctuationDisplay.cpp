#include "PunctuationDisplay.h"
#include <iostream>
#include "GameManager.h"

using namespace std;

#define defaultTime 1500

#define SCREEN_WIDTH 1024

PunctuationDisplay::PunctuationDisplay()
{
}

PunctuationDisplay::~PunctuationDisplay()
{
		clear();
}

void PunctuationDisplay::init()
{
	if (!text.init("res/Fonts/main_font.ttf"))
		cout << "Could not load font!!!" << endl;
}

void PunctuationDisplay::update(int deltaTime)
{
	for (int i = 0; i < displays.size(); i++) {
		displays[i]->remainingTime -= deltaTime;
		if (displays[i]->remainingTime <= 0) {
			displays.erase(displays.begin() + i);
			i--;
		}
	}
}

void PunctuationDisplay::render()
{
	for (int i = 0; i < displays.size(); i++) {
		text.render(displays[i]->text, displays[i]->coords, 15, glm::vec4(1.f, 1.f, 1.f, 1.f));
	}
}

void PunctuationDisplay::addDisplay(string text, glm::vec2 coords)
{
	float dispX = GameManager::instance().getScrollX() - SCREEN_WIDTH + coords.x;
	float dispY = coords.y - 20;
	displayText* display = new displayText;
	display->text = text;
	display->coords = glm::vec2(dispX, dispY);
	display->remainingTime = defaultTime;
	displays.push_back(display);
}

void PunctuationDisplay::addDisplay(string text, glm::vec2 coords, int time)
{
	displayText* display = new displayText;
	display->text = text;
	display->coords = coords;
	display->remainingTime = time;
	displays.push_back(display);
}

void PunctuationDisplay::clear()
{
	for (int i = 0; i < displays.size(); i++) {
		delete displays[i];
	}
	displays.clear();
}