#include "SoundManager.h"

SoundManager::SoundManager() {
	engine = NULL;
}

SoundManager::~SoundManager() {
	engine->drop();
}

void SoundManager::init() {
	engine = createIrrKlangDevice();
}

ISoundEngine* SoundManager::getSoundEngine() {
	return engine;
}

void SoundManager::playSound(const char* sound) {
	engine->play2D(sound);
}

void SoundManager::playMusic(const char* music) {
	engine->play2D(music, true);
}

void SoundManager::stopMusic() {
	engine->stopAllSounds();
}

void SoundManager::setVolume(float volume) {
	engine->setSoundVolume(volume);
}

