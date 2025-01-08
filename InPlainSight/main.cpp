/******************************************************************************//**
 * \file			main.cpp
 * \author 			Rommel Sim (100%)
 * \par    	email: zhenpengrommel.sim@digipen.edu
 * \brief	This file contains the declaration for editor.
 * \date   	January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

// Includes


#include <SEEngine.h>
// enable optimus!
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 1;
	_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
int main()
{
	

#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	SEWriterInit("InPlainSight_FileDump.txt");
	Game_Configuration g_Config = SEReadGameConfig(CONFIG_PATH "GameConfig.json");
	SEGfxInit(g_Config.Window_width, g_Config.Window_height, g_Config.Performance_fps, 0);
	SESetFullScreen(1);

	//glDepthFunc(GL_LEQUAL);
	SEAudioInit();
	SEAnimationInit();

	SELoadAllBgm(BGX_PATH);
	SELoadAllSfx(SFX_PATH);
	SELoadAllFonts(FONT_PATH);


	/*std::vector<std::string> Scenes = {
		"ExitScreen_Menu.json", "Screen_Win.json", "Pause_Menu.json", "Game_Over_Menu.json",
		"Settings_Menu.json", "How_To_Play_Menu.json", "MainMenu.json"
	};

	std::string path = "./Assets/Scenes/";
	for (const auto& scene : Scenes)
	{
		SEDeserialiseJSON(std::string(path + scene));
	}*/
	// Load all scenes during boot.
	p_Scene->Init();
	SEDialogueInit();

	// Check if all scens are all loaded?
	//auto container = SEGetAllLevels();

	//SEDeserialiseJSON(std::string(path + scene));
	//SESceneInit("MainMenu.json");
	SELayerStackInit();
	//SEDialogueManager->SELoadDialogueTree("./Assets/Scenes/DialogueTreejson");
	//SEDialogueInit("./Assets/Scenes/DialogueTree.json");

	//SEPlaySound(AUDIOTYPE::BGM, "backgroundmusic.ogg");
	SEPlaySound(AUDIOTYPE::BGM, "menu.ogg");
	while (SECheckWindowIsClose())
	{

		// Update to the corresponding scenes before start of each frame.

		//auto container = SEGetAllLevels();

		// Indicate start of frame
		SEFrameStart();
		// Update 

		if (SEIsKeyTriggered(SE_KEY_F1)) {
			IG_debug = !IG_debug;
		}

		IG_debug = true; // i'll remove this once everything over

		if (IG_debug) {
			//Set game progress
			if (SEIsKeyTriggered(SE_KEY_0))
				ToggleALLGameProgress();

			if (SEIsKeyTriggered(SE_KEY_1))
				ToggleGameProgressLv1(ProgressFlags_LV1::KEY_REPLACE_NOTE);
			if (SEIsKeyTriggered(SE_KEY_2))
				ToggleGameProgressLv1(ProgressFlags_LV1::KEY_DISPOSE_GLASS);
			if (SEIsKeyTriggered(SE_KEY_3))
				ToggleGameProgressLv1(ProgressFlags_LV1::KEY_REPLACE_PHOTO);
			if (SEIsKeyTriggered(SE_KEY_4))
				ToggleGameProgressLv2(ProgressFlags_LV2::REPLACE_BLOOD_WITH_PAINT);
			if (SEIsKeyTriggered(SE_KEY_5))
				ToggleGameProgressLv2(ProgressFlags_LV2::CLEAN_BLOOD_ON_KNIFE);
			if (SEIsKeyTriggered(SE_KEY_6))
				ToggleGameProgressLv2(ProgressFlags_LV2::REPLACE_HORN_WITH_CANDY);
		}

		if (SEIsKeyTriggered(SE_KEY_P))
		{
			showFPS = !showFPS; // toggle the state of the showFPS variable
		}

		if (SEIsKeyTriggered(SE_KEY_F))
			SESetFullScreen(1);
		if (SEIsKeyTriggered(SE_KEY_G))
			SESetFullScreen(0);

		//if (SEIsKeyTriggered(SE_KEY_8))		SESetWindowWidthHeight(1280, 720);			// <---- WTF!?!
		//else if (SEIsKeyTriggered(SE_KEY_9))SESetWindowWidthHeight(1920, 1080);
		//else if (SEIsKeyTriggered(SE_KEY_0))SESetWindowWidthHeight(2560, 1440);

		//SEDialogueTestToggle();

		//const std::unordered_multimap<std::string, GameObject*>& container = SEFindObjectsByLevel(LEVEL::MAIN_MENU);
		auto _curr = p_Scene->GetCurrentScene();
		const std::unordered_multimap<std::string, GameObject*>& container = SEFindObjectsByLevel(_curr);

		if (SEPlayerQuit()) break;				// player quits. contiune to free memory process.
		//SEPopulateDialogueTree();
		SEDialogueUpdate();
		SELayerStackUpdate(container);
		SEBehaviourUpdate(_curr);
		SEGfxUpdate();							// -> shifted down from above sedialougeupdate() to here ww


		// Rendering
		SELayerStackDraw(_curr);
		SESceneUpdate();						// -> shift down from before gfxupdate()
		// Indicate end of frame
		SEFrameEnd();


	}

	SEDialogueExit();
	SEFactoryExit();
	SEResourceMgrExit();
	SEGfxExit();

}

