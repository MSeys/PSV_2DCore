#include "Font.h"
#include <sstream>
#include <regex>

enum TextType
{
	Text, Icon
};

struct TextIcon
{
	TextType textType;
	std::string text;
	Point2f pos;
	float width;
	float scale;
};

Font::Font(const std::string& fileName)
{
	for(int i{ 0 }; i <= 100; i++)
	{
		m_pVitaFonts.push_back(vita2d_load_font_file(fileName.c_str()));
	}
}

Font::~Font()
{
	for (vita2d_font* pFont : m_pVitaFonts)
	{
		vita2d_free_font(pFont);
		pFont = nullptr;
	}

	m_pVitaFonts.clear();
}

void Font::Draw(const Point2f& pos, int size, const std::string& text, const TextLocation& location, const Color4& color) const
{
	std::vector<std::string> compText = Split(text, "<>");
	Point2f normPos{ pos };
	Point2f transPos{ pos };
	const int normSize{ size };
	float centerOffset{ 3.f };
	float textOffsetX;
	float textOffsetY;
	float iconOffsetX;
	
	if (PSV_Allowed)
	{
		transPos = GetTransformedPoint(pos);
		size *= (PSV_Scales[PSV_CT].x + PSV_Scales[PSV_CT].y) / 2;
		centerOffset *= PSV_Scales[PSV_CT].y;
	}

	float width{};
	float normWidth{};
	const float height{ float(GetHeight(text, size)) };

	// set width
	for (const std::string& splitText : compText)
	{
		// is an icon
		if (Bank::FindIcon(splitText))
		{
			const float iconScale{ float(size) / Bank::FindIcon(splitText)->GetWidth() };
			const float normIconScale{ float(normSize) / Bank::FindIcon(splitText)->GetWidth() };
			width += Bank::FindIcon(splitText)->GetWidth() * iconScale;
			normWidth += Bank::FindIcon(splitText)->GetWidth() * normIconScale;
		}

		// is text
		else
		{
			width += float(GetWidth(splitText, size));
			normWidth += float(GetWidth(splitText, normSize));
		}
	}
	
	// set offsets
	switch (location)
	{
	case BOTTOM_LEFT:
		textOffsetX = 0;
		textOffsetY = 0;
		iconOffsetX = 0;
		break;

	case MID_LEFT:
		textOffsetX = 0;
		textOffsetY = height / 2.f - centerOffset;
		iconOffsetX = 0;
		break;

	case TOP_LEFT:
		textOffsetX = 0;
		textOffsetY = height;
		iconOffsetX = 0;
		break;

	case BOTTOM_CENTER:
		textOffsetX = -width / 2.f;
		textOffsetY = 0;
		iconOffsetX = -normWidth / 2.f;
		break;

	case MID_CENTER:
		textOffsetX = -width / 2.f;
		textOffsetY = height / 2.f - centerOffset;
		iconOffsetX = -normWidth / 2.f;
		break;

	case TOP_CENTER:
		textOffsetX = -width / 2.f;
		textOffsetY = height;
		iconOffsetX = -normWidth / 2.f;
		break;

	case BOTTOM_RIGHT:
		textOffsetX = -width;
		textOffsetY = 0;
		iconOffsetX = -normWidth;
		break;

	case MID_RIGHT:
		textOffsetX = -width;
		textOffsetY = height / 2.f - centerOffset;
		iconOffsetX = -normWidth;
		break;

	case TOP_RIGHT:
		textOffsetX = -width;
		textOffsetY = height;
		iconOffsetX = -normWidth;
		break;
	}

	for (const std::string& splitText : compText)
	{
		// is an icon
		if (Bank::FindIcon(splitText))
		{
			const float iconScale{ float(size) / Bank::FindIcon(splitText)->GetWidth() };
			const float normIconScale{ float(normSize) / Bank::FindIcon(splitText)->GetWidth() };
			const float iconHeight{ Bank::FindIcon(splitText)->GetHeight() * normIconScale };
			float iconOffsetY{};

			switch (location)
			{
			case BOTTOM_LEFT:
			case BOTTOM_CENTER:
			case BOTTOM_RIGHT:
				iconOffsetY = -iconHeight + centerOffset;
				break;

			case MID_LEFT:
			case MID_CENTER:
			case MID_RIGHT:
				iconOffsetY = -(iconHeight / 2.f - centerOffset);
				break;

			case TOP_LEFT:
			case TOP_CENTER:
			case TOP_RIGHT:
				iconOffsetY = centerOffset;
				break;
			}


			Bank::FindIcon(splitText)->Draw(Point2f{ normPos.x + iconOffsetX, normPos.y + iconOffsetY + (size / 10) }, Rectf{ 0, 0, 0, 0 }, Scale2f{ normIconScale, normIconScale });
			normPos.x += Bank::FindIcon(splitText)->GetWidth() * normIconScale;
			transPos.x += Bank::FindIcon(splitText)->GetWidth() * iconScale;
		}

		// is text
		else
		{
			vita2d_font_draw_text(m_pVitaFonts[size],
				int(transPos.x + textOffsetX), int(transPos.y + textOffsetY),
				RGBA8(color.r, color.g, color.b, color.a),
				size, splitText.c_str());
			normPos.x += GetWidth(splitText, normSize);
			transPos.x += GetWidth(splitText, size);
		}
	}
}

void Font::Draw(const Point2f& pos, int size, const std::string& text, const TextLocation& location,
	const Color3& color, int transparency) const
{
	Draw(pos, size, text, location, Color4{ color, transparency });
}

int Font::GetWidth(const std::string& text, int textSize) const
{
	return vita2d_font_text_width(m_pVitaFonts[textSize], textSize, text.c_str());
}

int Font::GetHeight(const std::string& text, int textSize) const
{
	return vita2d_font_text_height(m_pVitaFonts[textSize], textSize, text.c_str());
}
