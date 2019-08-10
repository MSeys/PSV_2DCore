#pragma once
#include <iostream>
#include <string>
#include "structs.h"

struct Vector2f
{
	// -------------------------
	// Constructors 
	// -------------------------
	Vector2f( );
	explicit Vector2f( float x, float y );
	explicit Vector2f( const Point2f& fromPoint, const Point2f& tillPoint );
	explicit Vector2f( const Point2f& point );

	// -------------------------
	// Member operators
	// -------------------------
	Vector2f operator-( ) const;
	Vector2f operator+( ) const;
	Vector2f& operator*=( float rhs);
	Vector2f& operator/=( float rhs);
	Vector2f& operator+=( const Vector2f& rhs);
	Vector2f& operator-=( const Vector2f& rhs);
	explicit operator Point2f(); 

	// -------------------------
	// Methods
	// -------------------------
	// Convert to Point2f	
	Point2f	ToPoint2f( ) const;

	// Are two vectors equal within a threshold?				
	// u.Equals(v)
	bool Equals( const Vector2f& other, float epsilon = 0.001f ) const;

	// Convert to String 
	std::string	ToString( ) const;

	// DotProduct
	// float d = u.DotProduct(v);
	float DotProduct( const Vector2f& other ) const;

	// CrossProduct 
	// float d = u.CrossProduct(v);
	float CrossProduct( const Vector2f& other ) const;
	
	// Norm of a vector 
	// float l = v.Norm();
	float Norm( ) const;

	// Length of a vector: 
	// float l = v.Length();
	float Length( ) const;

	// Square Length of a vector.
	// Faster alternative for Length, sqrt is not executed. 
	float SquaredLength( ) const;
	
	// AngleWith returns the smallest angle with another vector within the range [-PI/2, PI/2]. 
	// A pos angle is counter clockwise from this to the other
	// float angle = u.AngleWith(v);
	float AngleWith( const Vector2f& other ) const;


	// Returns normalized form of a vector
	// Vector2f n = v.Normalized();
	Vector2f Normalized( float epsilon = 0.001f ) const;

	// Returns the orthogonal of the Vector2f
	// Vector2f w = v.Orthogonal();
	Vector2f Orthogonal( ) const;

	// Returns a vector that is the reflection of the Vector2f
	// surfaceNormal: represents the normal of the surface at the reflection point
	Vector2f Reflect( const Vector2f& surfaceNormal ) const;

	// Sets the values of x and y
	void Set( float newX, float newY );

	// -------------------------
	// Datamembers 
	// -------------------------
	float x;
	float y;
};
// -------------------------
// Non member operators
// -------------------------
Vector2f operator*( float lhs, Vector2f rhs );
Vector2f operator*( Vector2f lhs, float rhs );
Vector2f operator/( Vector2f lhs, float rhs );

Vector2f operator+( Vector2f lhs, const Vector2f& rhs );
Vector2f operator-( Vector2f lhs, const Vector2f& rhs );

bool operator==( const Vector2f& lhs, const Vector2f& rhs );
bool operator!=( const  Vector2f& lhs, const Vector2f& rhs );

std::ostream& operator<< ( std::ostream& lhs, const Vector2f& rhs );

// Translating a point by a vector
Point2f& operator+=(Point2f& lhs, const Vector2f& rhs);
Point2f operator+(Point2f lhs, const Vector2f& rhs);

Point2f& operator-=(Point2f& lhs, const Vector2f& rhs);
Point2f operator-(Point2f lhs, const Vector2f& rhs);

// The difference vector between 2 points
Vector2f operator-( const Point2f& lhs, const Point2f& rhs);