//int main()
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	SEWriterInit("InPlainSight_FileDump.txt");
//
//	// Init Core Systems!
//	Game_Configuration g_Config = SEReadGameConfig("./Config/GameConfig.json");
//
//	SESceneInit("BG.json");
//	SEGfxInit(g_Config.Window_width, g_Config.Window_height, g_Config.Performance_fps, 0);
//	SEAudioInit();
//	SEAnimationInit();
//	
//	p_Scene->SetNextScene("BG.json");
//
//	// [Render Layers - System]
//	SELayerStackInit();
//	SELoadAllFonts("./Assets/Fonts");
//	
//	SetCameraPosition(0, 0);
//
//	while (SECheckWindowIsClose())
//	{
//		p_Scene->LoadScene(p_Scene->GetNextScene());
//
//		while ((p_Scene->GetNextScene() == p_Scene->GetCurrentScene()) || SECheckWindowIsClose())
//		{
//			auto container = SEGetAllLevels();
//			SEFrameStart();									// Indicate start of frame
//
//			SEGfxUpdate();
//			SEAudioUpdate();
//			
//			SELayerStackUpdate(container);
//
//			for (const auto& obj : goArray)
//			{
//				GameObject* go = obj.second.second;
//				if (go)
//				{
//					if (go->m_behaviour)
//						go->m_behaviour->Update();
//				}
//			}
//			if (SEIsKeyTriggred(SE_KEY_ESCAPE))
//				break;
//
//			if (SEIsKeyTriggred(SE_KEY_H)) // test switch scene
//			{
//				p_Scene->SetNextScene("Layout_Test_v8.json");
//			}
//			SELayerStackDraw();
//
//			SEFrameEnd();		// Indicate end of frame
//		}
//
//		SEGfxExit();
//		SEResourceMgrExit();
//		SEFactoryExit();
//	}
//		
//}



