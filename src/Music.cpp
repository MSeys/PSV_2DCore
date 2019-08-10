#include "Music.h"

Music::Music(SoLoud::Soloud* pSoLoudEngine, const std::string& filePath)
	: m_pSoLoudEngine(pSoLoudEngine)
	, m_pWavStream(new SoLoud::WavStream())
	, m_Volume(100)
{
	m_pWavStream->load(filePath.c_str());
	SetVolume(m_Volume);
}

Music::~Music()
{
	delete m_pWavStream;
}

void Music::Play(bool isLooping) const
{
	m_pWavStream->setLooping(isLooping);
	m_pSoLoudEngine->play(*m_pWavStream);
}

void Music::PlayBackground(bool isLooping) const
{
	m_pWavStream->setLooping(isLooping);
	m_pSoLoudEngine->playBackground(*m_pWavStream);
}

void Music::SetVolume(int volume)
{
	m_Volume = volume;
	m_pWavStream->setVolume(volume / 100.f);
}

int Music::GetVolume() const
{
	return m_Volume;
}

void Music::Stop() const
{
	m_pWavStream->stop();
}
