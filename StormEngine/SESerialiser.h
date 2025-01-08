/******************************************************************************//**
 * \file		SESerialiser.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (80%)
 *
 * \brief		Serialiser class for json files
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEFactory.h"
using namespace rapidjson;

#define PARENT_PATH "../"
#define CURRENT_PATH "./"
#define ASSETS_PATH CURRENT_PATH "Assets/"
#define CONFIG_PATH ASSETS_PATH "Config/"
#define BGX_PATH ASSETS_PATH "BGX/"
#define FONT_PATH ASSETS_PATH "Fonts/"
#define SFX_PATH ASSETS_PATH "SFX/"
#define SCENE_PATH ASSETS_PATH "Scenes/"
#define TEXTURE_PATH ASSETS_PATH "Textures/"
#define OBJLIST_PATH SCENE_PATH "Objective List/"
#define HELP_PATH CONFIG_PATH "HELPJSON/"
#define PREFAB_PATH ASSETS_PATH "Prefabs/"
#define PREFAB_FILE PREFAB_PATH "Prefabs.json"

static const int MAX_Action_Count = static_cast<int>(ACTION::ACTION_ENUM_COUNT);
static LEVEL Level;
LEVEL GetLevelForJSON();
void SetLevelForJSON(const LEVEL& _level);

std::string LEVEL_STRING(LEVEL elevel);

enum Error
{
	OK = 0,
	FAIL
};

struct Editor_Configuration {
	std::string Window_title{};
	SEBool Window_fullscreen{};
	SEInt Window_width{};
	SEInt Window_height{};

	SEInt Performance_fps{};
	SEBool Performance_profiler{};

	SEBool Audio_State{};
	std::string Audio_BGMName{};
	SEInt Audio_Volume{};
};

struct Game_Configuration {
	std::string Window_title{};
	SEBool Window_fullscreen{};
	SEInt Window_width{};
	SEInt Window_height{};

	SEInt Performance_fps{};

	SEInt BGM_Volume{};
	SEInt SFX_Volume{};
};

/**
	 * Retrieves the current json file
	 * @return Document from JSON api
 */
Document SEGetJSONDocument(std::string file);

/**
	 * Formats the json
 */
void JsonDocFormat(std::string str);

std::string ID_RENDERLAYER(SERenderLayer E_RenderLayer);

//serialisers //will output all to newest format
void SESerialiseJSON(const std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>>& container, const std::filesystem::path& out);
void SESerialiseJSON(std::vector<GameObject*>& container, std::string out);

//deserialisers //always check for version to make sure application runs.
void SEDeserialiseJSON(std::string file);
LEVEL SEDeserialiseJSON_v1_140223(std::string file);

void SEDeserialiseJSON(std::vector<GameObject*>& container, std::string out);
//void SEDeserialiseJSON_v1_0(std::vector<GameObject*>& container, std::string file);
//void SEDeserialiseJSON_v1_140223(std::vector<GameObject*>& container, std::string file);
//void SEDeserialiseJSON(std::string file, SEBool dialouge_stuff = false); // [Added 1/2/2023]
//void SEDeserialiseJSON(std::vector<GameObject*>& container, std::string file);
void SEDeserialiseJSON_v1_140223(std::vector<GameObject*>& container, std::string file);

/**
	 get all textures from resource manager form a specified directory
 */
std::vector<std::string> SEGetAllTexturesFrom(std::string path);

/**

 */
Editor_Configuration SEReadEditorConfig(std::string file);

/**

 */
void SEWriteEditorConfig(std::string output_file);

/**
	 * read a config file for game
 */
Game_Configuration SEReadGameConfig(std::string input_file);

/**
	 * write a config file for game
 */
void SEWriteGameConfig(std::string output_file);

/**
	 * read objective list
 */
std::vector<std::string> SEReadObjectiveList(std::filesystem::path filepath);

std::vector <std::string> SEReadList(std::filesystem::path filepath);