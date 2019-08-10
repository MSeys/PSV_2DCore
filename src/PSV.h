#pragma once
#include "pch.h"
#include <queue>
#include <unordered_map>

#pragma region Enums
enum class PSV_ButtonType
{
	CROSS, CIRCLE, SQUARE, TRIANGLE, LTRIGGER, RTRIGGER, UP, DOWN, LEFT, RIGHT, START, SELECT
};

enum class PSV_JoystickType
{
	LSTICK, RSTICK
};

enum class PSV_JoystickDirection
{
	UP_LEFT, UP, UP_RIGHT, LEFT, RIGHT, DOWN_LEFT, DOWN, DOWN_RIGHT, MIDDLE
};

enum class PSV_TouchpadType
{
	FRONT, BACK
};

enum class PSV_TouchpadSwipeDirection
{
	UP, DOWN, LEFT, RIGHT
};

enum PSV_EventType
{
	PSV_NONE, PSV_KEYDOWN, PSV_KEYUP, PSV_KEYHELD, PSV_JOYSTICKMOTION, PSV_TOUCHPAD_DOWN, PSV_TOUCHPAD_UP, PSV_TOUCHPAD_MOTION, PSV_TOUCHPAD_SWIPE
};
#pragma endregion Enums

#pragma region Events
struct PSV_ButtonEvent
{
	PSV_ButtonType buttonType;
};

struct PSV_JoystickEvent
{
	PSV_JoystickType joyType;
	PSV_JoystickDirection joyDirection;
	float xValue;
	float yValue;
};

struct PSV_TouchpadEvent
{
	PSV_TouchpadType touchpadType;

	Point2 startTouch;
	Point2 endTouch;

	int touchNum;

	PSV_TouchpadSwipeDirection touchpadSwipeDirection;
};

struct PSV_Event
{
	PSV_EventType eType;
	
	PSV_ButtonEvent bEvent;
	PSV_JoystickEvent jEvent;
	PSV_TouchpadEvent tpEvent;
};
#pragma endregion Events

#pragma region PSV Button
struct PSV_Button
{
private:
	SceCtrlButtons sceCtrlButton;
	PSV_ButtonType buttonType;

	bool isPressed;
	bool isHeld;
	bool isReleased;

	float press_time;

public:
	PSV_Button(const SceCtrlButtons& sceCtrlButton, const PSV_ButtonType& buttonType);
	PSV_Event Update(SceCtrlData& pad);
	bool IsPressed() const;
	bool IsHeld() const;
	bool IsReleased() const;
};
#pragma endregion PSV Button

#pragma region PSV Joystick
struct PSV_Joystick
{
private:
	PSV_JoystickType joyType;
	PSV_JoystickDirection joyDirection;

public:
	PSV_Joystick(const PSV_JoystickType& joystickType);
	PSV_Event Update(SceCtrlData& pad);
};
#pragma endregion PSV Joystick

#pragma region PSV Touchpad
struct PSV_Touchpad
{
private:
	PSV_TouchpadType touchpadType;

	bool isPressed;
	bool isReleased;

	float press_time;
	
	Point2 startTouch;
	Point2 endTouch;
	
public:
	PSV_Touchpad(const PSV_TouchpadType& touchpadType);
	PSV_Event Update(SceTouchData* touch, SceTouchData* touchOld);
};
#pragma endregion PSV Touchpad

extern std::unordered_map<PSV_ButtonType, PSV_Button> PSV_Buttons;
extern std::unordered_map<PSV_JoystickType, PSV_Joystick> PSV_Joysticks;
extern std::unordered_map<PSV_TouchpadType, PSV_Touchpad> PSV_Touchpads;
extern std::queue<PSV_Event> PSV_EventsQueue;

#pragma region PSV String Vectors
extern std::vector<std::string> PSV_ButtonStrings;
extern std::vector<std::string> PSV_JoystickDirectionStrings;
extern std::vector<std::string> PSV_JoystickTypeStrings;
#pragma endregion PSV String Vectors

#pragma region PSV Functions
void PSV_Init();
void PSV_Update(SceCtrlData& pad, SceTouchData* touch, SceTouchData* touchOld);
int PSV_PollEvent(PSV_Event& e);
#pragma endregion PSV Functions