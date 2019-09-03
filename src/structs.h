#pragma once
#include <string>

struct Point2f
{
	Point2f( );
	explicit Point2f( float x, float y );

	float x;
	float y;
};

struct Scale2f
{
	Scale2f();
	explicit Scale2f(float x, float y);

	float x;
	float y;
};

struct Rectf
{
	Rectf( );
	explicit Rectf(float left, float bottom, float width, float height);
	explicit Rectf(Point2f pos, float width, float height);

	float left;
	float bottom;
	float width;
	float height;

};

struct Color3
{
	Color3();
	explicit Color3(int r, int g, int b);

	int r;
	int g;
	int b;
};

struct Color4
{
	Color4();
	explicit Color4(const Color3& color, int a);
	explicit Color4(int r, int g, int b, int a);

	int r;
	int g;
	int b;
	int a;
};

struct Circlef
{
	Circlef( );
	explicit Circlef( const Point2f& center, float radius );
	explicit Circlef( float centerX, float centerY, float radius );

	Point2f center;
	float radius;
};


struct Ellipsef
{
	Ellipsef( );
	explicit Ellipsef( const Point2f& center, float radiusX, float radiusY );
	explicit Ellipsef( float centerX, float centerY, float radiusX, float radiusY );

	Point2f center;
	float radiusX;
	float radiusY;
};

struct Linef
{
	Linef();
	explicit Linef(float x1, float y1, float x2, float y2);
	explicit Linef(Point2f p1, Point2f p2);

	Point2f p1;
	Point2f p2;
};