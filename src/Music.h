#pragma once
#include "soloud.h"
#include "soloud_wavstream.h"
#include <string>

class Music
{
	SoLoud::Soloud* m_pSoLoudEngine;
	SoLoud::WavStream* m_pWavStream;

	int m_Volume;

public:
	Music(SoLoud::Soloud* pSoLoudEngine, const std::string& filePath);

	Music(const Music& other) = delete;
	Music& operator=(const Music& rhs) = delete;
	Music(const Music&& other) = delete;
	Music& operator=(const Music&& rhs) = delete;
	
	~Music();

	void Play(bool isLooping) const;
	void PlayBackground(bool isLooping) const;
	void SetVolume(int volume);
	int GetVolume() const;
	void Stop() const;
};

