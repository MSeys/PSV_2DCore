#include "GameHandler.h"

// Necessary includes if you use the Bank (specified object)
#include "Texture.h" // UI or Game Asset
#include "Font.h" // Font
#include "SFX.h" // SFX
#include "Music.h" // Music

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
	
}

void GameHandler::ProcessKeyUpEvent(const PSV_ButtonEvent& bEvent)
{
}

void GameHandler::ProcessKeyDownEvent(const PSV_ButtonEvent& bEvent)
{
	
}

void GameHandler::ProcessKeyHeldEvent(const PSV_ButtonEvent& bEvent)
{
}

void GameHandler::ProcessJoystickMotionEvent(const PSV_JoystickEvent& jEvent)
{
	
}

void GameHandler::ProcessTouchpadDownEvent(const PSV_TouchpadEvent& tpEvent)
{
	
}

void GameHandler::ProcessTouchpadUpEvent(const PSV_TouchpadEvent& tpEvent)
{
	
}

void GameHandler::ProcessTouchpadMotionEvent(const PSV_TouchpadEvent& tpEvent)
{
	
}

void GameHandler::ProcessTouchpadSwipeEvent(const PSV_TouchpadEvent& tpEvent)
{
	
}

void GameHandler::Initialize()
{
	
}

void GameHandler::Cleanup()
{
}
