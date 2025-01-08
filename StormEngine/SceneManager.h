/******************************************************************************//**
 * \file		SceneManager.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (50%)
 *
 * \brief		Scene Manager
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SESerialiser.h"
#include "SEAudio.h"
#include "SELayerStack.h"
#include <vector>
#include "SEWaypointSystem.h"
#include <ConsoleColor.h>


#define p_Scene SceneManager::GetInstance()
static std::map < std::string, SEVec2> positions;

class SceneManager {
public:
	// key: json, val: container
	//static std::unordered_map<std::string, std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>>> Scenes;
	static std::unique_ptr<SceneManager>& GetInstance();
	void LoadScene(std::string scene);
	//void SaveScene(std::string filepath);
	void Unload();

	//std::string GetCurrentScene() { return currentScene; }
	//std::string GetPreviousScene() { return previousScene; }
	//std::string GetNextScene() const { return nextScene; }
	//std::string GetSavedScene() const { return savedScene; }

	// void SetNextScene(std::string _nextScene)
	// {
	//     std::cout << _nextScene << std::endl;
	//     nextScene = _nextScene;
	// }

	bool nextSceneLoaded;
	
	const LEVEL& GetCurrentScene() { return current; }
	const LEVEL& GetPreviousScene() { return prev; }
	const LEVEL& GetNextScene() { return next; }
	void SetNextScene(const LEVEL& _level) { next = _level; }
	void SetCurrentScene(const LEVEL& _level)
	{
		prev = current;     // save prev scene.
		current = _level;
	}
	std::vector<std::string> _Scenes;
	void Init()
	{   
		// Load all scenes during first boot.
		_Scenes.push_back("Splash.json");
		_Scenes.push_back("ExitScreen_Menu.json");
		//_Scenes.push_back("Screen_Win.json");
		_Scenes.push_back("Pause_Menu.json");
		//_Scenes.push_back("Game_Over_Menu.json");
		_Scenes.push_back("Settings_Menu.json");
		_Scenes.push_back("How_To_Play_Menu.json");
		_Scenes.push_back("MainMenu.json");
		_Scenes.push_back("Cutscenes.json");
		_Scenes.push_back("Level_1.json");
		_Scenes.push_back("Ending_Cutscenes.json");
		_Scenes.push_back("Credits.json");

		for (const auto& scene : _Scenes)
		{
			// deserialiser will auto load into repective scene
#if _DEBUG
			std::cout << yellow << "Loading: " << scene << white << std::endl;
#endif
			SEDeserialiseJSON(std::string(SCENE_PATH + scene));
		}

		p_Scene->prev = p_Scene->current = LEVEL::SPLASH;
		auto container = SEGetAllLevels();
	}

private:
	LEVEL current, prev, next;
	//std::string currentScene, previousScene, savedScene, nextScene;
};



/*!
 *  Exits Scene Manager
 */
inline void SESceneMgrExit()
{
	if (p_Scene)
	{
		p_Scene.reset();
	}
}

/*!
 *  Init to Specified Scene
 *
 *      @param [in] scene
 */
 //inline void SESceneInit(const LEVEL& _level)
 //{
 //    if (scene.size() > 0)
 //    {
 //        p_Scene->SetNextScene(_level);
 //        p_Scene->LoadScene(_level);
 //        SEPlaySound(AUDIOTYPE::BGM, "backgroundmusic.ogg");
 //    }
 //}

void inline SESave(std::map<std::string, SEVec2>& _positions, std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>>& goArr)
{
	for (const auto& obj : goArr)
	{
		Transform* pt = (Transform*)obj.second.second->getComponent(CT_TRANSFORM);
		if (pt)
		{
			if (obj.first == "NPC Lala45")
			{
				_positions.emplace(std::make_pair(obj.first, pt->getTranslate()));
			}

			if (obj.first == "NPC Muse32")
			{
				_positions.emplace(std::make_pair(obj.first, pt->getTranslate()));
			}

			if (obj.first == "Player9")
			{
				_positions.emplace(std::make_pair(obj.first, pt->getTranslate()));
			}
		}
	}
}

