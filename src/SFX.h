#pragma once
#include "pch.h"

class SFX
{
	SoLoud::Soloud* m_pSoLoudEngine;
	SoLoud::Wav* m_pWav;

	int m_Volume;

public:
	SFX(SoLoud::Soloud* pSoLoudEngine, const std::string& filePath);
	~SFX();

	void Play() const;
	void PlayBackground() const;
	void SetVolume(int volume);
	int GetVolume() const;
	void Stop() const;
};

