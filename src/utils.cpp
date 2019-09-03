#include "pch.h"

#include <algorithm>
#include <iostream>
#include <cmath>
#include "utils.h"
#include "Vector2f.h"

float GetTimeNow()
{
	return float(sceKernelGetProcessTimeWide()) / 1000000.f;
}

int GetRandomInteger(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

float GetRandomFloat(float min, float max)
{
	return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min + 1));
}

#pragma region DrawFunctionality
void DrawLine(float x1, float y1, float x2, float y2, float lineWidth, const Color4& color)
{
	DrawLine(Point2f{ x1, y1 }, Point2f{ x2, y2 }, lineWidth, color);
}

void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth, const Color4& color)
{
	const Point2f trueP1{ (PSV_Allowed ? GetTransformedPoint(p1) : p1) };
	const Point2f trueP2{ (PSV_Allowed ? GetTransformedPoint(p2) : p2) };
	
	if (PSV_Allowed)
	{
		lineWidth *= (PSV_Scales[PSV_CT].x + PSV_Scales[PSV_CT].y) / 2;
	}

	sceGxmSetFrontPointLineWidth(_vita2d_context, int(lineWidth));
	sceGxmSetBackPointLineWidth(_vita2d_context, int(lineWidth));

	vita2d_draw_line(trueP1.x, trueP1.y, trueP2.x, trueP2.y, RGBA8(color.r, color.g, color.b, color.a));
}

void DrawLine(const Linef& line, float lineWidth, const Color4& color)
{
	DrawLine(line.p1, line.p2, lineWidth, color);
}

void DrawRect(const Rectf& rect, float lineWidth, const Color4& color)
{
	Point2f bottomLeft { rect.left, rect.bottom },
				topLeft { rect.left, rect.bottom + rect.height },
				bottomRight { rect.left + rect.width, rect.bottom },
				topRight { rect.left + rect.width, rect.bottom + rect.height };

	if (PSV_Allowed)
	{
		lineWidth *= (PSV_Scales[PSV_CT].x + PSV_Scales[PSV_CT].y) / 2;
		bottomLeft = GetTransformedPoint(bottomLeft);
		topLeft = GetTransformedPoint(topLeft);
		bottomRight = GetTransformedPoint(bottomRight);
		topRight = GetTransformedPoint(topRight);
	}
	
	DrawLine(bottomLeft, topLeft, lineWidth, color);
	DrawLine(topLeft, topRight, lineWidth, color);
	DrawLine(topRight, bottomRight, lineWidth, color);
	DrawLine(bottomRight, bottomLeft, lineWidth, color);
}

void FillRect(const Rectf& rect, const Color4& color)
{
	Rectf trueRect{ rect };
	if(PSV_Allowed)
	{
		trueRect = GetTransformedRectangle(trueRect);
	}
	
	vita2d_draw_rectangle(trueRect.left, trueRect.bottom, trueRect.width, trueRect.height, RGBA8(color.r, color.g, color.b, color.a));
}

void FillCircle(float centerX, float centerY, float rad, const Color4& color)
{
	Point2f trueCenter{ centerX, centerY };

	if(PSV_Allowed)
	{
		trueCenter = GetTransformedPoint(trueCenter);
		rad *= (PSV_Scales[PSV_CT].x + PSV_Scales[PSV_CT].y) / 2;
	}
	
	vita2d_draw_fill_circle(trueCenter.x, trueCenter.y, rad, RGBA8(color.r, color.g, color.b, color.a));
}

void FillCircle(const Circlef& circle, const Color4& color)
{
	FillCircle(circle.center.x, circle.center.y, circle.radius, color);
}

void FillCircle(const Point2f& center, float rad, const Color4& color)
{
	FillCircle(center.x, center.y, rad, color);
}

void DrawLine(float x1, float y1, float x2, float y2, float lineWidth, const Color3& color, int transparency)
{
	DrawLine(x1, y1, x2, y2, lineWidth, Color4{ color, transparency });
}

