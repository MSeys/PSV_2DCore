#include "PSV.h"
#define PSV_HOLD_TIME 0.4 
#define PSV_JOY_THRESHOLD 40

std::vector<std::string> PSV_ButtonStrings
{
	"Cross", "Circle", "Square", "Triangle",
	"L-Trigger", "R-Trigger",
	"D-Pad Up", "D-Pad Down", "D-Pad Left", "D-Pad Right",
	"Start", "Select"
};

std::vector<std::string> PSV_JoystickDirectionStrings
{
	"North West", "North", "North East", "West", "East", "South West", "South", "South East", "Middle"
};

std::vector<std::string> PSV_JoystickTypeStrings
{
	"L-Stick", "R-Stick"
};

std::unordered_map<PSV_ButtonType, PSV_Button> PSV_Buttons;
std::unordered_map<PSV_JoystickType, PSV_Joystick> PSV_Joysticks;
std::unordered_map<PSV_TouchpadType, PSV_Touchpad> PSV_Touchpads;
std::queue<PSV_Event> PSV_EventsQueue;
PSV_TouchSamplingMode PSV_TSMode{ PSV_TOUCH_MOTION };




#pragma region PSV Button
PSV_Button::PSV_Button(const SceCtrlButtons& sceCtrlButton, const PSV_ButtonType& buttonType)
	: sceCtrlButton(sceCtrlButton)
	, buttonType(buttonType)
	, isPressed(false)
	, isHeld(false)
	, isReleased(false)
	, press_time(0)
{
}

PSV_Event PSV_Button::Update(SceCtrlData& pad)
{
	PSV_Event e{};
	e.bEvent = PSV_ButtonEvent{ buttonType };

	const float timeNow = GetTimeNow();

	isReleased = false;
	if(pad.buttons & sceCtrlButton)
	{
		isReleased = false;

		if(isPressed && !isHeld && (timeNow - press_time > PSV_HOLD_TIME))
		{
			isHeld = true;
			isPressed = false;

			e.eType = PSV_KEYHELD;
			return e;
		}

		if(!isPressed && !isHeld)
		{
			isPressed = true;
			press_time = GetTimeNow();

			e.eType = PSV_KEYDOWN;
			return e;
		}
	}

	else
	{
		if(isPressed || isHeld)
		{
			isReleased = true;
		}

		isPressed = false;
		isHeld = false;
	}

	if(isReleased)
	{
		e.eType = PSV_KEYUP;
		return e;
	}

	e.eType = PSV_NONE;
	return e;
}

bool PSV_Button::IsPressed() const
{
	return isPressed;
}

bool PSV_Button::IsHeld() const
{
	return isHeld;
}

bool PSV_Button::IsReleased() const
{
	return isReleased;
}
#pragma endregion PSV Button

#pragma region PSV Joystick
PSV_Joystick::PSV_Joystick(const PSV_JoystickType& joystickType)
	: joyType(joystickType)
	, joyDirection(MIDDLE)
{
}

