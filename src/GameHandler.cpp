#include "GameHandler.h"
#include "Texture.h"
#include "Font.h"

GameHandler::GameHandler()
{
	Initialize();
}

GameHandler::~GameHandler()
{
	Cleanup();
}

void GameHandler::Update(float elapsedSec)
{
}

void GameHandler::Draw() const
{
	Bank::FindUI("background")->Draw();
	Bank::FindUI("topbar")->Draw();

	Bank::FindFont("BerlinSansFBDemi")->Draw(Point2{ 100, 100 }, Color4{ 255, 255, 255, 255 }, 30, m_KeyboardValue, MID_CENTER);

	FillCircle(m_Circle);
}

void GameHandler::ProcessKeyUpEvent(const PSV_ButtonEvent& bEvent)
{
}

void GameHandler::ProcessKeyDownEvent(const PSV_ButtonEvent& bEvent)
{
	if(bEvent.buttonType == PSV_ButtonType::START)
	{
		m_KeyboardValue = VitaKeyboardGet("Title", m_KeyboardValue, 600, 0);
	}
}

void GameHandler::ProcessKeyHeldEvent(const PSV_ButtonEvent& bEvent)
{
}

void GameHandler::ProcessJoystickMotionEvent(const PSV_JoystickEvent& jEvent)
{
}

void GameHandler::ProcessTouchpadDownEvent(const PSV_TouchpadEvent& tpEvent)
{
	if(IsPointInCircle(tpEvent.startTouch, m_Circle))
	{
		m_MoveCircle = true;
	}
}

void GameHandler::ProcessTouchpadUpEvent(const PSV_TouchpadEvent& tpEvent)
{
	m_MoveCircle = false;
}

void GameHandler::ProcessTouchpadMotionEvent(const PSV_TouchpadEvent& tpEvent)
{
	if (m_MoveCircle)
	{
		m_Circle.center = Point2f{ float(tpEvent.endTouch.x), float(tpEvent.endTouch.y) };
	}
}

void GameHandler::ProcessTouchpadSwipeEvent(const PSV_TouchpadEvent& tpEvent)
{
}

void GameHandler::Initialize()
{
	m_Circle.center = Point2f{ 100, 100 };
	m_Circle.radius = 30;
}

void GameHandler::Cleanup()
{
}
