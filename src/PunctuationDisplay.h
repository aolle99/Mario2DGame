#ifndef _PUNCTUATIONDISPLAY_INCLUDE
#define _PUNCTUATIONDISPLAY_INCLUDE

#include "Text.h"
#include <vector>

struct displayText
{
	string text;
	glm::vec2 coords;
	int remainingTime;
};

class PunctuationDisplay
{
private:
	PunctuationDisplay();
public:
	static PunctuationDisplay& instance()
	{
		static PunctuationDisplay P;

		return P;
	}
	
	~PunctuationDisplay();

	void init();
	void update(int deltaTime);
	void render();

	void addDisplay(string text, glm::vec2 coords);

	void addDisplay(string text, glm::vec2 coords, int time);

	void clear();


private:
	vector<displayText*> displays;
	Text text;
};

#endif // _PUNCTUATIONDISPLAY_INCLUDE