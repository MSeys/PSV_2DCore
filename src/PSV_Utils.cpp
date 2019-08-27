#include "PSV_Utils.h"

PSV_TouchSamplingMode PSV_TSMode{ PSV_TOUCH_MOTION };

// 0 is reserved for drawing when there is no transformation
int PSV_CT{ 0 };

std::vector<Vector2f> PSV_Translations{ Vector2f{ 0, 0 } };
std::vector<Scale2f> PSV_Scales{ Scale2f{ 1, 1 } };

bool PSV_Allowed{ false };

void PSV_SetTouchSamplingMode(const PSV_TouchSamplingMode& psvTouchSamplingMode)
{
	PSV_TSMode = psvTouchSamplingMode;
}

void PSV_Begin()
{
	if (PSV_CT == 0)
	{
		PSV_Allowed = true;
	}

	PSV_CT++;

	PSV_Scales.emplace_back(1, 1);
	PSV_Translations.emplace_back(0, 0);
}

void PSV_Translate(float x, float y)
{
	if(!PSV_Allowed)
	{
		return;
	}

	PSV_Translations[PSV_CT] = Vector2f{ x, y };
}

void PSV_Scale(float x, float y)
{
	if (!PSV_Allowed)
	{
		return;
	}

	PSV_Scales[PSV_CT] = Scale2f{ x, y };
}

void PSV_End()
{
	if(PSV_CT > 0)
	{
		PSV_CT--;

		PSV_Scales.pop_back();
		PSV_Translations.pop_back();
	}
	
	if (PSV_CT == 0)
	{
		PSV_Allowed = false;
	}
}
