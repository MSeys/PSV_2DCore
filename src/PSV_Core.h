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

	Point2f pressTouch;
	Point2f motionTouch;
	Point2f releaseTouch;

	int reportNum;
	int touchVectorNum;
	
	float angleRad;
	int angleDegrees;

	float velocity;
	float length;

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
	SceCtrlButtons sceCtrlButton;
	PSV_ButtonType buttonType;

	bool isPressed;
	bool isHeld;
	bool isReleased;

	float press_time;

	PSV_Button(const SceCtrlButtons& sceCtrlButton, const PSV_ButtonType& buttonType);
	PSV_Event Update(SceCtrlData& pad);
};
#pragma endregion PSV Button

#pragma region PSV Joystick
struct PSV_Joystick
{
	PSV_JoystickType joyType;
	PSV_JoystickDirection joyDirection;

	int x;
	int y;

	PSV_Joystick(const PSV_JoystickType& joystickType);
	PSV_Event Update(SceCtrlData& pad);
};
#pragma endregion PSV Joystick

#pragma region PSV Touchpad
struct PSV_Touchpad
{
	PSV_TouchpadType touchpadType;
	int maxTouches;

	std::vector<Point2f> pressTouches;
	std::vector<Point2f> motionTouches;
	std::vector<Point2f> releaseTouches;
	std::vector<float> touchPressTimes;
	
	std::vector<bool> touchIsPressed;
	std::vector<bool> touchIsReleased;

	PSV_Touchpad(const PSV_TouchpadType& touchpadType);
	std::queue<PSV_Event> Update(SceTouchData* touch, SceTouchData* touchOld);

private:
	PSV_TouchpadSwipeDirection GetDirection(int angle);
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
extern std::vector<std::string> PSV_TouchSwipeDirectionStrings;
#pragma endregion PSV String Vectors

#pragma region PSV Functions
void PSV_Init();
void PSV_Update(SceCtrlData& pad, SceTouchData* touch, SceTouchData* touchOld);
int PSV_PollEvent(PSV_Event& e);
#pragma endregion PSV Functions