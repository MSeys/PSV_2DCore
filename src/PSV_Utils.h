#pragma once
#include "pch.h"

enum PSV_TouchSamplingMode
{
	PSV_TOUCH_MOTION, PSV_TOUCH_SWIPE
};

extern PSV_TouchSamplingMode PSV_TSMode;
extern int PSV_CT;

extern std::vector<Vector2f> PSV_Translations;
extern std::vector<Scale2f> PSV_Scales;
extern bool PSV_Allowed;

void PSV_SetTouchSamplingMode(const PSV_TouchSamplingMode& psvTouchSamplingMode);

void PSV_Begin();
void PSV_Translate(float x, float y);
void PSV_Scale(float x, float y);
void PSV_End();