void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth, const Color3& color, int transparency)
{
	DrawLine(p1, p2, lineWidth, Color4{ color, transparency });
}

void DrawLine(const Linef& line, float lineWidth, const Color3& color, int transparency)
{
	DrawLine(line, lineWidth, Color4{ color, transparency });
}

void DrawRect(const Rectf& rect, float lineWidth, const Color3& color, int transparency)
{
	DrawRect(rect, lineWidth, Color4{ color, transparency });
}

void FillRect(const Rectf& rect, const Color3& color, int transparency)
{
	FillRect(rect, Color4{ color, transparency });
}

void FillCircle(float centerX, float centerY, float rad, const Color3& color, int transparency)
{
	FillCircle(centerX, centerY, rad, Color4{ color, transparency });
}

void FillCircle(const Circlef& circle, const Color3& color, int transparency)
{
	FillCircle(circle, Color4{ color, transparency });
}

void FillCircle(const Point2f& center, float rad, const Color3& color, int transparency)
{
	FillCircle(center, rad, Color4{ color, transparency });
}
#pragma endregion OpenGLDrawFunctionality

#pragma region CollisionFunctionality
bool IsPointInRect(const Point2f& p, const Rectf& r)
{
	return (p.x >= r.left && p.x <= r.left + r.width && p.y >= r.bottom&& p.y <= r.bottom + r.height);
}

bool IsPointInCircle(const Point2f& p, const Circlef& c)
{
	float squaredDist = (p.x - c.center.x) * (p.x - c.center.x) + (p.y - c.center.y) * ( p.y - c.center.y );
	float squaredRadius = c.radius * c.radius;
	return (squaredRadius >= squaredDist);
}

bool IsOverlapping( const Point2f& a, const Point2f& b, const Rectf& r )
{
	// if one of the line segment end points is in the rect
	if ( IsPointInRect( a, r ) || IsPointInRect( b, r ) )
	{
		return true;
	}

	HitInfo hitInfo{};
	Point2f vertices[]{ Point2f {r.left, r.bottom},
		Point2f{ r.left + r.width, r.bottom },
		Point2f{ r.left + r.width, r.bottom + r.height },
		Point2f{ r.left, r.bottom + r.height } };

	return Raycast( vertices, 4, a, b, hitInfo );
}

bool IsOverlapping( const Rectf& r1, const Rectf& r2 )
{
	// If one rectangle is on left side of the other
	if ( ( r1.left + r1.width ) < r2.left || ( r2.left + r2.width ) < r1.left )
	{
		return false;
	}

	// If one rectangle is under the other
	if ( r1.bottom > ( r2.bottom + r2.height ) || r2.bottom > ( r1.bottom + r1.height ) )
	{
		return false;
	}

	return true;
}

