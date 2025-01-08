/******************************************************************************//**
 * \file			SEEngine.cpp
 * \author 			Rommel Sim (100%)
 * \par    			zhenpengrommel.sim@digipen.edu
 *
 * \brief			Wrapper for Engine sub systems
 *
 * \date   			January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#include "pch.h"
#include "SEEngine.h"

#include <filesystem>



void SEInit()
{
	
	SEWriterInit("Editor_FileDump.txt");
	Editor_Configuration initConfig = SEReadEditorConfig("./Config/EditorConfig.json");

	SEGfxInit(initConfig.Window_width, initConfig.Window_height, initConfig.Performance_fps, 1);
	//SEGfxInit(1600, 900, 60, 1);
	SEAudioInit();
}

void SEExit()
{
	//SESerialiseJSON(goArray, "./Assets/Scenes/Level_1.json");

	SEGfxExit();
	//SEAudioExit();
	SEFactoryExit();
	SEResourceMgrExit();
	SEWrite("SEEngine Shutting down\n");
	SEWriterExit();
}
