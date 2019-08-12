#pragma once
#include <string>
#include <vector>
#include <psp2/ctrl.h>

struct Point2f
{
	Point2f( );
	explicit Point2f( float x, float y );

	float x;
	float y;
};

struct Point2
{
	Point2();
	explicit Point2(int x, int y);

	int x;
	int y;
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
	explicit Rectf(Point2 pos, float width, float height);
	explicit Rectf(Point2f pos, float width, float height);

	float left;
	float bottom;
	float width;
	float height;

};

struct Color4
{
	Color4();
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