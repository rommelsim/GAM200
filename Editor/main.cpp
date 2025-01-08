/******************************************************************************//**
 * \file			main.cpp
 * \author 			Loke Kheng Ray (50%)
 * \co-author 		Rommel Sim (50%)
 * \par    	email: l.khengray@digipen.edu
 * \par    	email: kaiquan.chua@digipen.edu
 * \brief	This file contains the declaration for editor.
 * \date   	January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

// Includes
#include <SEEngine.h>
#include "SEEditor.h"

#define AAAB_RENDERING 0
#define EDITOR 1

int main()
{
#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	SEWriterInit("InPlainSight_FileDump.txt");
	Editor_Configuration e_Config = SEReadEditorConfig(CONFIG_PATH "EditorConfig.json");
	SEGfxInit(e_Config.Window_width, e_Config.Window_height, e_Config.Performance_fps, EDITOR);
	SE_UIInit();
	SEAudioInit();
	SEAnimationInit();
	
	SEDialogueInit();

	
	/*SEDialogueInit("./Assets/Scenes/TestSavingDial.json");*/

	//SEDialogueNode* new_node = SEDialogueManager->SECreateEmptyNodeReturn("Node #4");
	//new_node->SEAddNewOptionObject("New Option #4a", "I am a new option", 0, SEVec2(100, 100), SEVec2(-433, 250), SEVec2(95, 100), SEVec2(-433, 250));
	//new_node->SEAddNewPromptObject("New Prompt #4b", "Hello World", SEVec2(100, 100), SEVec2(-433, 250), SEVec2(95, 100), SEVec2(-433, 250));
	//new_node->SEUpdatePrompt("New Prompt #4b", "Hello I Changed...");
	//new_node->SEUpdateOption("New Option #4a", "Hello I have changed too...", 1, "Node #1");
	//new_node->SEDeleteOption("New Option #4a");
	//new_node->SEDeletePrompt();

	//SEDialogueInit("./Assets/Scenes/Level_1.json");

	// Test [Draw Box]
	//SELoadTexture("./Assets/Textures/UI_Dialogue_TextBox.png");

	SELayerStackInit();


	SELoadAllBgm("./Assets/BGX");
	SELoadAllSfx("./Assets/SFX");
	SELoadAllFonts("./Assets/Fonts");

	SetCameraPosition(0, 0);
	//SESetCameraHeight(1000);

	while (SECheckWindowIsClose())
	{
		// Indicate start of frame
		SEFrameStart();

		// Update 
		SEGfxUpdate();
		SE_UIUpdate();
	

		auto current_lvl = GetLevelForJSON();

		const std::unordered_multimap<std::string, GameObject*>& container = SEFindObjectsByLevel(current_lvl);

		SEDialogueUpdate();
		SELayerStackUpdate(container);
		//SEBehaviourUpdate(current_lvl);

		SEVec2 currmpos{}, prevmpos{};
		static float x = 0;
		static float y = 0;
		static float cam_height = (float)SEGetCameraHeight();
		if (SEMouseScroll() > 0 && cam_height > 50 && CheckHoveredViewport()) {
			/*currmpos.x = (float)SEGetCursorX();
			currmpos.y = (float)SEGetCursorY();*/

			/*if (SECheckCursorDeadZone(currmpos, prevmpos, 10)) {
				x = SEGetImguiMouse().x;
				y = SEGetImguiMouse().y;
			}*/

			cam_height -= 50;

			ResetMouseScroll();
			/*prevmpos.x = currmpos.x;
			prevmpos.y = currmpos.y;*/
		}

		if (SEMouseScroll() < 0 && cam_height < 4000 && CheckHoveredViewport()) {
			/*x -= 10;
			y -= 10;*/
			cam_height += 50;
			ResetMouseScroll();
		}

		if (SEIsKeyTriggered(SE_KEY_R)) {
			x = 0;
			y = 0;
			cam_height = 1000;
		}

		if (SEIsKeyTriggered(SE_KEY_LEFT))
		{
			x -= 100;
			std::cout << "left\n";
		}
		if (SEIsKeyTriggered(SE_KEY_RIGHT))
		{
			x += 100;
			std::cout << "right\n";
		}
		if (SEIsKeyTriggered(SE_KEY_UP))
		{
			y += 100;
			std::cout << "up\n";
		}
		if (SEIsKeyTriggered(SE_KEY_DOWN))
		{
			y -= 100;
			std::cout << "down\n";
		}
		SESetCameraHeight(cam_height);
		SetCameraPosition(x, y);

		// Imgui Rendering
		SELayerStackDraw(current_lvl);
		SE_UIDrawMainBar();
		SE_UIDrawFileManager();
		SE_UIDrawSettingsWindow();
		SE_UIDrawLayerRender();
		SE_UIDrawHierarchy();
		SE_UIDrawPalette();
		SE_UIDrawCreatePrefab();
		SE_UIDrawObjInspector();
		SE_ContentBrowser();

		if (ImGui::Begin("Debug")) {
			
			ImGui::Text("FPS:(%0.2f)",SEGetFPS());
		}
		ImGui::End();
		

		// Rendering
		
		SE_UIRender();

		// Indicate end of frame
		SEFrameEnd();
	}

	SEDialogueExit();
	SEGfxExit();
	SEResourceMgrExit();
	SEFactoryExit();
	SESceneMgrExit();
	SE_UIExit();

}