PSV_Event PSV_Joystick::Update(SceCtrlData& pad)
{
	PSV_Event e{};
	const PSV_JoystickDirection previousDirection{ joyDirection };

	const Point2f center{ 128.f, 128.f };
	Point2f joyPos{};

	if(joyType == LSTICK)
	{
		joyPos = Point2f{ float(pad.lx), float(pad.ly) };	
	}

	else
	{
		joyPos = Point2f{ float(pad.rx), float(pad.ry) };
	}

	float angleRad = AngleBetweenPoints(center, joyPos);
	const int angleDegrees = RadiansToDegrees(angleRad);

	// Radian angle correction
	if (angleRad < 0)
	{
		angleRad += M_PI * 2;
	}
	

	if(DistanceBetweenPoints(center, joyPos) > PSV_JOY_THRESHOLD)
	{
		// EAST
		if ((AngleInRange(angleDegrees, 0, 22.5) || AngleInRange(angleDegrees, 337.5, 360)) && joyDirection != E)
		{
			joyDirection = E;
		}
		
		// NORTH EAST
		if(AngleInRange(angleDegrees, 22.5, 67.5) && joyDirection != NE)
		{
			joyDirection = NE;
		}

		// NORTH
		if (AngleInRange(angleDegrees, 67.5, 112.5) && joyDirection != N)
		{
			joyDirection = N;
		}

		// NORTH WEST
		if (AngleInRange(angleDegrees, 112.5, 157.5) && joyDirection != NW)
		{
			joyDirection = NW;
		}

		// WEST
		if (AngleInRange(angleDegrees, 157.5, 202.5) && joyDirection != W)
		{
			joyDirection = W;
		}

		// SOUTH WEST
		if (AngleInRange(angleDegrees, 202.5, 247.5) && joyDirection != SW)
		{
			joyDirection = SW;
		}

		// SOUTH
		if (AngleInRange(angleDegrees, 247.5, 292.5) && joyDirection != S)
		{
			joyDirection = S;
		}

		// SOUTH EAST
		if (AngleInRange(angleDegrees, 292.5, 337.5) && joyDirection != SE)
		{
			joyDirection = SE;
		}
	}

	else
	{
		joyDirection = MIDDLE;
	}

	// Final event check
	if(previousDirection != joyDirection)
	{
		e.eType = PSV_JOYSTICKMOTION;
		e.jEvent = PSV_JoystickEvent{ joyType, joyDirection, joyPos.x, joyPos.y, angleDegrees, angleRad };
	}

	else
	{
		e.eType = PSV_NONE;
	}

	return e;
}
#pragma endregion PSV Joystick

#pragma region PSV Touchpad
PSV_Touchpad::PSV_Touchpad(const PSV_TouchpadType& touchpadType)
	: touchpadType(touchpadType)
{
}

PSV_Event PSV_Touchpad::Update(SceTouchData* touch, SceTouchData* touchOld)
{
	PSV_Event e{ PSV_NONE };
	PSV_TouchpadEvent tpEvent{ touchpadType };
	
	isReleased = false;

	// if pressed or motion
	if (touch[int(touchpadType)].reportNum > 0)
	{
		isReleased = false;

		if (!isPressed)
		{			
			isPressed = true;

			press_time = GetTimeNow();
			
			const int reportNum = int(touch[int(touchpadType)].reportNum);
			startTouch = Point2f{ float(touch[int(touchpadType)].report[reportNum - 1].x) / 2.f,  float(touch[int(touchpadType)].report[reportNum - 1].y) / 2.f };
			
			tpEvent.startTouch = startTouch;
			
			e.eType = PSV_TOUCHPAD_DOWN;
		}

		// MOTION 
		else if(PSV_TSMode == PSV_TOUCH_MOTION)
		{
			e.eType = PSV_TOUCHPAD_MOTION;
		}

		const int reportNum = int(touch[int(touchpadType)].reportNum);
		endTouch = Point2f{ float(touch[int(touchpadType)].report[reportNum - 1].x) / 2.f,  float(touch[int(touchpadType)].report[reportNum - 1].y) / 2.f };

		tpEvent.endTouch = endTouch;
		tpEvent.touchNum = reportNum;

		e.tpEvent = tpEvent;
		return e;
	}


	// if not pressed anymore but isPressed is true
	if (isPressed)
	{	
		isReleased = true;
	}

	isPressed = false;

	// release - swipe or normal release
	if (isReleased)
	{
		const int reportNum = int(touch[int(touchpadType)].reportNum);

		tpEvent.endTouch = endTouch;
		tpEvent.touchNum = reportNum;

		if (PSV_TSMode == PSV_TOUCH_SWIPE)
		{
			tpEvent.angleRad = AngleBetweenPoints(startTouch, endTouch);
			tpEvent.angleDegrees = RadiansToDegrees(tpEvent.angleRad);
			tpEvent.velocity = DistanceBetweenPoints(startTouch, endTouch) / (GetTimeNow() - press_time);

			// Radian angle correction
			if(tpEvent.angleRad < 0)
			{
				tpEvent.angleRad += M_PI * 2;
			}

			const PSV_TouchpadSwipeDirection direction{ GetDirection(tpEvent.angleDegrees) };

			tpEvent.touchpadSwipeDirection = direction;
			e.eType = PSV_TOUCHPAD_SWIPE;
			e.tpEvent = tpEvent;
			return e;
		}

		e.eType = PSV_TOUCHPAD_UP;
		e.tpEvent = tpEvent;
		return e;
	}

	return e;
}

