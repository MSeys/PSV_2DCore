#include "Bank.h"

#include <fstream>

#include "Texture.h"
#include "Font.h"
#include "SFX.h"
#include "Music.h"

std::unordered_map<std::string, Texture*> Bank::m_AssetsMap;
std::unordered_map<std::string, Texture*> Bank::m_UIMap;
std::unordered_map<std::string, Font*> Bank::m_FontsMap;
std::unordered_map<std::string, Music*> Bank::m_MusicMap;
std::unordered_map<std::string, SFX*> Bank::m_SFXMap;
SoLoud::Soloud* Bank::m_pSoLoudEngine{ new SoLoud::Soloud() };

Bank::Bank()
{
	m_pSoLoudEngine->init();
	InitializeBanks();
}

void Bank::InitializeBanks()
{
	sceIoMkdir("ux0:/data/PSV_2DCore_Data", 0777);
	InitializeAssetsMap(ASSETS_PATH);
	InitializeUIMap(UI_PATH);
	InitializeFontsMap(FONTS_PATH);
	InitializeMusicMap(MUSIC_PATH);
	InitializeSFXMap(SFX_PATH);
}

Texture* Bank::FindAsset(const std::string& fileName)
{
	if (m_AssetsMap.find(fileName) != m_AssetsMap.end())
	{
		return m_AssetsMap.find(fileName)->second;
	}

	return nullptr;
}

Texture* Bank::FindUI(const std::string& fileName)
{
	if (m_UIMap.find(fileName) != m_UIMap.end())
	{
		return m_UIMap.find(fileName)->second;
	}

	return nullptr;
}

Font* Bank::FindFont(const std::string& fileName)
{
	if (m_FontsMap.find(fileName) != m_FontsMap.end())
	{
		return m_FontsMap.find(fileName)->second;
	}

	return nullptr;
}

Music* Bank::FindMusic(const std::string& fileName)
{
	if (m_MusicMap.find(fileName) != m_MusicMap.end())
	{
		return m_MusicMap.find(fileName)->second;
	}

	return nullptr;
}

SFX* Bank::FindSound(const std::string& fileName)
{
	if (m_SFXMap.find(fileName) != m_SFXMap.end())
	{
		return m_SFXMap.find(fileName)->second;
	}

	return nullptr;
}

void Bank::InitializeAssetsMap(const std::string& assetsPath)
{
	const int dfd = sceIoDopen(assetsPath.c_str());

	if (dfd > 0)
	{
		SceIoDirent file;

		while (sceIoDread(dfd, &file) > 0)
		{
			if (SCE_S_ISDIR(file.d_stat.st_mode) != 0)
			{
				std::string newPath{ assetsPath + "/" + file.d_name };
				InitializeAssetsMap(newPath);
			}

			else
			{
				std::string filename{ file.d_name };
				const std::string filePath{ assetsPath + "/" + filename };
				std::string formattedFilename{ filename.substr(0, filename.find('.')) };

				m_AssetsMap[formattedFilename] = new Texture{ filePath };
			}
		}
	}

	sceIoDclose(dfd);
}

void Bank::InitializeUIMap(const std::string& uiPath)
{
	const int dfd = sceIoDopen(uiPath.c_str());

	if (dfd > 0)
	{
		SceIoDirent file;

		while (sceIoDread(dfd, &file) > 0)
		{
			if (SCE_S_ISDIR(file.d_stat.st_mode) != 0)
			{
				std::string newPath{ uiPath + "/" + file.d_name };
				InitializeUIMap(newPath);
			}

			else
			{
				std::string filename{ file.d_name };
				const std::string filePath{ uiPath + "/" + filename };
				std::string formattedFilename{ filename.substr(0, filename.find('.')) };
				m_UIMap[formattedFilename] = new Texture{ filePath };

				
			}
		}
	}

	sceIoDclose(dfd);
}

void Bank::InitializeFontsMap(const std::string& fontsPath)
{
	const int dfd = sceIoDopen(fontsPath.c_str());

	if (dfd > 0)
	{
		SceIoDirent file;

		while (sceIoDread(dfd, &file) > 0)
		{
			if (SCE_S_ISDIR(file.d_stat.st_mode) != 0)
			{
				std::string newPath{ fontsPath + "/" + file.d_name };
				InitializeFontsMap(newPath);
			}

			else
			{
				std::string filename{ file.d_name };
				const std::string filePath{ fontsPath + "/" + filename };
				std::string formattedFilename{ filename.substr(0, filename.find('.')) };
				m_FontsMap[formattedFilename] = new Font{ filePath };
			}
		}
	}

	sceIoDclose(dfd);
}

void Bank::InitializeMusicMap(const std::string& musicPath)
{
	const int dfd = sceIoDopen(musicPath.c_str());

	if (dfd > 0)
	{
		SceIoDirent file;

		while (sceIoDread(dfd, &file) > 0)
		{
			if (SCE_S_ISDIR(file.d_stat.st_mode) != 0)
			{
				std::string newPath{ musicPath + "/" + file.d_name };
				InitializeMusicMap(newPath);
			}

			else
			{
				std::string filename{ file.d_name };
				const std::string filePath{ musicPath + "/" + filename };
				std::string formattedFilename{ filename.substr(0, filename.find('.')) };
				m_MusicMap[formattedFilename] = new Music{ m_pSoLoudEngine, filePath };
			}
		}
	}

	sceIoDclose(dfd);
}

void Bank::InitializeSFXMap(const std::string& sfxPath)
{
	const int dfd = sceIoDopen(sfxPath.c_str());

	if (dfd > 0)
	{
		SceIoDirent file;

		while (sceIoDread(dfd, &file) > 0)
		{
			if (SCE_S_ISDIR(file.d_stat.st_mode) != 0)
			{
				std::string newPath{ sfxPath + "/" + file.d_name };
				InitializeSFXMap(newPath);
			}

			else
			{
				std::string filename{ file.d_name };
				const std::string filePath{ sfxPath + "/" + filename };
				std::string formattedFilename{ filename.substr(0, filename.find('.')) };
				m_SFXMap[formattedFilename] = new SFX{ m_pSoLoudEngine, filePath };
			}
		}
	}

	sceIoDclose(dfd);
}

Bank::~Bank()
{
	for (std::pair<std::string, Texture*> pair : m_AssetsMap)
	{
		delete pair.second;
		pair.second = nullptr;
	}

	for (std::pair<std::string, Texture*> pair : m_UIMap)
	{
		delete pair.second;
		pair.second = nullptr;
	}

	for (std::pair<std::string, Font*> pair : m_FontsMap)
	{
		delete pair.second;
		pair.second = nullptr;
	}

	for (std::pair<std::string, SFX*> pair : m_SFXMap)
	{
		delete pair.second;
		pair.second = nullptr;
	}

	for (std::pair<std::string, Music*> pair : m_MusicMap)
	{
		delete pair.second;
		pair.second = nullptr;
	}

	m_pSoLoudEngine->deinit();
	delete m_pSoLoudEngine;
	m_pSoLoudEngine = nullptr;
}
