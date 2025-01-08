/******************************************************************************//**
 * \file			SEAudio.cpp
 * \author 			Rommel Sim
 * \contributions	zhenpengrommel.sim@digpen.edu (100%)
 *
 * \brief			Audio System
 *
 * \date   			January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEAudio.h"
#include "SESerialiser.h"

std::unique_ptr<AudioSys> pAudio;

void SEAudioInit()
{
	pAudio = std::make_unique<AudioSys>();
	FMOD_RESULT result = FMOD::System_Create(&pAudio->system);
	(void)result;
	pAudio->system->init(MAX_CHANNEL, FMOD_INIT_NORMAL, NULL); // creates 64 sound channels 
	pAudio->system->createSoundGroup(NULL, &pAudio->GroupBGM); // background sound 
	pAudio->system->createSoundGroup(NULL, &pAudio->GroupSFX); // normal sounds 

	Game_Configuration g_Config = SEReadGameConfig(CONFIG_PATH "GameConfig.json");
	pAudio->m_volume = g_Config.BGM_Volume/100.f;
}

void SEAudioUpdate()
{
	if (pAudio)
		pAudio->system->update();
}

void SEAudioExit()
{
	// left empty. garbage done by unique ptr.
}

AudioSys* SEGetAudioHandle()
{
	if (pAudio)
	{
		return pAudio.get();
	}
	return 0;
}

void SEPlaySound(AUDIOTYPE AudioType, std::filesystem::path path)
{
	if (pAudio)
	{
		SEAudio* audio_tmp = SEGetAudio(AudioType, path.filename().string());
		if (audio_tmp->isPlaying_BGM == false && AudioType == AUDIOTYPE::BGM && audio_tmp->isBGM)
		{
			pAudio->system->playSound(audio_tmp->track, 0, 0, &audio_tmp->chn);
			audio_tmp->chn->setVolume(pAudio->m_volume);
			audio_tmp->isPlaying_BGM = true;
		}
		if (AudioType == AUDIOTYPE::SFX && audio_tmp->isSFX)
		{
			pAudio->system->playSound(audio_tmp->track, 0, 0, &audio_tmp->chn);
			audio_tmp->chn->setVolume(pAudio->m_volume);
		}
	}
}

void SEPlaySound(AUDIOTYPE AudioType, const char* track)
{
	if (pAudio)
	{
		std::filesystem::path tmp_track = track;
		SEAudio* audio_tmp = SEGetAudio(AudioType, tmp_track);
		if (audio_tmp->isPlaying_BGM == false && AudioType == AUDIOTYPE::BGM)
		{
			pAudio->system->playSound(audio_tmp->track, 0, 0, &audio_tmp->chn);
			audio_tmp->chn->setVolume(pAudio->m_volume);
			audio_tmp->isPlaying_BGM = true;
		}
		if (AudioType == AUDIOTYPE::SFX)		// audio_tmp->isPlaying_SFX == false, no where its false
		{
			pAudio->system->playSound(audio_tmp->track, 0, 0, &audio_tmp->chn);
			audio_tmp->chn->setVolume(pAudio->m_volume);
		}
	}
}

void SEPauseSound(AUDIOTYPE AudioType, std::filesystem::path path)
{
	SEAudio* audio_tmp = SEGetAudio(AudioType, path);
	if (audio_tmp)
	{
		pAudio->system->playSound(audio_tmp->track, 0, 1, &audio_tmp->chn);
	}
	/*if (audio_tmp)
	{
		audio_tmp->chn->getPaused(&audio_tmp->isPaused);
		if (audio_tmp->isPaused == false)
			audio_tmp->chn->setPaused(1);
	}*/
}

void SEPauseSound(AUDIOTYPE AudioType, std::string track)
{
	SEAudio* audio_tmp = SEGetAudio(AudioType, track);
	if (audio_tmp)
	{
		audio_tmp->chn->getPaused(&audio_tmp->isPaused);
		if (audio_tmp->isPaused == false)
			audio_tmp->chn->setPaused(1);
	}
}

void SEUnpauseSound(AUDIOTYPE AudioType, std::filesystem::path path)
{
	SEAudio* audio_tmp = SEGetAudio(AudioType, path);
	if (audio_tmp)
	{
		if (audio_tmp->isPaused)
			audio_tmp->chn->setPaused(0);
	}
}

void SEUnpauseSound(AUDIOTYPE AudioType, std::string track)
{
	SEAudio* audio_tmp = SEGetAudio(AudioType, track);
	if (audio_tmp)
	{
		if (audio_tmp->isPaused)
			audio_tmp->chn->setPaused(0);
	}
}

void SEStopSound(AUDIOTYPE AudioType, std::string track)
{
	SEAudio* audio_tmp = SEGetAudio(AudioType, track);
	assert(audio_tmp);
	if (audio_tmp)
	{
		audio_tmp->chn->stop();
		audio_tmp->isPlaying_BGM = false;
	}
}

void SEStopSound(AUDIOTYPE AudioType, std::filesystem::path path)
{
	SEAudio* audio_tmp = SEGetAudio(AudioType, path);
	if (audio_tmp)
	{
		audio_tmp->chn->stop();
		audio_tmp->isPlaying_BGM = false;
	}
}

void SESetVolume(AUDIOTYPE AudioType, std::string track, float volume)
{
	SEAudio* audio_tmp = SEGetAudio(AudioType, track);
	if (audio_tmp)
	{
		audio_tmp->chn->setVolume(volume);
	}
}

void SESetAllVolume(float _volume)
{
	
	//pAudio->GroupBGM->setVolume(_volume);
	//pAudio->GroupSFX->setVolume(_volume);

	auto& bgm_ARsc = SEGetAudioBGMResources();
	for (auto& bgms : bgm_ARsc) {
		SEAudio* bgm_temp = SEGetAudio(AUDIOTYPE::BGM, bgms.first);
		if (bgm_temp) {
			bgm_temp->chn->setVolume(_volume);
#if _DEBUG
			float tVol{};
			bgm_temp->chn->getVolume(&tVol);
			std::cout << bgms.first << " : " << tVol << std::endl;
#endif // _DEBUG


		}
	}

	auto& sfx_ARsc = SEGetAudioSFXResources();
	for (auto& sfxs : sfx_ARsc) {
		SEAudio* sfx_temp = SEGetAudio(AUDIOTYPE::SFX, sfxs.first);
		if (sfx_temp) {
			sfx_temp->chn->setVolume(_volume);
#if _DEBUG
			float tVol{};
			sfx_temp->chn->getVolume(&tVol);
			std::cout << sfxs.first << " : " << tVol << std::endl;
#endif // _DEBUG
		}
	}

	pAudio->m_volume = _volume;
}

float SEGetVolume(AUDIOTYPE AudioType, std::string track)
{
	SEAudio* audio_tmp = SEGetAudio(AudioType, track);
	if (audio_tmp)
	{
		audio_tmp->chn->getVolume(&audio_tmp->volume);
		return audio_tmp->volume;
	}
	return 0;
}

float SEgetGlobalVolume()
{
	return pAudio->m_volume;
}
