#pragma once
#include "pch.h"

class Core
{
public:
	Core();
	Core(const Core& other) = delete;
	Core& operator=(const Core& other) = delete;
	Core(Core&& other) = delete;
	Core& operator=(Core&& other) = delete;
	~Core();

	void Run();

private:
	Bank* m_pBank;

	SceCtrlData m_Pad{};
	SceTouchData m_TouchOld[SCE_TOUCH_PORT_MAX_NUM]{};
	SceTouchData m_Touch[SCE_TOUCH_PORT_MAX_NUM]{};

	void Initialize();
	void Cleanup();
};