bool IsOverlapping( const Rectf& r, const Circlef& c )
{
	// Is center of circle in the rectangle?
	if ( IsPointInRect( c.center, r ) )return true;

	// Check line segments
	if ( DistPointLineSegment( c.center, Point2f{ r.left, r.bottom }, Point2f{ r.left, r.bottom + r.height } ) <= c.radius ) return true;
	if ( DistPointLineSegment( c.center, Point2f{ r.left, r.bottom }, Point2f{ r.left + r.width, r.bottom } ) <= c.radius ) return true;
	if ( DistPointLineSegment( c.center, Point2f{ r.left + r.width, r.bottom + r.height }, Point2f{ r.left, r.bottom + r.height } ) <= c.radius ) return true;
	if ( DistPointLineSegment( c.center, Point2f{ r.left + r.width, r.bottom + r.height }, Point2f{ r.left + r.width, r.bottom } ) <= c.radius ) return true;
	return false;
}
bool IsOverlapping( const Circlef& c1, const Circlef& c2 )
{
	// squared distance between centers
	float xDistance{ c1.center.x - c2.center.x };
	float yDistance{ c1.center.y - c2.center.y };
	float squaredDistance = xDistance * xDistance + yDistance * yDistance;

	float squaredTouchingDistance = ( c1.radius + c2.radius ) * ( c1.radius + c2.radius );
	if ( squaredDistance < squaredTouchingDistance )
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsOverlapping( const Point2f& a, const Point2f& b, const Circlef& c )
{
	return DistPointLineSegment( c.center, a, b ) <= c.radius;
}

bool IsOverlapping( const std::vector<Point2f>& vertices, const Circlef& c )
{
	return IsOverlapping( vertices.data( ), vertices.size( ), c );
}

bool IsOverlapping( const Point2f* vertices, size_t nrVertices, const Circlef& c )
{
	// Check points in circle
	for ( size_t i{ 0 }; i < nrVertices; ++i )
	{
		if ( IsPointInCircle( vertices[i], c ) )
		{
			return true;
		}
	}

	// Check overlapping line segments with circle
	for ( size_t i{ 0 }; i < nrVertices; ++i )
	{
		if ( DistPointLineSegment( c.center, vertices[i], vertices[( i + 1 ) % nrVertices] ) <= c.radius )
		{
			return true;
		}
	}

	// No overlapping line segments, verify whether circle is inside polygon
	if ( IsPointInPolygon( c.center, vertices, nrVertices ) )
	{
		return true;
	}
	return false;
}

bool IsPointInPolygon( const Point2f& p, const std::vector<Point2f>& vertices )
{
	return IsPointInPolygon( p, vertices.data( ), vertices.size( ) );
}

bool IsPointInPolygon( const Point2f& p, const Point2f* vertices, size_t nrVertices )
{
	if ( nrVertices < 2 )
	{
		return false;
	}
	// 1. First do a simple test with axis aligned bounding box around the polygon
	float xMin{ vertices[0].x };
	float xMax{ vertices[0].x };
	float yMin{ vertices[0].y };
	float yMax{ vertices[0].y };
	for ( size_t idx{ 1 }; idx < nrVertices; ++idx )
	{
		if ( xMin > vertices[idx].x )
			xMin = vertices[idx].x;
		if ( xMax < vertices[idx].x )
			xMax = vertices[idx].x;
		if ( yMin > vertices[idx].y )
			yMin = vertices[idx].y;
		if ( yMax < vertices[idx].y )
			yMax = vertices[idx].y;
	}
	if ( p.x < xMin || p.x > xMax || p.y < yMin || p.y > yMax )
		return false;

	// 2. Draw a virtual ray from anywhere outside the polygon to the point 
	//    and count how often it hits any side of the polygon. 
	//    If the number of hits is even, it's outside of the polygon, if it's odd, it's inside.
	int numberOfIntersectionPoints{0};
	Point2f p2{ xMax + 10.0f, p.y }; // Horizontal line from point to point outside polygon (p2)

	// Count the number of intersection points
	float lambda1{}, lambda2{};
	for ( size_t i{ 0 }; i < nrVertices; ++i )
	{
		if ( IntersectLineSegments( vertices[i], vertices[( i + 1 ) % nrVertices], p, p2, lambda1, lambda2 ) )
		{
			if ( lambda1 > 0 && lambda1 <= 1 && lambda2 > 0 && lambda2 <= 1 )
			{
				++numberOfIntersectionPoints;
			}
		}
	}
	if ( numberOfIntersectionPoints % 2 == 0 )
		return false;
	else
		return true;
}

bool IntersectLineSegments( const Point2f& p1, const Point2f& p2, const Point2f& q1, const Point2f& q2, float& outLambda1, float& outLambda2, float epsilon )
{
	bool intersecting{ false };

	Vector2f p1p2{ p1, p2 };
	Vector2f q1q2{ q1, q2 };

	// cross product to determine if parallel
	float denom = p1p2.CrossProduct( q1q2 );

	// Don't divide by zero
	if ( std::abs( denom ) > epsilon )
	{
		intersecting = true;

		Vector2f p1q1{ p1, q1 };

		float num1 = p1q1.CrossProduct( q1q2 );
		float num2 = p1q1.CrossProduct( p1p2 );
		outLambda1 = num1 / denom;
		outLambda2 = num2 / denom;
	}
	else // are parallel
	{
		//connect start points
		Vector2f p1q1{ p1, q1 };

		// cross product to determine if segments and the line connecting their start points are parallel, 
		// if so, than they are on a line
		// if not, then there is no intersection
		float denom = p1q1.CrossProduct( q1q2 );
		if ( std::abs( denom ) > epsilon ) return false;

		// check the 4 conditions
		outLambda1 = 0;
		outLambda2 = 0;
		if ( IsPointOnLineSegment( p1, q1, q2 ) )intersecting = true;
		if ( IsPointOnLineSegment( p2, q1, q2 ) )intersecting = true;
		if ( IsPointOnLineSegment( q1, p1, p2 ) )intersecting = true;
		if ( IsPointOnLineSegment( q2, p1, p2 ) )intersecting = true;
	}
	return intersecting;
}

bool Raycast( const std::vector<Point2f>& vertices, const Point2f& rayP1, const Point2f& rayP2, HitInfo& hitInfo )
{
	return Raycast( vertices.data( ), vertices.size( ), rayP1, rayP2, hitInfo );
}

bool Raycast( const Point2f* vertices, const size_t nrVertices, const Point2f& rayP1, const Point2f& rayP2, HitInfo& hitInfo )
{
	if ( nrVertices == 0 )
	{
		return false;
	}

	std::vector<HitInfo> hits;

	Rectf r1, r2;
	// r1: minimal AABB rect enclosing the ray
	r1.left = std::min( rayP1.x, rayP2.x );
	r1.bottom = std::min( rayP1.y, rayP2.y );
	r1.width = std::max( rayP1.x, rayP2.x ) - r1.left;
	r1.height = std::max( rayP1.y, rayP2.y ) - r1.bottom;

	// Line-line intersections.
	for ( size_t idx{ 0 }; idx <= nrVertices; ++idx )
	{
		// Consider line segment between 2 consecutive vertices
		// (modulo to allow closed polygon, last - first vertice)
		Point2f q1 = vertices[( idx + 0 ) % nrVertices];
		Point2f q2 = vertices[( idx + 1 ) % nrVertices];

		// r2: minimal AABB rect enclosing the 2 vertices
		r2.left = std::min( q1.x, q2.x );
		r2.bottom = std::min( q1.y, q2.y );
		r2.width = std::max( q1.x, q2.x ) - r2.left;
		r2.height = std::max( q1.y, q2.y ) - r2.bottom;

		if ( IsOverlapping( r1, r2 ) )
		{
			float lambda1{};
			float lambda2{};
			if ( IntersectLineSegments( rayP1, rayP2, q1, q2, lambda1, lambda2 ) )
			{
				if ( lambda1 > 0&& lambda1 <= 1&& lambda2 > 0&& lambda2 <= 1 )
				{
					HitInfo hitInfo;
					hitInfo.lambda = lambda1;
					hitInfo.intersectPoint = Point2f{ rayP1.x + ( ( rayP2.x - rayP1.x ) * lambda1 ), rayP1.y + ( ( rayP2.y - rayP1.y ) * lambda1 ) };
					hitInfo.normal = Vector2f{ q2 - q1 }.Orthogonal( ).Normalized( );
					hits.push_back( hitInfo );
				}
			}
		}
	}

	if ( hits.size( ) == 0 )
	{
		return false;
	}

	// Get closest intersection point and copy it into the hitInfo parameter
	hitInfo = *std::min_element( hits.begin( ), hits.end( ),
		[]( const HitInfo& first, const HitInfo& last ) {
		return first.lambda < last.lambda; } );
	return true;
}

bool  IsPointOnLineSegment( const Point2f& p, const Point2f& a, const Point2f& b )
{
	Vector2f ap{ a, p }, bp{ b, p };
	// If not on same line, return false
	if ( abs( ap.CrossProduct( bp ) ) > 0.001f )
	{
		return false;
	}

	// Both vectors must point in opposite directions if p is between p1 and p2
	if ( ap.DotProduct( bp ) > 0 )
	{
		return false;
	}

	return true;
}

float  DistPointLineSegment( const Point2f& p, const Point2f& a, const Point2f& b )
{
	Vector2f ab{ a, b };
	Vector2f ap{ a, p };
	Vector2f abNorm = ab.Normalized( );
	float distToA = abNorm.DotProduct( ap );
	// if distToA is negative, then the closest point is A
	// return the distance a, p
	if ( distToA < 0 )
	{
		return ap.Length( );
	}
	// if distToA is > than dist(a,b) then the closest point is B
	// return the distance b, p
	float distAB = ab.Length( );
	if ( distToA > distAB )
	{
		return Vector2f{ b, p }.Length( );
	}

	//closest point is between A and B, calc intersection point
	Vector2f intersection = abNorm.DotProduct(ap) * abNorm + Vector2f{ a };
	return Vector2f{ p - intersection }.Length( );
}
#pragma endregion CollisionFunctionality

float DistanceBetweenPoints(const Point2f& p1, const Point2f& p2)
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

float AngleBetweenPoints(const Point2f& p1, const Point2f& p2)
{
	return atan2(p1.y - p2.y, p2.x - p1.x);
}

float DegreesToRadians(float degrees)
{
	return degrees * (float(M_PI) / 180.f);
}

int RadiansToDegrees(float rad)
{
	int angle{ int(rad * (180.f / float(M_PI))) };
	angle = (angle + 720) % 360;
	return angle;
}

bool AngleInRange(int degrees, float init, float end)
{
	return (float(degrees) >= init) && (float(degrees) < end);
}

#pragma region Transformation
Point2f GetTransformedPoint(float x, float y)
{
	const Point2f p
	{
		(x + PSV_Translations[PSV_CT].x) * PSV_Scales[PSV_CT].x,
		(y + PSV_Translations[PSV_CT].y) * PSV_Scales[PSV_CT].y
	};

	return p;
}

Point2f GetTransformedPoint(const Point2f& p)
{
	return GetTransformedPoint(p.x, p.y);
}

Rectf GetTransformedRectangle(float x, float y, float width, float height)
{
	return GetTransformedRectangle(Point2f{ x, y }, width, height);
}

Rectf GetTransformedRectangle(const Point2f& p, float width, float height)
{
	return Rectf{ GetTransformedPoint(p), width * PSV_Scales[PSV_CT].x, height * PSV_Scales[PSV_CT].y };
}

Rectf GetTransformedRectangle(const Rectf& r)
{
	return GetTransformedRectangle(r.left, r.bottom, r.width, r.height);
}

Circlef GetTransformedCircle(float centerX, float centerY, float rad)
{
	const Circlef transformedCircle
	{
		GetTransformedPoint(Point2f{ centerX, centerY }),
		rad * ((PSV_Scales[PSV_CT].x + PSV_Scales[PSV_CT].y) / 2)
	};
	
	return transformedCircle;
}

Circlef GetTransformedCircle(const Point2f& center, float rad)
{
	return GetTransformedCircle(center.x, center.y, rad);
}

Circlef GetTransformedCircle(const Circlef& circle)
{
	return GetTransformedCircle(circle.center, circle.radius);
}
#pragma endregion Transformation

std::vector<std::string> Split(const std::string& s, const std::string& delimits)
{
	std::vector<std::string> tmp;
	size_t p1, p2 = 0;
	for (; ; ) // loop forever ... until break
	{
		p1 = s.find_first_not_of(delimits, p2); // Note: p2 is 0 on first loop
		if (std::string::npos == p1)
			break; // i.e. if empty or all delimits
		p2 = s.find_first_of(delimits, p1 + 1);
		if (std::string::npos != p2) // i.e. if still more ... p2 is not past end
			tmp.push_back(s.substr(p1, p2 - p1));
		else
		{
			tmp.push_back(s.substr(p1));
			break;
		}
	}
	return tmp;
}