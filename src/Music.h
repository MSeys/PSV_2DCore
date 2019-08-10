#pragma once
#include "pch.h"

class Music
{
	SoLoud::Soloud* m_pSoLoudEngine;
	SoLoud::WavStream* m_pWavStream;

	int m_Volume;

public:
	Music(SoLoud::Soloud* pSoLoudEngine, const std::string& filePath);
	~Music();

	void Play(bool isLooping) const;
	void PlayBackground(bool isLooping) const;
	void SetVolume(int volume);
	int GetVolume() const;
	void Stop() const;
};

