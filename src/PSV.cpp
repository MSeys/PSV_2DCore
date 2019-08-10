#include "PSV.h"
#define PSV_HOLD_TIME 0.4 
#define PSV_JOY_LOW_DZ 110
#define PSV_JOY_HIGH_DZ 150

std::vector<std::string> PSV_ButtonStrings
{
	"CROSS", "CIRCLE", "SQUARE", "TRIANGLE",
	"LTRIGGER", "RTRIGGER",
	"UP", "DOWN", "LEFT", "RIGHT",
	"START", "SELECT"
};

std::vector<std::string> PSV_JoystickDirectionStrings
{
	"UP_LEFT", "UP", "UP_RIGHT", "LEFT",
	"RIGHT", "DOWN_LEFT", "DOWN", "DOWN_RIGHT", "MIDDLE"
};

std::vector<std::string> PSV_JoystickTypeStrings
{
	"LSTICK", "RSTICK"
};

std::unordered_map<PSV_ButtonType, PSV_Button> PSV_Buttons;
std::unordered_map<PSV_JoystickType, PSV_Joystick> PSV_Joysticks;
std::unordered_map<PSV_TouchpadType, PSV_Touchpad> PSV_Touchpads;
std::queue<PSV_Event> PSV_EventsQueue;





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
	, joyDirection(PSV_JoystickDirection::MIDDLE)
{
}

