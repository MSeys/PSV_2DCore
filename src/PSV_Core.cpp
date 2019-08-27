#include "PSV_Core.h"
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

std::vector<std::string> PSV_TouchSwipeDirectionStrings
{
	"Swipe Up", "Swipe Down", "Swipe Left", "Swipe Right"
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
#pragma endregion PSV Button

#pragma region PSV Joystick
PSV_Joystick::PSV_Joystick(const PSV_JoystickType& joystickType)
	: joyType(joystickType)
	, joyDirection(MIDDLE)
	, x(0)
	, y(0)
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
		x = pad.lx;
		y = pad.ly;
	}

	else
	{
		joyPos = Point2f{ float(pad.rx), float(pad.ry) };
		x = pad.rx;
		y = pad.ry;
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
	if (touchpadType == FRONT)
	{
		maxTouches = 6;
	}
	
	else
	{
		maxTouches = 4;
	}
	
	for (int i{ 0 }; i < maxTouches; i++)
	{
		pressTouches.emplace_back(0, 0);
		motionTouches.emplace_back(0, 0);
		releaseTouches.emplace_back(0, 0);
		
		touchIsReleased.push_back(false);
		touchIsPressed.push_back(false);
		touchPressTimes.push_back(0);
	}
}

std::queue<PSV_Event> PSV_Touchpad::Update(SceTouchData* touch, SceTouchData* touchOld)
{
	std::queue<PSV_Event> events;
	const int reportNum{ int(touch[int(touchpadType)].reportNum) };
	const int touchVPos{ reportNum - 1 };

	for (int i{}; i < maxTouches; i++)
	{
		// release default set to false
		touchIsReleased[i] = false;
		
		// PRESS/MOTION ON or ABOVE current touch position
		if(touchVPos >= i)
		{
			if (PSV_TSMode != PSV_TOUCH_SWIPE)
			{
				// Touch position isn't in PRESSED state
				if (!touchIsPressed[i])
				{
					// set pressed and pressed coordinate.
					touchIsPressed[i] = true;
					pressTouches[i] = Point2f{ float(touch[int(touchpadType)].report[i].x) / 2.f,  float(touch[int(touchpadType)].report[i].y) / 2.f };
					touchPressTimes[i] = GetTimeNow();

					// PUSHING EVENT TO QUEUE
					{
						// Events declarations and defining
						PSV_Event e{ PSV_TOUCHPAD_DOWN };
						PSV_TouchpadEvent tpEvent{ touchpadType };

						// set tpEvent variables
						tpEvent.pressTouch = pressTouches[i];
						tpEvent.touchVectorNum = i;
						tpEvent.reportNum = reportNum;

						// set PSV_Event tpEvent as local tpEvent
						e.tpEvent = tpEvent;

						// Pushing the PSV_Event to the local queue
						events.push(e);
					}
				}

				// Touch is in PRESSED state
				else
				{
					// If TouchSamplingMode is set on Touch Motion
					if (PSV_TSMode == PSV_TOUCH_MOTION)
					{
						motionTouches[i] = Point2f{ float(touch[int(touchpadType)].report[i].x) / 2.f,  float(touch[int(touchpadType)].report[i].y) / 2.f };

						// PUSHING EVENT TO QUEUE
						{
							// Events declarations and defining
							PSV_Event e{ PSV_TOUCHPAD_MOTION };
							PSV_TouchpadEvent tpEvent{ touchpadType };

							// set tpEvent variables
							tpEvent.motionTouch = motionTouches[i];
							tpEvent.touchVectorNum = i;
							tpEvent.reportNum = reportNum;

							// set PSV_Event tpEvent as local tpEvent
							e.tpEvent = tpEvent;

							// Pushing the PSV_Event to the local queue
							events.push(e);
						}
					}
				}
			}

			else
			{
				// Touch position isn't in PRESSED state
				if (!touchIsPressed[i])
				{
					// set pressed and pressed coordinate.
					touchIsPressed[i] = true;
					pressTouches[i] = Point2f{ float(touch[int(touchpadType)].report[i].x) / 2.f,  float(touch[int(touchpadType)].report[i].y) / 2.f };
					touchPressTimes[i] = GetTimeNow();
				}
			}
		}

		// PRESS UNDER current touch position or NO PRESS (anymore, e.g. touchVPos is -1 and i is 0)
		// Triggered on a release of a finger at the latest i not hitting the above i index.
		else
		{
			// if it was pressed while the num
			if (touchIsPressed[i])
			{
				touchIsReleased[i] = true;
			}

			touchIsPressed[i] = false;

			if (touchIsReleased[i])
			{
				// Set release position at index i
				releaseTouches[i] = Point2f{ float(touch[int(touchpadType)].report[i].x) / 2.f,  float(touch[int(touchpadType)].report[i].y) / 2.f };
				
				if (PSV_TSMode == PSV_TOUCH_SWIPE)
				{
					// PUSHING EVENT TO QUEUE
					{
						// Events declarations and defining
						PSV_Event e{ PSV_TOUCHPAD_SWIPE };
						PSV_TouchpadEvent tpEvent{ touchpadType };

						// set tpEvent variables
						tpEvent.releaseTouch = releaseTouches[i];
						tpEvent.pressTouch = pressTouches[i];
						tpEvent.touchVectorNum = i;
						tpEvent.reportNum = reportNum;

						tpEvent.angleRad = AngleBetweenPoints(pressTouches[i], releaseTouches[i]);
						tpEvent.angleDegrees = RadiansToDegrees(tpEvent.angleRad);
						tpEvent.length = DistanceBetweenPoints(pressTouches[i], releaseTouches[i]);
						tpEvent.velocity = tpEvent.length / (GetTimeNow() - touchPressTimes[i]);
						
						// Radian angle correction
						if (tpEvent.angleRad < 0)
						{
							tpEvent.angleRad += M_PI * 2;
						}

						const PSV_TouchpadSwipeDirection direction{ GetDirection(tpEvent.angleDegrees) };
						tpEvent.touchpadSwipeDirection = direction;

						// set PSV_Event tpEvent as local tpEvent
						e.tpEvent = tpEvent;

						// Pushing the PSV_Event to the local queue
						events.push(e);
					}
				}

				else
				{
					// PUSHING EVENT TO QUEUE
					{
						// Events declarations and defining
						PSV_Event e{ PSV_TOUCHPAD_UP };
						PSV_TouchpadEvent tpEvent{ touchpadType };

						// set tpEvent variables
						tpEvent.releaseTouch = releaseTouches[i];
						tpEvent.touchVectorNum = i;
						tpEvent.reportNum = reportNum;

						// set PSV_Event tpEvent as local tpEvent
						e.tpEvent = tpEvent;

						// Pushing the PSV_Event to the local queue
						events.push(e);
					}
				}
			}
		}
	}

	return events;
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
		std::queue<PSV_Event> touchEventsQueue = pair.second.Update(touch, touchOld);
		while(!touchEventsQueue.empty())
		{
			PSV_EventsQueue.push(touchEventsQueue.front());
			touchEventsQueue.pop();
		}
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
