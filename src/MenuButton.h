#pragma once
#include "pch.h"
#include "Font.h"

class MenuButton
{
	std::string m_Text;
	Point2f m_Pos;

	bool m_IsSelected = false;


public:
	MenuButton(const Point2f& pos, std::string text);

	MenuButton(const MenuButton& other) = delete;
	MenuButton& operator=(const MenuButton& rhs) = delete;
	MenuButton(const MenuButton&& other) = delete;
	MenuButton& operator=(const MenuButton&& rhs) = delete;

	~MenuButton();

	void Draw() const;
	void SetSelected(bool value);
	float GetHeight() const;
};