/*
	* For ref
	*
	*
	* // Fixed on 16/12
		for (int i = 0; i <= COL; ++i)
		{
			for (int j = 0; j <= ROW; ++j)
			{
				int minX = -500;
				int minY = -500;
				int maxX = TileWidth * i;
				int maxY = TileHeight * j;

				//int maxX = TileWidth * i;
				//int maxY = TileHeight * j;

				SEDrawQuad(-500, -500, maxX - 500, maxY - 500, 1, 0, 0);
			}
		}
	//rapidjson::Document document;
	//
	//// define the document as an object rather than an array
	//document.SetObject();
	//
	//// create a rapidjson array type with similar syntax to std::vector
	//rapidjson::Value array(rapidjson::kArrayType);
	//
	//// must pass an allocator when the object may need to allocate memory
	//rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	//
	//// chain methods as rapidjson provides a fluent interface when modifying its objects
	//array.PushBack("hello", allocator);
	//array.PushBack("rommel", allocator);//"array":["hello","world"]
	//
	//document.AddMember("Name", "XYZ", allocator);
	//document.AddMember("Rollnumer", 2, allocator);
	//document.AddMember("array", array, allocator);
	//
	//// create a rapidjson object type
	//rapidjson::Value object(rapidjson::kObjectType);
	//object.AddMember("Math", "50", allocator);
	//object.AddMember("Science", "70", allocator);
	//object.AddMember("English", "50", allocator);
	//object.AddMember("Social Science", "70", allocator);
	//document.AddMember("Marks", object, allocator);
	////	fromScratch["object"]["hello"] = "Yourname";
	//
	//// For std::cout
	//StringBuffer strbuf;
	//Writer<StringBuffer> writer(strbuf);
	//document.Accept(writer);
	//std::cout << strbuf.GetString() << std::endl;
	//
	//// For writing to file
	//std::filesystem::path solDir;
	//// get Solution Dir
	//std::string path = SOLUTIONDIR;
	//path += "/Assets/Scenes/test4.json";
	//std::ofstream ofs(path);
	//OStreamWrapper osw(ofs);
	//Writer<OStreamWrapper> writer_file(osw);
	//document.Accept(writer_file);

		//const Value& Level_One = Level_1_JSON["Level_1"];
	//for (SizeType i = 0; i < Level_One.Size(); ++i)
	//{
	//	if (Level_One[i].IsObject())
	//	{
	//		for (SizeType j = 0; j < Level_One[i]["Color"].Size(); ++j)
	//		{
	//			r = Level_One[i]["Color"][0].GetFloat();
	//			g = Level_One[i]["Color"][1].GetFloat();
	//			b = Level_One[i]["Color"][2].GetFloat();
	//		}
	//		std::string name = Level_One[i]["Name"].GetString();
	//		std::string texture = Level_One[i]["Texture"].GetString();
	//		std::cout << name << ", " << texture << ", " << r << g << b << std::endl;
	//	}
	//}

	//SEDeserialiseJSON("./Assets/Scenes/Level_1.json");
	*
	*
	* /*
		*
		* dx = 1/width
		* dy = 1/height
		*
		* Frame N
		* TL = { x * dx, y * dy)
		* TR = { x + 1 * dx, y * dy)
		* BR = { x + 1 * dx, y + 1 * dy)
		* BL = { x * dx, y + 1 * dy)
		*
		// Frame 0
		SEVec2 TL = {0				, 1};
		SEVec2 TR = {0 + 1.0f/width , 1};
		SEVec2 BL = {0				, 1.0f - 1.0f/height};				// {
		SEVec2 BR = {0 + 1.0f/width	, 1.0f - 1.0f/height};

		// Frame 1
		SEVec2 TL1 = { 0 + 1.0f / width	, 1.0f };
		SEVec2 TR1 = { 0 + 2.0f / width , 1.0f };
		SEVec2 BL1 = { 0 + 1.0f / width , 1.0f - 1.0f / height };
		SEVec2 BR1 = { 0 + 2.0f / width	, 1.0f - 1.0f / height };

		// Frame N
		SEVec2 TL1 = { 0 + 1.0f / width	, 1.0f };
		SEVec2 TR1 = { 0 + 2.0f / width , 1.0f };
		SEVec2 BL1 = { 0 + 1.0f / width , 1.0f - 1.0f / height };
		SEVec2 BR1 = { 0 + 2.0f / width	, 1.0f - 1.0f / height };


		// Frame 9
		SEVec2 TL9 = { 0 + 0.0f / width	, 1.0f - 1.0f / height };
		SEVec2 TR9 = { 0 + 1.0f / width , 1.0f - 1.0f / height };
		SEVec2 BL9 = { 0 + 0.0f / width , 1.0f - 2.0f / height };
		SEVec2 BR9 = { 0 + 1.0f / width	, 1.0f - 2.0f / height };

		//
		(AUDIOTYPE::BGM, "./Assets/BGX/bgm.mp3");

	//SECreateObject<LEVEL::LEVEL_0>("Sprite", 2, COMPONENT_TYPE::CT_TEXTURE, COMPONENT_TYPE::CT_TRANSFORM);
	//SEAddAnimationToObject("Sprite", "./Assets/Textures/MainCharacter_Idle_Paper_80.png", ACTION::ACTION_IDLE, 80, 0.5f);
	//SEAddAnimationToObject("Sprite", "./Assets/Textures/MainCharacter_Walk_30.png", ACTION::ACTION_WALKING, 30, 0.3f);
	//SEAddAnimationToObject("Sprite", "./Assets/Textures/MainCharacter_Clean_Broom_24.png", ACTION::ACTION_CLEANING, 24, 1.0f);

	//SECreateObject<LEVEL::LEVEL_0>("Bunny", 2, COMPONENT_TYPE::CT_TEXTURE, COMPONENT_TYPE::CT_TRANSFORM);
	//SESetObjectPosition("Bunny", 100, 100);
	//SEAddAnimationToObject("Bunny", "./Assets/Textures/NPC_Bunny_Walk_27.png", ACTION::ACTION_WALKING, 27, 0.1f);
	//SEAddAnimationToObject("Bunny", "./Assets/Textures/NPC_Bunny_Idle_40.png", ACTION::ACTION_IDLE, 40, 0.15f);

*/