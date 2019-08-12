#pragma once

#include "Vector2f.h"
#include <vector>

	float GetTimeNow();
#pragma region DrawFunctionality	
	void DrawPoint(float x, float y, const Color4& color, float pointSize = 1.0f);
	void DrawPoint(const Point2f& p, const Color4& color, float pointSize = 1.0f);

	void DrawLine(float x1, float y1, float x2, float y2, const Color4& color = Color4{ 255, 255, 255, 255 }, float lineWidth = 1.0f);
	void DrawLine(const Point2f& p1, const Point2f& p2, const Color4& color = Color4{ 255, 255, 255, 255 }, float lineWidth = 1.0f);
	void DrawLine(const Point2& p1, const Point2& p2, const Color4& color = Color4{ 255, 255, 255, 255 }, float lineWidth = 1.0f);

	void DrawRect(const Rectf& rect, const Color4& color = Color4{ 255, 255, 255, 255 }, float lineWidth = 1.0f);
	void FillRect(const Rectf& rect, const Color4& color = Color4{ 255, 255, 255, 255 });

	void FillCircle(float centerX, float centerY, float rad, const Color4& color = Color4{ 255, 255, 255, 255 });
	void FillCircle(const Circlef& circle, const Color4& color = Color4{ 255, 255, 255, 255 });
	void FillCircle(const Point2f& center, float rad, const Color4& color = Color4{ 255, 255, 255, 255 });
#pragma endregion OpenGLDrawFunctionality

#pragma region CollisionFunctionality
	struct HitInfo
	{
		float lambda;
		Point2f intersectPoint;
		Vector2f normal;
	};

	bool IsPointInRect(const Point2f& p, const Rectf& r);

	bool IsPointInCircle(const Point2f& p, const Circlef& c);
	bool IsPointInCircle(const Point2& p, const Circlef& c);

	bool IsPointInPolygon( const Point2f& p, const std::vector<Point2f>& vertices );
	bool IsPointInPolygon( const Point2f& p, const Point2f* vertices, size_t nrVertices );

	bool IsOverlapping( const Point2f& a, const Point2f& b, const Circlef& c );
	bool IsOverlapping( const Point2f& a, const Point2f& b, const Rectf& r );

	bool IsOverlapping(const Rectf & r1, const Rectf & r2);
	bool IsOverlapping( const Rectf& r, const Circlef& c );
	bool IsOverlapping( const Circlef& c1, const Circlef& c2 );
	bool IsOverlapping( const std::vector<Point2f>& vertices, const Circlef& c );
	bool IsOverlapping( const Point2f* vertices, size_t nrVertices, const Circlef& c );

	bool Raycast( const Point2f* vertices, const size_t nrVertices, const Point2f& rayP1, const Point2f& rayP2, HitInfo& hitInfo );
	bool Raycast( const std::vector<Point2f>& vertices, const Point2f& rayP1, const Point2f& rayP2, HitInfo& hitInfo );

	bool IntersectLineSegments(const Point2f& p1, const Point2f& p2, const Point2f& q1, const Point2f& q2, float& outLambda1, float& outLambda2, float epsilon = 1e-6);
	float DistPointLineSegment(const Point2f& p, const Point2f& a, const Point2f& b);
	bool IsPointOnLineSegment(const Point2f& p, const Point2f& a, const Point2f& b);
#pragma endregion CollisionFunctionality

	float DistanceBetweenPoints(const Point2f& p1, const Point2f& p2);

	float AngleBetweenPoints(const Point2f& p1, const Point2f& p2);
	float DegreesToRadians(float degrees);
	int RadiansToDegrees(float rad);
	bool AngleInRange(int degrees, float init, float end);