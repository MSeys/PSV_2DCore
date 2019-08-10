#include "pch.h"
#include "Vector2f.h"
#include <sstream>
#include <iomanip>
#include <cmath>
//-----------------------------------------------------------------
// Vector2f Constructors
//-----------------------------------------------------------------
Vector2f::Vector2f( )
	:Vector2f{ 0.0f, 0.0f }
{
}

Vector2f::Vector2f( float x, float y )
	: x{ x }
	, y{ y }
{
}

Vector2f::Vector2f( const Point2f& fromPoint, const Point2f& tillPoint )
	: Vector2f{ tillPoint.x - fromPoint.x, tillPoint.y - fromPoint.y }
{
}

Vector2f::Vector2f(const Point2f & point) 
	: Vector2f{ Point2f{ 0.0f, 0.0f }, point }
{
}

// -------------------------
// Methods
// -------------------------
bool Vector2f::Equals(const Vector2f& other, float epsilon) const
{
	return ( abs(x - other.x) < epsilon ) && ( abs(y - other.y) < epsilon );
}

Point2f Vector2f::ToPoint2f() const
{
	return Point2f{ x, y };
}

float Vector2f::DotProduct(const Vector2f& other) const
{
	return x * other.x + y * other.y;
}

float Vector2f::CrossProduct(const Vector2f& other) const
{
	return x * other.y - y * other.x;
}

std::string Vector2f::ToString() const
{
	std::stringstream buffer;

	buffer << std::fixed;
	buffer << std::setprecision( 2 );
	buffer << "Vector2f(" <<  x  << ", " <<  y  << ")";
	return buffer.str();
}

float Vector2f::Norm() const
{
	return Length();
}

float Vector2f::Length() const
{
	return sqrt( x * x + y * y );
}

float Vector2f::SquaredLength() const
{
	return x * x + y * y;
}

float Vector2f::AngleWith(const Vector2f& other) const
{
	//https://stackoverflow.com/questions/21483999/using-atan2-to-find-angle-between-two-vectors
	// not calling dot and cross functions to prevent the overhead
	// return atan2f(CrossProduct(other), DotProduct(other));
	return atan2(x * other.y - other.x * y, x * other.x + y * other.y);
}

Vector2f Vector2f::Normalized(float epsilon) const
{
	float length{ Length( ) };
	if ( length < epsilon )
	{
		return Vector2f{ 0, 0 };
	}
	else
	{
		return Vector2f{ x / length, y / length };
	}
}

Vector2f Vector2f::Orthogonal() const
{
	return Vector2f{ -y, x };
}

Vector2f Vector2f::Reflect( const Vector2f& surfaceNormal ) const
{
	return (*this) - 2 * ( this->DotProduct( surfaceNormal ) * surfaceNormal );
}

void Vector2f::Set(float newX, float newY)
{
	x = newX;
	y = newY;
}

// -------------------------
// Member operators
// -------------------------
Vector2f Vector2f::operator-( ) const
{
	return Vector2f{ -x, -y };
}
Vector2f Vector2f::operator+ ( ) const
{
	return Vector2f{ x, y };
}

Vector2f& Vector2f::operator*=(float rhs)
{
	x *= rhs;
	y *= rhs;
	return *this;
}

Vector2f& Vector2f::operator/=(float rhs)
{
	*this *= 1 / rhs;
	return *this;
}

Vector2f& Vector2f::operator+=(const Vector2f& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vector2f& Vector2f::operator-=(const Vector2f& rhs)
{
	*this += -rhs;
	return *this;
}

Vector2f::operator Point2f()
{
	return Point2f{ x,y };
}

// -------------------------
// Non-member operators
// -------------------------
Vector2f operator*( float lhs, Vector2f rhs )
{
	return rhs *= lhs;
}

Vector2f operator*( Vector2f lhs, float rhs )
{
	return lhs *= rhs;
}

Vector2f operator/( Vector2f lhs, float rhs )
{
	return lhs *= (1 / rhs);
}

Vector2f operator+( Vector2f lhs, const Vector2f& rhs )
{
	return lhs += rhs;
}

Vector2f operator-( Vector2f lhs, const Vector2f& rhs )
{
	return lhs += -rhs;
}

bool operator==( const Vector2f& lhs, const Vector2f& rhs )
{
	return ( lhs.Equals( rhs ) );
}

bool operator!=( const  Vector2f& lhs, const Vector2f& rhs )
{
	return !( lhs == rhs );
}

std::ostream& operator<< ( std::ostream& lhs, const Vector2f& rhs )
{
	lhs << rhs.ToString( );
	return lhs;
}

// Point2f related operators
Point2f& operator+=(Point2f& lhs, const Vector2f& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

Point2f operator+(Point2f lhs, const Vector2f& rhs)
{
	lhs += rhs;
	return lhs;
}

Point2f& operator-=(Point2f& lhs, const Vector2f& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

Point2f operator-(Point2f lhs, const Vector2f& rhs)
{
	lhs -= rhs;
	return lhs;
}


Vector2f operator-(const Point2f& lhs, const Point2f& rhs)
{
	Vector2f v{ lhs.x - rhs.x, lhs.y - rhs.y };
	return v;
}