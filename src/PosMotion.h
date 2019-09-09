#pragma once
#include "pch.h"

enum AxisLimit
{
	NONE, X, Y
};

enum CompareLimit
{
	SMALLER, BIGGER
};

class PosMotion
{
	bool m_IsStarted{ false };
	Point2f m_Pos{};
	Point2f m_Limits{};
	
	Scale2f m_Velocity{};
	AxisLimit m_AxisLimit{};
	CompareLimit m_ComparePosToLimit{};
	
public:
	PosMotion();
	PosMotion(const Point2f& pos, float time, const Point2f& limits, const AxisLimit& axisLimit);
	PosMotion(const Point2f& pos, const Scale2f& velocity, const Point2f& limits, const AxisLimit& axisLimit);

	void Start();
	
	int Update(float elapsedSec);
	Point2f GetPos() const;
	
	void Reset(const Point2f& pos, float time, const Point2f& limits, const AxisLimit& axisLimit);
	void Reset(const Point2f& pos, const Scale2f& velocity, const Point2f& limits, const AxisLimit& axisLimit);

private:
	void UpdateNone(float elapsedSec);
	int UpdateX(float elapsedSec);
	int UpdateY(float elapsedSec);
};

