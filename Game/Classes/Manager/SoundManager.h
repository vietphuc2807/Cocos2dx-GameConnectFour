// SoundManager.h
#pragma once
#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "audio/include/AudioEngine.h"
#include "cocos2d.h"

class SoundManager
{
public:
    bool isSoundEnabled() const { return soundEnabled; }
    void setSoundEnabled(bool enabled) { soundEnabled = enabled; }

    static SoundManager* getInstance()
    {
        static SoundManager instance;
        return &instance;
    }

    void playBackgroundMusic(const std::string& path, float volume = 0.5f)
    {
        if (backgroundMusicID != -1)
        {
            cocos2d::AudioEngine::stop(backgroundMusicID);
            backgroundMusicID = -1;
        }

        backgroundMusicID = cocos2d::AudioEngine::play2d(path, true);
        cocos2d::AudioEngine::setVolume(backgroundMusicID, volume);
    }

    void stopBackgroundMusic()
    {
        if (backgroundMusicID != -1)
        {
            cocos2d::AudioEngine::stop(backgroundMusicID);
            backgroundMusicID = -1;
        }
    }

    void playSFX(const std::string& path, float volume = 0.5f)
    {
        if (!soundEnabled) return;
        int sfxID = cocos2d::AudioEngine::play2d(path, false);
        cocos2d::AudioEngine::setVolume(sfxID, volume);
    }

    void setBackgroundMusicVolume(float volume)
    {
        if (backgroundMusicID != -1)
        {
            cocos2d::AudioEngine::setVolume(backgroundMusicID, volume);
        }
    }

    void pauseAll() {
        soundEnabled = false;
		cocos2d::AudioEngine::pauseAll();
    }

	void resumeAll() {
        soundEnabled = true;
		cocos2d::AudioEngine::resumeAll();
	}

private:
    bool soundEnabled = true;
    int backgroundMusicID = -1;
    SoundManager() {}
};

#endif // __SOUND_MANAGER_H__
