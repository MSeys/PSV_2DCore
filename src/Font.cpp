#include "pch.h"

#include "Font.h"

Font::Font(const std::string& fileName)
	  : m_pVitaFont(vita2d_load_font_file(fileName.c_str()))
{
}

Font::~Font()
{
	vita2d_free_font(m_pVitaFont);
	m_pVitaFont = nullptr;
}

void Font::Draw(const Point2f& pos, const Color4& color, int size, const std::string& text, const TextLocation& location) const
{
	const int offset{ 3 };
	const int width{ GetWidth(text, size) };
	const int height{ GetHeight(text, size) };

	switch(location)
	{
	case BOTTOM_LEFT:
		vita2d_font_draw_text(m_pVitaFont, pos.x, pos.y, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
		break;

	case MID_LEFT:
		vita2d_font_draw_text(m_pVitaFont, pos.x, pos.y + height / 2.f - offset, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
		break;

	case TOP_LEFT:
		vita2d_font_draw_text(m_pVitaFont, pos.x, pos.y + height, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
		break;

	case BOTTOM_CENTER:
		vita2d_font_draw_text(m_pVitaFont, pos.x - width / 2.f, pos.y, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
		break;

	case MID_CENTER:
		vita2d_font_draw_text(m_pVitaFont, pos.x - width / 2.f, pos.y + height / 2.f - offset, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
		break;

	case TOP_CENTER:
		vita2d_font_draw_text(m_pVitaFont, pos.x - width / 2.f, pos.y + height, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
		break;

	case BOTTOM_RIGHT:
		vita2d_font_draw_text(m_pVitaFont, pos.x - width, pos.y, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
		break;

	case MID_RIGHT:
		vita2d_font_draw_text(m_pVitaFont, pos.x - width, pos.y + height / 2.f - offset, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
		break;

	case TOP_RIGHT:
		vita2d_font_draw_text(m_pVitaFont, pos.x - width, pos.y + height, RGBA8(color.r, color.g, color.b, color.a), size, text.c_str());
		break;
	}
}

void Font::Draw(const Point2& pos, const Color4& color, int size, const std::string& text, const TextLocation& location) const
{
	Draw(Point2f{ float(pos.x), float(pos.y) }, color, size, text, location);
}

int Font::GetWidth(const std::string& text, int textSize) const
{
	return vita2d_font_text_width(m_pVitaFont, textSize, text.c_str());
}

int Font::GetHeight(const std::string& text, int textSize) const
{
	return vita2d_font_text_height(m_pVitaFont, textSize, text.c_str());
}
