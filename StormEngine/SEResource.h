/******************************************************************************//**
 * \file		SEResource.h
 * \author 		Rommel Sim (100%)
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (100%)
 *
 * \brief		Resource Manager
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEDebug.h"

// Forward Declaration 
class IResource;
class TextureResource;
class FontResource;
class AudioResource;

/**
	 * Font
 */
struct SECharacter
{
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

/**
	 * Texture
 */
struct SETexture
{
	GLuint ID;
	unsigned char* texture;
	int width, height, chn;
};

/**
	 * Audio
 */
struct SEAudio
{
	FMOD::Sound* track;
	FMOD::Channel* chn;
	bool isBGM;
	bool isSFX;
	bool isPlaying_BGM;
	bool isPlaying_SFX;
	bool isPaused;
	float volume;

};

/**
	 * Audio Type
 */
enum AUDIOTYPE
{
	BGM = 0,
	SFX
};

using SEFont = std::unordered_map<char, SECharacter>;


/**
	 * Loads all fonts in specified path
 */
void SELoadAllFonts(std::filesystem::path path);

/**
	 * Load all background music in specified path
 */
void SELoadAllBgm(std::filesystem::path path);

/**
	 * Load all sound effects in specified path
 */
void SELoadAllSfx(std::filesystem::path path);

/**
	 * Load all textures in specified path
 */
void SELoadTexture(std::filesystem::path path);

/**
	 * Load a font in specified path
 */
void SELoadFont(std::filesystem::path path);

/**
	 * Load a sound track in specified path
	 * can be bgm or sfx
 */
void SELoadSound(AUDIOTYPE TrackType, std::filesystem::path path);

/**
	 * Returns the Texture ID loaded in the resource manager.
	 * @return 0 if texture is not found in resource manager
 */
SEUint SEGetTexture(std::filesystem::path _name);


/**
	 * Returns font package.
 */
SEFont SEGetFont(std::filesystem::path name);

/**
	 * Get audio from resource manager using the specified path
 */
SEAudio* SEGetAudio(AUDIOTYPE AudioType, std::filesystem::path name);

/**
	 * Get audio from resource manager using the track name of the audio file
 */
SEAudio* SEGetAudio(AUDIOTYPE AudioType, std::string track);




// Used for IMGUI. fill in for the assets data
std::vector<std::string> SEGetTexturesResources_FileAndPath();
std::vector<std::string> SEGetFontResources_FileAndPath();

 const std::unordered_map<std::string, TextureResource*>& SEGetTextureResources();
 const std::unordered_map<std::string, FontResource*>& SEGetFontResources();
 const std::unordered_map<std::string, AudioResource*>& SEGetAudioBGMResources();
 const std::unordered_map<std::string, AudioResource*>& SEGetAudioSFXResources();


void SEResourceMgrExit();

// added on 24/2
float GetTextureAr(const std::string& name);
