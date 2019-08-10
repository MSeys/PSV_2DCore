#ifndef GLOBAL_HPP
#define GLOBAL_HPP

// Vita includes
#include <psp2/ctrl.h>
#include <psp2/audioout.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/dirent.h> 
#include <psp2/touch.h>

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_wavstream.h"

#include <vita2d.h>

// C++ Includes
#include <string>

// Self includes
#include "Keyboard.h"
#include "Bank.h"
#include "structs.h"
#include "utils.h"
#include "PSV.h"

// Pi define
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Screen dimensions
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

// Resource paths
extern const std::string UI_PATH;
extern const std::string ASSETS_PATH;
extern const std::string FONTS_PATH;
extern const std::string MUSIC_PATH;
extern const std::string SFX_PATH;

extern const std::string DEFAULT_PATH;

// vita2d context
extern SceGxmContext* _vita2d_context;

// Game version
static const char *VERSION = "0.1";

#endif // GLOBAL_HPP