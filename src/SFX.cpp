#include "SFX.h"

SFX::SFX(SoLoud::Soloud* pSoLoudEngine, const std::string& filePath)
	: m_pSoLoudEngine(pSoLoudEngine)
	, m_pWav(new SoLoud::Wav())
	, m_Volume(100)
{
	m_pWav->load(filePath.c_str());
	SetVolume(m_Volume);
}

SFX::~SFX()
{
	delete m_pWav;
}

void SFX::Play() const
{
	m_pSoLoudEngine->play(*m_pWav);
}

void SFX::PlayBackground() const
{
	m_pSoLoudEngine->playBackground(*m_pWav);
}

void SFX::SetVolume(int volume)
{
	m_Volume = volume;
	m_pWav->setVolume(volume / 100.f);
}

int SFX::GetVolume() const
{
	return m_Volume;
}

void SFX::Stop() const
{
	m_pWav->stop();
}
