#pragma once
#include "pch.h"
#include <queue>
#include <unordered_map>

#pragma region Enums
enum PSV_ButtonType
{
	CROSS, CIRCLE, SQUARE, TRIANGLE, LTRIGGER, RTRIGGER, DPAD_UP, DPAD_DOWN, DPAD_LEFT, DPAD_RIGHT, START, SELECT
};

enum PSV_JoystickType
{
	LSTICK, RSTICK
};

enum PSV_JoystickDirection
{
	NW, N, NE, W, E, SW, S, SE, MIDDLE
};

enum PSV_TouchpadType
{
	FRONT, BACK
};

enum PSV_TouchpadSwipeDirection
{
	SWIPE_UP, SWIPE_DOWN, SWIPE_LEFT, SWIPE_RIGHT
};

enum PSV_EventType
{
	PSV_NONE, PSV_KEYDOWN, PSV_KEYUP, PSV_KEYHELD, PSV_JOYSTICKMOTION, PSV_TOUCHPAD_DOWN, PSV_TOUCHPAD_UP, PSV_TOUCHPAD_MOTION, PSV_TOUCHPAD_SWIPE
};

enum PSV_TouchSamplingMode
{
	PSV_TOUCH_MOTION, PSV_TOUCH_SWIPE
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

	int angleDegrees;
	float angleRad;
};

struct PSV_TouchpadEvent
{
	PSV_TouchpadType touchpadType;

	Point2f startTouch;
	Point2f endTouch;

	int touchNum;
	float angleRad;
	int angleDegrees;

	float velocity;

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

	float press_time{};
	
	bool isPressed{};
	bool isReleased{};
	
	Point2f startTouch;
	Point2f endTouch;
	
public:
	PSV_Touchpad(const PSV_TouchpadType& touchpadType);
	PSV_Event Update(SceTouchData* touch, SceTouchData* touchOld);

private:
	PSV_TouchpadSwipeDirection GetDirection(int angle);
};
#pragma endregion PSV Touchpad

extern std::unordered_map<PSV_ButtonType, PSV_Button> PSV_Buttons;
extern std::unordered_map<PSV_JoystickType, PSV_Joystick> PSV_Joysticks;
extern std::unordered_map<PSV_TouchpadType, PSV_Touchpad> PSV_Touchpads;
extern std::queue<PSV_Event> PSV_EventsQueue;
extern PSV_TouchSamplingMode PSV_TSMode;

#pragma region PSV String Vectors
extern std::vector<std::string> PSV_ButtonStrings;
extern std::vector<std::string> PSV_JoystickDirectionStrings;
extern std::vector<std::string> PSV_JoystickTypeStrings;
#pragma endregion PSV String Vectors

#pragma region PSV Functions
void PSV_Init();
void PSV_Update(SceCtrlData& pad, SceTouchData* touch, SceTouchData* touchOld);
int PSV_PollEvent(PSV_Event& e);
void PSV_SetTouchSamplingMode(const PSV_TouchSamplingMode& psvTouchSamplingMode);
#pragma endregion PSV Functions