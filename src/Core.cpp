#include "Core.h"
#include "GameHandler.h"
#include "PSV.h"

Core::Core()
{
	Initialize();
	m_pBank = new Bank{};
}


Core::~Core()
{
	Cleanup();
}

void Core::Run()
{
	// Create GameHandler Object
	GameHandler gameHandler{};

	// Main loop flag
	bool quit{ false };

	float start{};
	while(!quit)
	{
		memcpy(m_TouchOld, m_Touch, sizeof(m_TouchOld));
		
		sceCtrlPeekBufferPositive(0, &m_Pad, 1);
		sceTouchPeek(0, &m_Touch[0], 1);
		sceTouchPeek(1, &m_Touch[1], 1);
		
		PSV_Update(m_Pad, m_Touch, m_TouchOld);
		PSV_Event psvEvent{};

		while(PSV_PollEvent(psvEvent))
		{
			switch(psvEvent.eType)
			{
			default:
			case PSV_NONE:
				break;

			case PSV_KEYDOWN:
				gameHandler.ProcessKeyDownEvent(psvEvent.bEvent);
				break;
			
			case PSV_KEYUP:
				gameHandler.ProcessKeyUpEvent(psvEvent.bEvent);
				break;

			case PSV_KEYHELD:
				gameHandler.ProcessKeyHeldEvent(psvEvent.bEvent);
				break;

			case PSV_JOYSTICKMOTION:
				gameHandler.ProcessJoystickMotionEvent(psvEvent.jEvent);
				break;

			case PSV_TOUCHPAD_DOWN:
				gameHandler.ProcessTouchpadDownEvent(psvEvent.tpEvent);
				break;

			case PSV_TOUCHPAD_UP:
				gameHandler.ProcessTouchpadUpEvent(psvEvent.tpEvent);
				break;

			case PSV_TOUCHPAD_MOTION:
				gameHandler.ProcessTouchpadMotionEvent(psvEvent.tpEvent);
				break;

			case PSV_TOUCHPAD_SWIPE:
				gameHandler.ProcessTouchpadSwipeEvent(psvEvent.tpEvent);
				break;
			}
		}

		if (!quit)
		{
			// Get current time
			const float finish = GetTimeNow();

			// Calculate elapsed time
			float elapsedSec = finish - start;

			start = GetTimeNow();

			// Update game
			gameHandler.Update(elapsedSec);
			
			vita2d_start_drawing();
			vita2d_clear_screen();
			
			// Draw game
			gameHandler.Draw();

			vita2d_end_drawing();
			vita2d_common_dialog_update();
			vita2d_wait_rendering_done();
			vita2d_swap_buffers();
		}
	}
}

void Core::Initialize()
{
	// Initialize controls
	sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
	sceTouchSetSamplingState(SCE_TOUCH_PORT_FRONT, SCE_TOUCH_SAMPLING_STATE_START);
	sceTouchEnableTouchForce(SCE_TOUCH_PORT_FRONT);

	// Initialize Vita2D
	vita2d_init_advanced_with_msaa(1 * 1024 * 1024, SCE_GXM_MULTISAMPLE_4X);
	vita2d_set_clear_color(RGBA8(0, 0, 0, 255));

	PSV_Init();
}

void Core::Cleanup()
{
	delete m_pBank;
	vita2d_fini();
}
