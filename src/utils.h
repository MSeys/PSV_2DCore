#pragma once

#include "Vector2f.h"
#include <vector>
#include "Colors.h"

	float GetTimeNow();
	int GetRandomInteger(int min, int max);
	float GetRandomFloat(float min, float max);

#pragma region DrawFunctionality	
	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth, const Color4& color);
	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth = 1.0f, const Color3& color = C3_WHITE, int transparency = 255);

	void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth, const Color4& color);
	void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth = 1.f, const Color3& color = C3_WHITE, int transparency = 255);

	void DrawLine(const Linef& line, float lineWidth, const Color4& color);
	void DrawLine(const Linef& line, float lineWidth = 1.0f, const Color3& color = C3_WHITE, int transparency = 255);

	void DrawRect(const Rectf& rect, float lineWidth, const Color4& color);
	void DrawRect(const Rectf& rect, float lineWidth = 1.0f, const Color3& color = C3_WHITE, int transparency = 255);

	void FillRect(const Rectf& rect, const Color4& color);
	void FillRect(const Rectf& rect, const Color3& color = C3_WHITE, int transparency = 255);

	void FillCircle(float centerX, float centerY, float rad, const Color4& color);
	void FillCircle(float centerX, float centerY, float rad, const Color3& color = C3_WHITE, int transparency = 255);
	void FillCircle(const Circlef& circle, const Color4& color);
	void FillCircle(const Circlef& circle, const Color3& color = C3_WHITE, int transparency = 255);
	void FillCircle(const Point2f& center, float rad, const Color4& color);
	void FillCircle(const Point2f& center, float rad, const Color3& color = C3_WHITE, int transparency = 255);
#pragma endregion DrawFunctionality

#pragma region CollisionFunctionality
	struct HitInfo
	{
		float lambda;
		Point2f intersectPoint;
		Vector2f normal;
	};

	bool IsPointInRect(const Point2f& p, const Rectf& r);

	bool IsPointInCircle(const Point2f& p, const Circlef& c);

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

#pragma region Transformation
	Point2f GetTransformedPoint(float x, float y);
	Point2f GetTransformedPoint(const Point2f& p);

	Rectf GetTransformedRectangle(float x, float y, float width, float height);
	Rectf GetTransformedRectangle(const Point2f& p, float width, float height);
	Rectf GetTransformedRectangle(const Rectf& r);

	Circlef GetTransformedCircle(float centerX, float centerY, float rad);
	Circlef GetTransformedCircle(const Point2f& center, float rad);
	Circlef GetTransformedCircle(const Circlef& circle);
#pragma endregion Transformation

	std::vector<std::string> Split(const std::string& s, const std::string& delimits = " \t");