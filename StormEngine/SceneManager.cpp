/******************************************************************************
 * \file		SEAudio.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (40%)
 * \par
 * \brief		Scene Manager
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#include "pch.h"
#include <SceneManager.h>
#include <SEDialogue.h>

//std::unordered_map<std::string, std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>>> SceneManager::Scenes;
std::unique_ptr<SceneManager> _instance{ nullptr };

std::unique_ptr<SceneManager>& SceneManager::GetInstance()
{
    if (_instance == nullptr)
    {
        _instance = std::make_unique<SceneManager>();
    }
    return _instance;
}

void SceneManager::LoadScene(std::string scene)
{
    //previousScene = currentScene;
    //nextScene = scene;
    //
    //std::string path = "./Assets/Scenes/";
    //
    // SETransferDialogueObjects(); // transfer dialogue objects to a buffer array. (before it gets deleted in [SEDeserialiseJSON()]
    //SEDeserialiseJSON(std::string(path + scene));
    ////SEDialogueLoad("./Assets/Scenes/DialogueTree.json");
    //
    //currentScene = nextScene;
    //
    //nextSceneLoaded = 1;            // wronglogic, im killing myself .
}

//void SceneManager::SaveScene(std::string filepath)
//{
//    std::string path = "./Assets/Scenes/";
//    SESerialiseJSON(goArray, (path + filepath).c_str());
//    savedScene = currentScene;
//}

void SceneManager::Unload()
{
    //needs to remove all stuff on screen
    //for (auto& obj : goArray)
    //{
    //    //std::string name = obj.first;
    //    SEDeleteAllObject();
    //}
    //auto tmp = goArray;
}