PSV_TouchpadSwipeDirection PSV_Touchpad::GetDirection(int angle)
{
	if (AngleInRange(angle, 45, 135)) 
	{
		return SWIPE_UP;
	}
	
	if (AngleInRange(angle, 0, 45) || AngleInRange(angle, 315, 360))
	{
		return SWIPE_RIGHT;
	}
	
	if (AngleInRange(angle, 225, 315))
	{
		return SWIPE_DOWN;
	}

	return SWIPE_LEFT;

}
#pragma endregion PSV Touchpad

#pragma region PSV Functions
void PSV_Init()
{
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{CROSS, PSV_Button{ SCE_CTRL_CROSS, CROSS } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{CIRCLE, PSV_Button{ SCE_CTRL_CIRCLE, CIRCLE } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{SQUARE, PSV_Button{ SCE_CTRL_SQUARE, SQUARE } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{TRIANGLE, PSV_Button{ SCE_CTRL_TRIANGLE, TRIANGLE } });

	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ LTRIGGER, PSV_Button{ SCE_CTRL_LTRIGGER, LTRIGGER } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ RTRIGGER, PSV_Button{ SCE_CTRL_RTRIGGER, RTRIGGER } });

	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ DPAD_UP, PSV_Button{ SCE_CTRL_UP, DPAD_UP } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ DPAD_DOWN, PSV_Button{ SCE_CTRL_DOWN, DPAD_DOWN } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ DPAD_LEFT, PSV_Button{ SCE_CTRL_LEFT, DPAD_LEFT } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ DPAD_RIGHT, PSV_Button{ SCE_CTRL_RIGHT, DPAD_RIGHT } });

	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ START, PSV_Button{ SCE_CTRL_START, START } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ SELECT, PSV_Button{ SCE_CTRL_SELECT, SELECT } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ SELECT, PSV_Button{ SCE_CTRL_SELECT, SELECT } });

	PSV_Joysticks.insert(std::pair<PSV_JoystickType, PSV_Joystick>{ LSTICK, PSV_Joystick{ LSTICK } });
	PSV_Joysticks.insert(std::pair<PSV_JoystickType, PSV_Joystick>{ RSTICK, PSV_Joystick{ RSTICK } });

	PSV_Touchpads.insert(std::pair<PSV_TouchpadType, PSV_Touchpad>{ FRONT, PSV_Touchpad{ FRONT }});
	PSV_Touchpads.insert(std::pair<PSV_TouchpadType, PSV_Touchpad>{ BACK, PSV_Touchpad{ BACK }});
}

void PSV_Update(SceCtrlData& pad, SceTouchData* touch, SceTouchData* touchOld)
{
	for (std::pair<const PSV_ButtonType, PSV_Button>& pair : PSV_Buttons)
	{
		PSV_EventsQueue.push(pair.second.Update(pad));
	}

	for (std::pair<const PSV_JoystickType, PSV_Joystick>& pair : PSV_Joysticks)
	{
		PSV_EventsQueue.push(pair.second.Update(pad));
	}

	for (std::pair<const PSV_TouchpadType, PSV_Touchpad>& pair : PSV_Touchpads)
	{
		PSV_EventsQueue.push(pair.second.Update(touch, touchOld));
	}
}

int PSV_PollEvent(PSV_Event& e)
{
	if(!PSV_EventsQueue.empty())
	{
		e = PSV_EventsQueue.front();
		PSV_EventsQueue.pop();
		return 1;
	}
	
	return 0;
}

void PSV_SetTouchSamplingMode(const PSV_TouchSamplingMode& psvTouchSamplingMode)
{
	PSV_TSMode = psvTouchSamplingMode;
}
#pragma endregion PSV Functions