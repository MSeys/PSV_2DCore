#include "MenuButton.h"
#include <utility>

#include "Texture.h"

MenuButton::MenuButton(const Point2f& pos, std::string text)
	: m_Text(std::move(text))
	, m_Pos(pos)
{
}

MenuButton::~MenuButton()
{
}

void MenuButton::Draw() const
{
	const float offsetX{ 14.f };
	const float offsetY{ Bank::FindUI("button_sheet")->GetHeight() / 4 };
	Rectf srcRect{ 0, 0, 310, Bank::FindUI("button_sheet")->GetHeight() / 2 };
	
	if(m_IsSelected)
	{
		srcRect.bottom = srcRect.height;
	}

	else
	{
		srcRect.bottom = 0;
	}

	Bank::FindUI("button_sheet")->Draw(m_Pos, srcRect);
	Bank::FindFont("ArialRoundedMTBold")->Draw(Point2f{ m_Pos.x + offsetX, m_Pos.y + offsetY }, Color4{ 255, 255, 255, 255 }, 25, m_Text, MID_LEFT);
}

void MenuButton::SetSelected(bool value)
{
	m_IsSelected = value;
}

float MenuButton::GetHeight() const
{
	return Bank::FindUI("button_sheet")->GetHeight() / 2;
}
