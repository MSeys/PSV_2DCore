#pragma once
#include "pch.h"

class Transition
{
	bool m_IsStarted;
	bool m_IsHalfway;
	bool m_IsFinished;

	Color3 m_Color;
	float m_Transparency;
	float m_Velocity;

public:
	Transition(const Color3& color = C3_BLACK, float startTransparency = 0, float time = 2.f);
	
	void Start();
	void Reset(const Color3& color = C3_BLACK, float startTransparency = 0, float time = 2.f);
	
	void Draw() const;
	void Update(float elapsedSec);

	bool GetHalfway() const;
	bool GetFinished() const;
};