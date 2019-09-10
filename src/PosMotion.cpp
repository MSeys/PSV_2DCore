#include "PosMotion.h"

PosMotion::PosMotion()
{
}

PosMotion::PosMotion(const Point2f& pos, float time, const Point2f& limits, const AxisLimit& axisLimit)
	: m_Pos(pos)
	, m_Limits(limits)
	, m_AxisLimit(axisLimit)
{
	m_Velocity.x = abs(m_Pos.x - limits.x) / time;
	m_Velocity.y = abs(m_Pos.y - limits.y) / time;

	switch(m_AxisLimit)
	{
	case NONE:
		break;
	case X:
		if(m_Velocity.x > 0)
		{
			m_ComparePosToLimit = SMALLER;
		}

		else
		{
			m_ComparePosToLimit = BIGGER;
		}
		break;
	case Y:
		if (m_Velocity.y > 0)
		{
			m_ComparePosToLimit = SMALLER;
		}

		else
		{
			m_ComparePosToLimit = BIGGER;
		}
		break;
	default:
		break;
	}
}

PosMotion::PosMotion(const Point2f& pos, const Scale2f& velocity, const Point2f& limits, const AxisLimit& axisLimit)
	: m_Pos(pos)
	, m_Limits(limits)
	, m_Velocity(velocity)
	, m_AxisLimit(axisLimit)
{
	switch (m_AxisLimit)
	{
	case NONE:
		break;
	case X:
		if (m_Velocity.x > 0)
		{
			m_ComparePosToLimit = SMALLER;
		}

		else
		{
			m_ComparePosToLimit = BIGGER;
		}
		break;
		
	case Y:
		if (m_Velocity.y > 0)
		{
			m_ComparePosToLimit = SMALLER;
		}

		else
		{
			m_ComparePosToLimit = BIGGER;
		}
		break;
	default:
		break;
	}
}

void PosMotion::Start()
{
	m_IsStarted = true;
}

int PosMotion::Update(float elapsedSec)
{
	if(!m_IsStarted)
	{
		return 0;
	}
	
	switch(m_AxisLimit)
	{
	case NONE:
		UpdateNone(elapsedSec);

	case X:
		return UpdateX(elapsedSec);
		
	case Y:
		return UpdateY(elapsedSec);
	default:
		return 1;
	}
}

Point2f PosMotion::GetPos() const
{
	return m_Pos;
}

void PosMotion::Reset(const Point2f& pos, float time, const Point2f& limits, const AxisLimit& axisLimit)
{
	m_IsStarted = false;

	m_Pos = pos;
	m_Limits = limits;
	m_AxisLimit = axisLimit;
	m_Velocity.x = abs(m_Pos.x - limits.x) / time;
	m_Velocity.y = abs(m_Pos.y - limits.y) / time;
}

void PosMotion::Reset(const Point2f& pos, const Scale2f& velocity, const Point2f& limits, const AxisLimit& axisLimit)
{
	m_IsStarted = false;
	
	m_Pos = pos;
	m_Limits = limits;
	m_AxisLimit = axisLimit;
	m_Velocity = velocity;
}

void PosMotion::UpdateNone(float elapsedSec)
{
	m_Pos.x += m_Velocity.x * elapsedSec;
	m_Pos.y += m_Velocity.y * elapsedSec;
}

int PosMotion::UpdateX(float elapsedSec)
{
	switch(m_ComparePosToLimit)
	{
	case SMALLER:
		if (m_Pos.x < m_Limits.x)
		{
			m_Pos.x += m_Velocity.x * elapsedSec;
			m_Pos.y += m_Velocity.y * elapsedSec;
			return 0;
		}

		return 1;

	case BIGGER:
		if (m_Pos.x > m_Limits.x)
		{
			m_Pos.x += m_Velocity.x * elapsedSec;
			m_Pos.y += m_Velocity.y * elapsedSec;
			return 0;
		}

		return 1;
	default:
		return 1;
	}
}

int PosMotion::UpdateY(float elapsedSec)
{
	switch (m_ComparePosToLimit)
	{
	case SMALLER:
		if (m_Pos.y < m_Limits.y)
		{
			m_Pos.x += m_Velocity.x * elapsedSec;
			m_Pos.y += m_Velocity.y * elapsedSec;
			return 0;
		}

		return 1;

	case BIGGER:
		if (m_Pos.y > m_Limits.y)
		{
			m_Pos.x += m_Velocity.x * elapsedSec;
			m_Pos.y += m_Velocity.y * elapsedSec;
			return 0;
		}

		return 1;
	default:
		return 1;
	}
}