void inline SELoad(std::map<std::string, SEVec2>& _saved, std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>>& goArr)
{
	(void)_saved;
	for (const auto& obj : goArr)
	{
		SEVec2 Pos{};
		Transform* pt = (Transform*)obj.second.second->getComponent(CT_TRANSFORM);
		if (pt)
		{
			if (obj.first == "NPC Lala45")
			{
				Pos = positions.at(obj.first);
				pt->setTranslate(Pos);
			}

			if (obj.first == "NPC Muse32")
			{
				Pos = positions.at(obj.first);
				pt->setTranslate(Pos);
			}

			if (obj.first == "Player9")
			{
				Pos = positions.at(obj.first);
				pt->setTranslate(Pos);
			}
		}
	}
	positions.clear();
}

/*!
 *  Update Scene Mgr
 */
static unsigned int cutscene_idx = 1;
static unsigned int ending_idx = 1;
inline void SESceneUpdate()
{
	// check for events each frame, update the scene per request.
	p_Scene->nextSceneLoaded = 0;
	if (p_Scene->GetCurrentScene() != LEVEL::LEVEL_1) { 
		SetCameraPosition(0, 0); 
		SEStopSound(BGM, std::string("backgroundmusic.ogg"));
		SEPlaySound(BGM, "menu.ogg");
	}
	if (p_Scene->GetCurrentScene() == LEVEL::LEVEL_1)
	{
		SEStopSound(AUDIOTYPE::BGM, std::string{ "menu.ogg" });
		SEPlaySound(AUDIOTYPE::BGM, "backgroundmusic.ogg");
		if (SEIsKeyTriggered(SE_KEY_ESCAPE))
		{
			p_Scene->SetCurrentScene(LEVEL::PAUSE);
			return;
		}
	}
	if (p_Scene->GetCurrentScene() == LEVEL::PAUSE)
	{
		if (SEIsKeyTriggered(SE_KEY_ESCAPE))
		{
			auto prev = p_Scene->GetPreviousScene();
			p_Scene->SetCurrentScene(prev);
			return;
		}
	}

	if (p_Scene->GetCurrentScene() == LEVEL::CUTSCENE)
	{
		
		if (SEIsMouseLeftPressed())
		{
			cutscene_idx++;
#if _DEBUG
			std::cout << "advance cutscene\n";
#endif // _DEBUG


		}

		const auto& cutscenes = SEFindObjectsByLevel(LEVEL::CUTSCENE);
		for (const auto& obj : cutscenes)
		{
			const std::string& name = obj.first;
			GameObject* go = obj.second;

			if (go)
			{
				Texture* pTexture = (Texture*)go->getComponent(CT_TEXTURE);
				if (pTexture)
				{
					std::string cutscene_name = "Cutscene " + std::to_string(cutscene_idx);
					if(name == cutscene_name) pTexture->isRendering = 1;
					else pTexture->isRendering = 0;
				}
			}
		}

		if (cutscene_idx > 10)
		{
			cutscene_idx = 1;
			p_Scene->SetCurrentScene(LEVEL::LEVEL_1);
			// waypoint init 
			PathFinding::WayPointSystem::Init();
			return;
		}
	}

	if (p_Scene->GetCurrentScene() == LEVEL::ENDING)
	{

		SEStopSound(AUDIOTYPE::BGM, std::string{ "backgroundmusic.ogg" });
		SEPlaySound(AUDIOTYPE::BGM, "menu.ogg");
		if (SEIsMouseLeftPressed())
		{
			ending_idx++;
#if _DEBUG
			std::cout << "advance cutscene\n";
			std::cout << ending_idx << std::endl;
#endif // _DEBUG


		}

		const auto& cutscenes = SEFindObjectsByLevel(LEVEL::ENDING);
		for (const auto& obj : cutscenes)
		{
			const std::string& name = obj.first;
			GameObject* go = obj.second;

			if (go)
			{
				Texture* pTexture = (Texture*)go->getComponent(CT_TEXTURE);
				if (pTexture)
				{
					std::string cutscene_name = "Ending " + std::to_string(ending_idx);
					if (name == cutscene_name) pTexture->isRendering = 1;
					else pTexture->isRendering = 0;
				}
			}
		}

		if (ending_idx > 6)
		{
			ending_idx = 1;
			p_Scene->SetCurrentScene(LEVEL::MAIN_MENU);
			// waypoint init 
			return;
		}
	}
}