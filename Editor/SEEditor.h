/******************************************************************************//**
 * \file		SEEditor.h
 * \author 		Loke Kheng Ray	(%)
 * \co-author	Chua Kai Quan	(%)
 * \par    		email: l.khengray@digipen.edu
 * \par    		email: kaiquan.chua@digipen.edu
 * \brief		This file contains the defination for editor
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include <SEEngine.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

void SE_UIInit();
void SE_UIUpdate();
void SE_UIRender();
void SE_UIExit();

// Debug window


// Editor windows
/*
*  - Main Bar
 - File Manager Window
 - Settings
 - Layer selection
 - Hierachy
 - Palette
 - Obj Inspector
*/

// Initialization Functions
void InitializeAvailablePrefabs();
void HandleMouseState();
void SpawnPrefab();

// Helper Functions
bool CheckHoveredViewport();

// Editor Windows
void SE_UIDrawMainBar();
//void SE_UIDrawLevelSelection();
void SE_UIDrawFileManager();
void SE_UIDrawSettingsWindow();
void SE_UIDrawLayerRender();
void SE_UIDrawHierarchy();
void SE_UIDrawPalette();
void SE_UIDrawObjInspector();
void SE_UIDrawCreatePrefab();
void SE_DockSpace();
void SE_Viewport();
void SE_ContentBrowser();

void SE_Guizmo();

// Sub Windows (Appears when certain functions are required)
void SE_NotifWindow();
void SE_SetTextureWindow();
void SE_UIAddComponentsWindow();
void SE_DialogueManagerWindow();
void SE_LoadingWindow(std::string task);
void SE_RenameObjects();


// Component UI functions (Contains the implementation for formatting of the inspector interface)
void COMP_TransformSection(ImVec2 cursorPos, bool is_inspect);
void COMP_CollisionSection(ImVec2 cursorPos, bool is_inspect);
void COMP_SpriteSection(ImVec2 cursorPos, bool is_inspect);
void COMP_TextureSection();		// subsection of Sprite, no cursor pos required
void COMP_AnimationSection();	// subsection of Sprite, no cursor pos required
void COMP_ButtonGUISection(ImVec2 cursorPos, bool is_inspect);
void COMP_BTNSwitchScenes();	// subsection of ButtonGUI, no cursor pos required
void COMP_BTNToggleUI();		// subsection of ButtonGUi, no cursor pos required
void COMP_DialogueSection(ImVec2 cursorPos, bool is_inspect);
void COMP_TextSection(ImVec2 cursorPos, bool is_inspect);
void COMP_WaypointsSection(ImVec2 cursorPos, bool is_inspect);
//void COMP_AudioSection(ImVec2 cursorPos);
//void COMP_ColorSection(ImVec2 cursorPos);


// Mouse state used to track user input
enum MOUSE_STATE
{
	M_IDLE = 0,
	M_CREATING,
	M_SELECT
};


// Current Render State the user is currently using
enum RENDERLAYER_STATE
{
	RL_OVERLAY = 0,
	RL_UIELEMENTS,
	RL_FOREGROUND,
	RL_GAMEPLAY,
	RL_BACKGROUND_OBJ,
	RL_BACKGROUND,
	RL_OTHERS
};

/*
	* get Mouse position inside IMGUI window. calculations performed relative to opengl viewport.
	* @returns x,y,z value of mouse
	*
*/
glm::vec3 SEGetImguiMouse();
// Serialization Functions
std::string SESaveFile(const char* filter, GLFWwindow* window);
std::string SEOpenFile(const char* filter, GLFWwindow* window);