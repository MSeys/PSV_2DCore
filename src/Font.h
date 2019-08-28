#pragma once
#include "pch.h"

struct Color4;
struct Point2f;

enum TextLocation
{
	BOTTOM_LEFT, MID_LEFT, TOP_LEFT, BOTTOM_CENTER, MID_CENTER, TOP_CENTER, BOTTOM_RIGHT, MID_RIGHT, TOP_RIGHT 
};

class Font
{
	vita2d_font* m_pVitaFont;

public:
	Font(const std::string& fileName);

	Font(const Font& other) = delete;
	Font& operator=(const Font& rhs) = delete;
	Font(const Font&& other) = delete;
	Font& operator=(const Font&& rhs) = delete;

	~Font();

	void Draw(const Point2f& pos, const Color4& color, int size, const std::string& text, const TextLocation& location) const;
	void Draw(const Point2& pos, const Color4& color, int size, const std::string& text, const TextLocation& location) const;

	int GetWidth(const std::string& text, int textSize) const;
	int GetHeight(const std::string& text, int textSize) const;
};
