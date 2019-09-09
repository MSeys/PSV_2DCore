#include "Transition.h"

Transition::Transition(const Color3& color, float startTransparency, float time)
	: m_IsStarted(false)
	, m_IsHalfway(false)
	, m_IsFinished(false)
	, m_Color(color)
	, m_Transparency(startTransparency)
{
	m_Velocity = (255 - startTransparency) / (time / 2);
}

void Transition::Start()
{
	m_IsStarted = true;
}

void Transition::Reset(const Color3& color, float startTransparency, float time)
{
	m_IsStarted = false;
	m_IsHalfway = false;
	m_IsFinished = false;

	m_Color = color;
	m_Transparency = startTransparency;
	m_Velocity = (255 - startTransparency) / (time / 2);
}

void Transition::Draw() const
{
	if(m_IsFinished)
	{
		return;
	}
	
	FillRect(Rectf{ 0, 0, float(SCREEN_WIDTH), float(SCREEN_HEIGHT) }, m_Color, int(m_Transparency));
}

void Transition::Update(float elapsedSec)
{
	if(m_IsFinished)
	{
		return;
	}
	
	if(m_IsStarted && !m_IsHalfway)
	{
		if (m_Transparency < 255)
		{
			m_Transparency += m_Velocity * elapsedSec;
		}

		else
		{
			m_Transparency = 255;
			m_IsHalfway = true;
		}
	}

	if(m_IsHalfway)
	{
		if (m_Transparency > 0)
		{
			m_Transparency -= m_Velocity * elapsedSec;
		}

		else
		{
			m_Transparency = 0;
			m_IsFinished = true;
		}
	}
}

bool Transition::GetHalfway() const
{
	return m_IsHalfway;
}

bool Transition::GetFinished() const
{
	return m_IsFinished;
}