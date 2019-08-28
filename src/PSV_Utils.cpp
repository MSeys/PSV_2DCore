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

	Scale2f scales{ 1, 1 };
	Vector2f translations{ 0, 0 };
	
	for(int i{ 1 }; i < PSV_CT; i++)
	{
		scales.x *= PSV_Scales[i].x;
		scales.y *= PSV_Scales[i].y;
		
		translations.x += PSV_Translations[i].x;
		translations.y += PSV_Translations[i].y;
	}
	
	PSV_Scales.push_back(scales);
	PSV_Translations.push_back(translations);
}

void PSV_Translate(float x, float y)
{
	if(!PSV_Allowed)
	{
		return;
	}

	PSV_Translations[PSV_CT].x += x;
	PSV_Translations[PSV_CT].y += y;
}

void PSV_Scale(float x, float y)
{
	if (!PSV_Allowed)
	{
		return;
	}

	PSV_Scales[PSV_CT].x *= x;
	PSV_Scales[PSV_CT].y *= y;
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
