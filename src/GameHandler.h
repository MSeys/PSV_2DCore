#pragma once
#include "pch.h"

class GameHandler
{
	
public:
	GameHandler();

	GameHandler(const GameHandler& other) = delete;
	GameHandler& operator=(const GameHandler& rhs) = delete;
	GameHandler(const GameHandler&& other) = delete;
	GameHandler& operator=(const GameHandler&& rhs) = delete;

	~GameHandler();

	void Update(float elapsedSec);
	void Draw() const;

	void ProcessKeyUpEvent(const PSV_ButtonEvent& bEvent);
	void ProcessKeyDownEvent(const PSV_ButtonEvent& bEvent);
	void ProcessKeyHeldEvent(const PSV_ButtonEvent& bEvent);

	void ProcessJoystickMotionEvent(const PSV_JoystickEvent& jEvent);

	void ProcessTouchpadDownEvent(const PSV_TouchpadEvent& tpEvent);
	void ProcessTouchpadUpEvent(const PSV_TouchpadEvent& tpEvent);
	void ProcessTouchpadMotionEvent(const PSV_TouchpadEvent& tpEvent);
	void ProcessTouchpadSwipeEvent(const PSV_TouchpadEvent& tpEvent);
	
private:
	void Initialize();
	void Cleanup();
};