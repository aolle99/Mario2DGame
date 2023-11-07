#ifndef _SOUND_MANAGER_INCLUDE
#define _SOUND_MANAGER_INCLUDE

#include <irrKlang.h>
using namespace irrklang;

class SoundManager {

public:
	SoundManager();
	~SoundManager();
	static SoundManager& instance() {
		static SoundManager SM;
		return SM;
	}

	void init();
	ISoundEngine* getSoundEngine();

private:
	ISoundEngine* engine;
};

#endif // _SOUND_MANAGER_INCLUDE