/******************************************************************************
 * \file		SEAudio.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (40%)
 * \par 
 * \brief		Audio System
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEResource.h"
class AudioSys
{
public:
#define MAX_CHANNEL 64
	FMOD::System* system;
	FMOD::Channel* channel;
	FMOD::SoundGroup* GroupBGM; // background music
	FMOD::SoundGroup* GroupSFX; // sound effects

public:

	float m_volume;
};

/********************************************************************************************
	* Audio Core Systems
*********************************************************************************************/

/**
	 * Init Audio System
 */
void SEAudioInit();

/**
	 * Update Audio System
	 * Called in 1 frame
 */
void SEAudioUpdate();

/**
	 * Exit Audio System
 */
void SEAudioExit();

/**
	 * Returns handle to audio system
 */
AudioSys* SEGetAudioHandle();

/**
	 * Play sound based on specified directory.
	 * Required to specify the AUDIOTYPE (BGM, SFX)
 */
void SEPlaySound(AUDIOTYPE AudioType, std::filesystem::path path);
void SEPlaySound(AUDIOTYPE AudioType, const char* track);

/**
	 * un/Pause sound based on specified directory.
	 * Required to specify the AUDIOTYPE (BGM, SFX)
 */
void SEPauseSound(AUDIOTYPE AudioType, std::filesystem::path path);
void SEPauseSound(AUDIOTYPE AudioType,std::string track);
void SEUnpauseSound(AUDIOTYPE AudioType, std::filesystem::path path);
void SEUnpauseSound(AUDIOTYPE AudioType, std::string track);

/**
	* Stop playing sound 
 */
void SEStopSound(AUDIOTYPE AudioType, std::string track);
void SEStopSound(AUDIOTYPE AudioType, std::filesystem::path path);

/**
	 * Set volume 
 */
void SESetVolume(AUDIOTYPE AudioType, std::string track, float volume);
void SESetAllVolume(float volume);
float SEGetVolume(AUDIOTYPE AudioType, std::string track);


float SEgetGlobalVolume();