PSV_Event PSV_Joystick::Update(SceCtrlData& pad)
{
	PSV_Event e{};
	const PSV_JoystickDirection previousDirection{ joyDirection };
	
	if (joyType == PSV_JoystickType::LSTICK)
	{
		// Middle
		if ((pad.lx > PSV_JOY_LOW_DZ && pad.lx < PSV_JOY_HIGH_DZ) && (pad.ly > PSV_JOY_LOW_DZ && pad.ly < PSV_JOY_HIGH_DZ) && joyDirection != PSV_JoystickDirection::MIDDLE)
		{
			joyDirection = PSV_JoystickDirection::MIDDLE;
		}

		// UP LEFT
		if (pad.lx == 0 && pad.ly < 50 && joyDirection != PSV_JoystickDirection::UP_LEFT)
		{
			joyDirection = PSV_JoystickDirection::UP_LEFT;
		}

		// UP MIDDLE
		if (pad.lx < PSV_JOY_LOW_DZ && pad.ly == 0 && joyDirection != PSV_JoystickDirection::UP)
		{
			joyDirection = PSV_JoystickDirection::UP;
		}

		// UP RIGHT
		if (pad.lx == 255 && pad.ly < PSV_JOY_LOW_DZ && joyDirection != PSV_JoystickDirection::UP_RIGHT)
		{
			joyDirection = PSV_JoystickDirection::UP_RIGHT;
		}

		// RIGHT
		if (pad.lx == 255 && pad.ly > PSV_JOY_LOW_DZ && pad.ly < PSV_JOY_HIGH_DZ && joyDirection != PSV_JoystickDirection::RIGHT)
		{
			joyDirection = PSV_JoystickDirection::RIGHT;
		}

		// DOWN RIGHT
		if (pad.lx == 255 && pad.ly > PSV_JOY_HIGH_DZ && joyDirection != PSV_JoystickDirection::DOWN_RIGHT)
		{
			joyDirection = PSV_JoystickDirection::DOWN_RIGHT;
		}

		// DOWN
		if (pad.lx > PSV_JOY_LOW_DZ && pad.lx < PSV_JOY_HIGH_DZ && pad.ly == 255 && joyDirection != PSV_JoystickDirection::DOWN)
		{
			joyDirection = PSV_JoystickDirection::DOWN;
		}

		// DOWN LEFT
		if (pad.lx < PSV_JOY_LOW_DZ && pad.ly == 255 && joyDirection != PSV_JoystickDirection::DOWN_LEFT)
		{
			joyDirection = PSV_JoystickDirection::DOWN_LEFT;
		}

		// LEFT
		if (pad.lx == 0 && pad.ly > PSV_JOY_LOW_DZ && pad.ly < PSV_JOY_HIGH_DZ && joyDirection != PSV_JoystickDirection::LEFT)
		{
			joyDirection = PSV_JoystickDirection::LEFT;
		}
	}

	else if (joyType == PSV_JoystickType::RSTICK)
	{
		// Middle
		if ((pad.rx > PSV_JOY_LOW_DZ && pad.rx < PSV_JOY_HIGH_DZ) && (pad.ry > PSV_JOY_LOW_DZ && pad.ry < PSV_JOY_HIGH_DZ) && joyDirection != PSV_JoystickDirection::MIDDLE)
		{
			joyDirection = PSV_JoystickDirection::MIDDLE;
		}

		// UP LEFT
		if (pad.rx == 0 && pad.ry < 50 && joyDirection != PSV_JoystickDirection::UP_LEFT)
		{
			joyDirection = PSV_JoystickDirection::UP_LEFT;
		}

		// UP MIDDLE
		if (pad.rx < PSV_JOY_LOW_DZ && pad.ry == 0 && joyDirection != PSV_JoystickDirection::UP)
		{
			joyDirection = PSV_JoystickDirection::UP;
		}

		// UP RIGHT
		if (pad.rx == 255 && pad.ry < PSV_JOY_LOW_DZ && joyDirection != PSV_JoystickDirection::UP_RIGHT)
		{
			joyDirection = PSV_JoystickDirection::UP_RIGHT;
		}

		// RIGHT
		if (pad.rx == 255 && pad.ry > PSV_JOY_LOW_DZ && pad.ry < PSV_JOY_HIGH_DZ && joyDirection != PSV_JoystickDirection::RIGHT)
		{
			joyDirection = PSV_JoystickDirection::RIGHT;
		}

		// DOWN RIGHT
		if (pad.rx == 255 && pad.ry > PSV_JOY_HIGH_DZ && joyDirection != PSV_JoystickDirection::DOWN_RIGHT)
		{
			joyDirection = PSV_JoystickDirection::DOWN_RIGHT;
		}

		// DOWN
		if (pad.rx > PSV_JOY_LOW_DZ && pad.rx < PSV_JOY_HIGH_DZ && pad.ry == 255 && joyDirection != PSV_JoystickDirection::DOWN)
		{
			joyDirection = PSV_JoystickDirection::DOWN;
		}

		// DOWN LEFT
		if (pad.rx < PSV_JOY_LOW_DZ && pad.ry == 255 && joyDirection != PSV_JoystickDirection::DOWN_LEFT)
		{
			joyDirection = PSV_JoystickDirection::DOWN_LEFT;
		}

		// LEFT
		if (pad.rx == 0 && pad.ry > PSV_JOY_LOW_DZ && pad.ry < PSV_JOY_HIGH_DZ && joyDirection != PSV_JoystickDirection::LEFT)
		{
			joyDirection = PSV_JoystickDirection::LEFT;
		}
	}

	if(previousDirection != joyDirection)
	{
		float xValue;
		float yValue;

		if(joyType == PSV_JoystickType::LSTICK)
		{
			xValue = pad.lx;
			yValue = pad.ly;
		}

		else
		{
			xValue = pad.rx;
			yValue = pad.ry;
		}

		e.eType = PSV_JOYSTICKMOTION;
		e.jEvent = PSV_JoystickEvent{ joyType, joyDirection, xValue, yValue };
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
	PSV_Event e{};
	PSV_TouchpadEvent tpEvent{ touchpadType };
	
	isReleased = false;
	
	if (touch[int(touchpadType)].reportNum > 0)
	{
		isReleased = false;

		if (!isPressed)
		{
			isPressed = true;

			const int reportNum = int(touch[int(touchpadType)].reportNum);
			startTouch = Point2{ touch[int(touchpadType)].report[reportNum - 1].x / 2,  touch[int(touchpadType)].report[reportNum - 1].y / 2 };
			
			tpEvent.startTouch = startTouch;
			tpEvent.touchNum = reportNum;
			
			e.eType = PSV_TOUCHPAD_DOWN;
			e.tpEvent = tpEvent;
			return e;
		}

		// motion
		const int reportNum = int(touch[int(touchpadType)].reportNum);
		endTouch = Point2{ touch[int(touchpadType)].report[reportNum - 1].x / 2,  touch[int(touchpadType)].report[reportNum - 1].y / 2 };

		tpEvent.endTouch = endTouch;
		tpEvent.touchNum = reportNum;

		e.eType = PSV_TOUCHPAD_MOTION;
		e.tpEvent = tpEvent;
		return e;
	}

	if (isPressed)
	{
		isReleased = true;
	}

	isPressed = false;

	if (isReleased)
	{
		const int reportNum = int(touch[int(touchpadType)].reportNum);
		const Point2 endTouch{ touch[int(touchpadType)].report[reportNum - 1].x / 2,  touch[int(touchpadType)].report[reportNum - 1].y / 2 };
		
		tpEvent.endTouch = endTouch;
		tpEvent.touchNum = reportNum;
		
		e.eType = PSV_TOUCHPAD_UP;
		e.tpEvent = tpEvent;
		return e;
	}
}
#pragma endregion PSV Touchpad

#pragma region PSV Functions
void PSV_Init()
{
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{PSV_ButtonType::CROSS, PSV_Button{ SCE_CTRL_CROSS, PSV_ButtonType::CROSS } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{PSV_ButtonType::CIRCLE, PSV_Button{ SCE_CTRL_CIRCLE, PSV_ButtonType::CIRCLE } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{PSV_ButtonType::SQUARE, PSV_Button{ SCE_CTRL_SQUARE, PSV_ButtonType::SQUARE } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{PSV_ButtonType::TRIANGLE, PSV_Button{ SCE_CTRL_TRIANGLE, PSV_ButtonType::TRIANGLE } });

	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ PSV_ButtonType::LTRIGGER, PSV_Button{ SCE_CTRL_LTRIGGER, PSV_ButtonType::LTRIGGER } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ PSV_ButtonType::RTRIGGER, PSV_Button{ SCE_CTRL_RTRIGGER, PSV_ButtonType::RTRIGGER } });

	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ PSV_ButtonType::UP, PSV_Button{ SCE_CTRL_UP, PSV_ButtonType::UP } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ PSV_ButtonType::DOWN, PSV_Button{ SCE_CTRL_DOWN, PSV_ButtonType::DOWN } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ PSV_ButtonType::LEFT, PSV_Button{ SCE_CTRL_LEFT, PSV_ButtonType::LEFT } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ PSV_ButtonType::RIGHT, PSV_Button{ SCE_CTRL_RIGHT, PSV_ButtonType::RIGHT } });

	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ PSV_ButtonType::START, PSV_Button{ SCE_CTRL_START, PSV_ButtonType::START } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ PSV_ButtonType::SELECT, PSV_Button{ SCE_CTRL_SELECT, PSV_ButtonType::SELECT } });
	PSV_Buttons.insert(std::pair<PSV_ButtonType, PSV_Button>{ PSV_ButtonType::SELECT, PSV_Button{ SCE_CTRL_SELECT, PSV_ButtonType::SELECT } });

	PSV_Joysticks.insert(std::pair<PSV_JoystickType, PSV_Joystick>{ PSV_JoystickType::LSTICK, PSV_Joystick{ PSV_JoystickType::LSTICK } });
	PSV_Joysticks.insert(std::pair<PSV_JoystickType, PSV_Joystick>{ PSV_JoystickType::RSTICK, PSV_Joystick{ PSV_JoystickType::RSTICK } });

	PSV_Touchpads.insert(std::pair<PSV_TouchpadType, PSV_Touchpad>{ PSV_TouchpadType::FRONT, PSV_Touchpad{ PSV_TouchpadType::FRONT }});
	PSV_Touchpads.insert(std::pair<PSV_TouchpadType, PSV_Touchpad>{ PSV_TouchpadType::BACK, PSV_Touchpad{ PSV_TouchpadType::BACK }});
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
#pragma endregion PSV Functions