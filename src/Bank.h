#pragma once

class Texture;
class Font;
class Music;
class SFX;


#include <unordered_map>
#include "soloud.h"

class Bank
{
	static std::unordered_map<std::string, Texture*> m_AssetsMap;
	static std::unordered_map<std::string, Texture*> m_UIMap;
	static std::unordered_map<std::string, Texture*> m_IconsMap;
	static std::unordered_map<std::string, Font*> m_FontsMap;
	static std::unordered_map<std::string, Music*> m_MusicMap;
	static std::unordered_map<std::string, SFX*> m_SFXMap;
	static SoLoud::Soloud* m_pSoLoudEngine;

public:
	Bank();

	Bank(const Bank& other) = delete;
	Bank& operator=(const Bank& rhs) = delete;
	Bank(const Bank&& other) = delete;
	Bank& operator=(const Bank&& rhs) = delete;

	~Bank();

	static void InitializeBanks();

	static Texture* FindAsset(const std::string& fileName);
	static Texture* FindUI(const std::string& fileName);
	static Texture* FindIcon(const std::string& fileName);
	static Font* FindFont(const std::string& fileName);
	static Music* FindMusic(const std::string& fileName);
	static SFX* FindSound(const std::string& fileName);

private:
	static void InitializeAssetsMap(const std::string& assetsPath);
	static void InitializeUIMap(const std::string& uiPath);
	static void InitializeIconsMap(const std::string& iconsPath);
	static void InitializeFontsMap(const std::string& fontsPath);
	static void InitializeMusicMap(const std::string& musicPath);
	static void InitializeSFXMap(const std::string& sfxPath);
};
