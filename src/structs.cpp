#include "structs.h"

//-----------------------------------------------------------------
// Point2f Constructors
//-----------------------------------------------------------------
Point2f::Point2f( )
	:Point2f{ 0.0f, 0.0f }
{
}
Point2f::Point2f( float x, float y )
	:x{ x }, y{ y }
{
}

//-----------------------------------------------------------------
// Point2 Constructors
//-----------------------------------------------------------------
Point2::Point2()
	:Point2{ 0, 0 }
{
}
Point2::Point2(int x, int y)
	: x{ x }, y{ y }
{
}


//-----------------------------------------------------------------
// Scale2f Constructors
//-----------------------------------------------------------------
Scale2f::Scale2f()
	:Scale2f{ 0.0f, 0.0f }
{
}
Scale2f::Scale2f(float x, float y)
	: x{ x }, y{ y }
{
}

//-----------------------------------------------------------------
// Rectf Constructors
//-----------------------------------------------------------------
Rectf::Rectf( )
	:Rectf{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

Rectf::Rectf( float left, float bottom, float width, float height )
	: left{ left }
	, bottom{ bottom }
	, width{ width }
	, height{ height }
{
}

Rectf::Rectf(Point2 pos, float width, float height)
	: left{ float(pos.x) }
	, bottom{ float(pos.y) }
	, width{ width }
	, height{ height }
{
}

Rectf::Rectf(Point2f pos, float width, float height)
	:left{ pos.x }
	, bottom{ pos.y }
	, width{ width }
	, height{ height }
{
}

//-----------------------------------------------------------------
// Color4 Constructors
//-----------------------------------------------------------------
Color4::Color4()
	:Color4{ 0, 0, 0, 0 }
{
}

Color4::Color4(int r, int g, int b, int a)
	: r{ r }
	, g{ g }
	, b{ b }
	, a{ a }
{
}

//-----------------------------------------------------------------
// Circlef Constructors
//-----------------------------------------------------------------
Circlef::Circlef( )
	:Circlef{ 0.0f, 0.0f, 0.0f }
{
}

Circlef::Circlef( float centerX, float centerY, float radius )
	:Circlef{ Point2f{ centerX, centerY }, radius }
{
}

Circlef::Circlef( const Point2f& center, float radius )
	:center{ center }
	,radius{ radius }
{
}

//-----------------------------------------------------------------
// Ellipsef Constructors
//-----------------------------------------------------------------
Ellipsef::Ellipsef( )
	:Ellipsef{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}


Ellipsef::Ellipsef( const Point2f& center, float radiusX, float radiusY )
	: center{ center }
	, radiusX{ radiusX }
	, radiusY{ radiusY }
{
}

Ellipsef::Ellipsef(float centerX, float centerY, float radiusX, float radiusY)
	: Ellipsef{ Point2f{ centerX, centerY }, radiusX, radiusY }
{
}

//-----------------------------------------------------------------
// Linef Constructors
//-----------------------------------------------------------------
//
Linef::Linef()
	: p1{ 0, 0 }
	, p2{ 0, 0 }
{
}

Linef::Linef(float x1, float y1, float x2, float y2)
	: p1{ x1, y1 }
	, p2{ x2, y2 }
{
}

Linef::Linef(Point2f p1, Point2f p2)
	: p1{ p1 }
	, p2{ p2 }
{
}