// Entry Point
//int main()
//{
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	
//	// Init Core Systems!
//	SEInit();
//	SE_UIInit();	
//	//SEDeserialiseJSON("./Assets/Scenes/Level_1.json");
//	p_Scene->SetNextScene("EmptyScene.json");
//	for (const auto& obj : goArray)
//	{
//		GameObject* go = obj.second.second;
//		if (go)
//		{
//			if(go->m_behaviour)
//				go->m_behaviour->Init();
//		}
//	}
//
//	SELoadAllFonts("./Assets/Fonts");
//	SELoadAllBgm("./Assets/BGX");
//	SELoadAllSfx("./Assets/SFX");
//
//	
// (AUDIOTYPE::BGM, std::string("backgroundmusic.ogg"));
//
//
//	SELayerStackInit();
//	SEPrintLayerElements();
//	SetCameraPosition(0, 0);
//	auto textures = SEGetTextureResources();
//	auto fonts = SEGetFontResources();
//
//	SEVec2 currmpos{}, prevmpos{};
//	
//	while (SECheckWindowIsClose())
//	{
//		p_Scene->LoadScene(p_Scene->GetNextScene());
//
//		while ((p_Scene->GetNextScene() == p_Scene->GetCurrentScene()) && SECheckWindowIsClose())
//		{
//			auto container = SEGetAllLevels();
//			SEFrameStart();
//
//			SEGfxUpdate();
//
//			SE_UIUpdate();
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
//			SELayerStackUpdate(container);
//
//
//			static int x = 0;
//			static int y = 0;
//			static float cam_height = SEGetCameraHeight();
//
//
//			if (SEMouseScroll() > 0 && cam_height > 0 && CheckHoveredViewport()) {
//				currmpos.x = SEGetCursorX();
//				currmpos.y = SEGetCursorY();
//
//				if (SECheckCursorDeadZone(currmpos, prevmpos, 10)) {
//					x = SEGetImguiMouse().x;
//					y = SEGetImguiMouse().y;
//				}
//
//				cam_height -= 50;
//
//				ResetMouseScroll();
//				prevmpos.x = currmpos.x;
//				prevmpos.y = currmpos.y;
//			}
//
//			if (SEMouseScroll() < 0 && cam_height < 4000 && CheckHoveredViewport()) {
//				x -= 10;
//				y -= 10;
//				cam_height += 50;
//				ResetMouseScroll();
//			}
//
//			/*
//			Reset Camera Position and Height
//			*/
//			//if (SEIsKeyTriggred(SE_KEY_R)) {
//			//	x = 0;
//			//	y = 0;
//			//	cam_height = 1000;
//			//}
//
//			if (SEIsKeyTriggred(SE_KEY_F))
//				SESetFullScreen(1);
//			if(SEIsKeyTriggred(SE_KEY_G))
//				SESetFullScreen(0);
//
//			if (SEIsKeyTriggred(SE_KEY_LEFT))
//			{
//				x -= 100;
//				std::cout << "left\n";
//			}
//			if (SEIsKeyTriggred(SE_KEY_RIGHT))
//			{
//				x += 100;
//				std::cout << "right\n";
//			}
//			if (SEIsKeyTriggred(SE_KEY_UP))
//			{
//				y += 100;
//				std::cout << "up\n";
//			}
//			if (SEIsKeyTriggred(SE_KEY_DOWN))
//			{
//				y -= 100;
//				std::cout << "down\n";
//			}
//			SESetCameraHeight(cam_height);
//			SetCameraPosition(x, y);
//			
//			// Use other keys!! 
//			//if (SEIsKeyTriggred(SE_KEY_H)) // test switch scene
//			//{
//			//	p_Scene->SetNextScene("Level_2.json");
//			//}
//
//			//if (SEIsKeyTriggred(SE_KEY_Y)) // test switch scene
//			//{
//			//	p_Scene->SetNextScene("Level_1.json");
//			//}
//
//			/*auto goudybookletter1911 = SEGetFont("./Assets/Fonts/GoudyBookletter1911.ttf");
//			SEDrawFont("Goudy Book Letter 1911", goudybookletter1911, -300, -300, 100, 1, 0, 0);
//
//			auto IndieFlower = SEGetFont("./Assets/Fonts/IndieFlower.ttf");
//			SEDrawFont("IndieFlower", IndieFlower, -200, -200, 100, 1, 0, 0);
//
//			auto scadabold = SEGetFont("./Assets/Fonts/Scada-Bold.ttf");
//			SEDrawFont("Scada Bold", scadabold, -100, -100, 100, 1, 0, 0);
//
//			auto scadabold_italic = SEGetFont("./Assets/Fonts/Scada-BoldItalic.ttf");
//			SEDrawFont("Scada Italic", scadabold_italic, 0, 0, 100, 1, 0, 0);
//
//			auto scadareg = SEGetFont("./Assets/Fonts/Scada-Regular.ttf");
//			SEDrawFont("Scada Regular", scadareg, 100, 100, 100, 1, 0, 0);*/
//
//			SELayerStackDraw();
//
//
//			//SE_UIDrawLevelSelection();
//
//			SE_UIDrawMainBar();
//			SE_UIDrawFileManager();
//			SE_UIDrawSettingsWindow();
//			SE_UIDrawLayerRender();
//			SE_UIDrawHierarchy();
//			SE_UIDrawPalette();
//			SE_UIDrawCreatePrefab();
//			SE_UIDrawObjInspector();
//
//			SE_UIRender();
//			SEFrameEnd();
//		}
//		//p_Scene->Unload();
//
//	}
//	
//	SEExit();
//	SE_UIExit();
//}
