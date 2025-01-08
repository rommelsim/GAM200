/******************************************************************************//**
 * \file			SELayerStack.cpp
 * \author 			Cheong Ming Lun  (100%)
 * \par    			m.cheong@digipen.edu
 *
 *
 * \brief			Layer Stack System
 *
 * \date   			January 2023
Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SELayerStack.h"
#include "SEFactory.h"
#include "SEGraphics.h"
#include "SELogger.h"
#include "SEAnimation.h"
#include "SEDialogue.h"		// SEDialogueManager

// Initialize an unique instance of the "SELayerStack"
std::unique_ptr<SELayerStack> SELayerStackManager = std::make_unique<SELayerStack>(); // this will solve some issues with incorrect access error

extern LEVEL current_level;

void SELayerStackInit()
{
	std::vector<std::string> LayersToInit = { "Background","Background Objects", "Gameplay", "Foreground", "UI Elements", "Overlay" };
	for (const auto& Layer : LayersToInit)
	{
		// Pushed all the layers { "Background", "Gameplay", "Foreground", "UI Elements", "Overlay" } into stack at Init phase once. 
		// All these layers will be used by the game loop update and draw calls.
		auto layer_pointer = SELayerStackManager->SEFindLayer(Layer);
		if (!layer_pointer)
		{
			SELayerStackManager->m_selected_layer = new SELayer(Layer);
			SELayerStackManager->m_layer_stack.push_back(SELayerStackManager->m_selected_layer);
		}
	}

	// Assign all gameobjects to its own layer.
	for (auto go : goArray)
	{
		std::pair<LEVEL, GameObject*> goObject = go.second;

		// We need the <Texture> component to extract out the [Render Layers] & [Render_Priority] data
		if (goObject.second)
		{
			Texture* pTexture = (Texture*)goObject.second->getComponent(CT_TEXTURE);

			// Find if the name of this layer exist in the database already. If ...
			// [Yes] - Just push in this new texture into that layer
			// [No]  - Creates a new layer object + push this new texture into that layer.
			if (pTexture)
			{
				std::string render_layer_name;
				SELayer* layer_pointer;

				switch (pTexture->m_render_layer)
				{
				case SERenderLayer::BACKGROUND:
					render_layer_name = "Background";
					break;
				case SERenderLayer::GAMEPLAY:
					render_layer_name = "Gameplay";
					break;
				case SERenderLayer::FOREGROUND:
					render_layer_name = "Foreground";
					break;
				case SERenderLayer::UI_ELEMENTS:
					render_layer_name = "UI Elements";
					break;
				case SERenderLayer::OVERLAY:
					render_layer_name = "Overlay";
					break;
				}

				layer_pointer = SELayerStackManager->SEFindLayer(render_layer_name);

				if (layer_pointer != nullptr)	// if the layer exists already. 
				{
					// Do not create a new layer. but find that layer and populate with this gameObject. 
					// std::pair<LEVEL, GameObject*> goObject 
					layer_pointer->AddLayerObject(goObject.second); // Add this object into the layer. 
				}

				else // layer does not exist
				{
					SELayerStackManager->m_selected_layer = new SELayer(render_layer_name);						// Creates a new layer object
					SELayerStackManager->m_selected_layer->AddLayerObject(goObject.second);						// Add this object into the layer
					SELayerStackManager->m_layer_stack.push_back(SELayerStackManager->m_selected_layer);		// Push back this new layer into the [Layer Stack]);				

				}
			} // end of check texture component (if it exists)
		}
	} // end of for-each loop through the goArray

	// Assign default render layer priority
	for (auto layer : SELayerStackManager->m_layer_stack)
	{
		if (layer->GetLayerName() == "Background")
		{
			layer->SetLayerPriority(1);
		}

		else if (layer->GetLayerName() == "Gameplay")
		{
			layer->SetLayerPriority(2);
		}

		else if (layer->GetLayerName() == "Foreground")
		{
			layer->SetLayerPriority(3);
		}

		else if (layer->GetLayerName() == "UI_Elements")
		{
			layer->SetLayerPriority(4);
		}

		else if (layer->GetLayerName() == "Overlay")
		{
			layer->SetLayerPriority(5);
		}

		else
		{
			layer->SetLayerPriority(6);
		}

	}

	// Sorts layers by their [SERenderlayer - enum value] 
	SELayerStackManager->SortLayerPriority();

	// Sorts the layers in the stack to accordance to [Render Priority]
	for (auto layer : SELayerStackManager->m_layer_stack)
	{
		layer->SortObjectPriority(); // sorts according the the correct priority. 
	}

}

//void SELayerStackDraw(LEVEL _level)
//{
//	// Render based on [m_priority_level - located in <CT_TEXTURE>] 
//	for (SELayer* layer : SELayerStackManager->m_layer_stack) // [Stack Level - iterates through different layers within the stack] 
//	{
//		for (GameObject* gameObject : layer->GetLayerObjects())
//		{
//			if (gameObject)
//			{
//				// Only render specfied level. 
//				if (gameObject->m_level != _level)
//					continue;
//
//				std::string name = gameObject->getName();
//				Transform* trans = (Transform*)gameObject->getComponent(CT_TRANSFORM);
//				Texture* texs = (Texture*)gameObject->getComponent(CT_TEXTURE);
//				Collision* cols = (Collision*)gameObject->getComponent(CT_COLLISION);		// For Debug Layer
//				Behaviour_Player* behaviour_player = dynamic_cast<Behaviour_Player*>(gameObject->m_behaviour.get());
//				Behaviour_Button* behaviour_button = dynamic_cast<Behaviour_Button*>(gameObject->m_behaviour.get());
//				Behaviour_NPC* behaviour_npc = dynamic_cast<Behaviour_NPC*>(gameObject->m_behaviour.get());
//				Behaviour_Evidence* behaviour_evidence = dynamic_cast<Behaviour_Evidence*>(gameObject->m_behaviour.get());
//				Behaviour_Trigger_Meter* behaviour_ui_elem = dynamic_cast<Behaviour_Trigger_Meter*>(gameObject->m_behaviour.get());
//				Font* font = (Font*)gameObject->getComponent(CT_FONT);
//				Color* color = (Color*)gameObject->getComponent(CT_COLOR);					// Added [29/1/2023] - Dialouge Text
//				Dialogue* dialogue = (Dialogue*)gameObject->getComponent(CT_DIALOGUE);		// Added [29/1/2023] - Dialogue Stuff
//
//				SEUint ID = SEGetTexture(texs->m_texture_path);
//
//#if _DEBUG
//				if (cols)
//				{
//					auto aabb = cols->boundaries;
//					cols->collisionPos.pos = trans->getTranslate();
//					SEDrawQuad(aabb.min.x, aabb.min.y, aabb.max.x, aabb.max.y, 1, 0, 0);
//				}
//#endif
//				if (font)
//				{
//					auto id = SEGetFont(font->fontPath);
//				}
//
//				if (behaviour_player)
//					SEPlayAnimation(name, behaviour_player->m_CurrentAction);
//				else if (behaviour_button)
//					SEPlayAnimation(name, behaviour_button->m_CurrentAction);
//				else if (behaviour_npc)
//					SEPlayAnimation(name, behaviour_npc->m_currentAction);
//				else if (behaviour_evidence)
//					SEPlayAnimation(name, behaviour_evidence->m_currentAction);
//				else
//					SEPlayAnimation(name, ACTION::ACTION_NULL);			// added by rommel 16/2/2023
//			}
//		}
//	}
//}


void SELayerStackDraw(LEVEL _level)
{
	// Render based on [m_priority_level - located in <CT_TEXTURE>] 
	for (SELayer* layer : SELayerStackManager->m_layer_stack) // [Stack Level - iterates through different layers within the stack] 
	{
		for (GameObject* gameObject : layer->GetLayerObjects())
		{
			if (gameObject)
			{
				// Only render specfied level. 
				if (gameObject->m_level != _level)
					continue;

				std::string name = gameObject->getName();
				Transform* trans = (Transform*)gameObject->getComponent(CT_TRANSFORM);
				Texture* texs = (Texture*)gameObject->getComponent(CT_TEXTURE);
				Collision* cols = (Collision*)gameObject->getComponent(CT_COLLISION);		// For Debug Layer
				Behaviour_Player* behaviour_player = dynamic_cast<Behaviour_Player*>(gameObject->m_behaviour.get());
				Behaviour_Button* behaviour_button = dynamic_cast<Behaviour_Button*>(gameObject->m_behaviour.get());
				Behaviour_NPC* behaviour_npc = dynamic_cast<Behaviour_NPC*>(gameObject->m_behaviour.get());
				Behaviour_Evidence* behaviour_evidence = dynamic_cast<Behaviour_Evidence*>(gameObject->m_behaviour.get());
				Behaviour_GameplayUI* behaviour_GameUI = dynamic_cast<Behaviour_GameplayUI*>(gameObject->m_behaviour.get());
				Behaviour_Arrow* behaviour_arrow = dynamic_cast<Behaviour_Arrow*>(gameObject->m_behaviour.get());
				Behaviour_Esclamation* behaviour_esclamation = dynamic_cast<Behaviour_Esclamation*>(gameObject->m_behaviour.get());
				Behaviour_Splash* behaviour_splash = dynamic_cast<Behaviour_Splash*>(gameObject->m_behaviour.get());
				Behaviour_Credits* behaviour_credits = dynamic_cast<Behaviour_Credits*>(gameObject->m_behaviour.get());
				//Behaviour_Trigger_Meter* behaviour_ui_elem = dynamic_cast<Behaviour_Trigger_Meter*>(gameObject->m_behaviour.get());
				Font* font = (Font*)gameObject->getComponent(CT_FONT);
				Color* color = (Color*)gameObject->getComponent(CT_COLOR);					// Added [29/1/2023] - Dialouge Text
				Dialogue* dialogue = (Dialogue*)gameObject->getComponent(CT_DIALOGUE);		// Added [29/1/2023] - Dialogue Stuff (depreciated)

				//SEUint ID = SEGetTexture(texs->m_texture_path);

				if (texs)
				{
					if (texs->isRendering)
					{

						if (font)
						{
							auto id = SEGetFont(font->fontPath);
						}


						if (behaviour_player)
							SEPlayAnimation(name, behaviour_player->m_CurrentAction);
						else if (behaviour_button)
							SEPlayAnimation(name, behaviour_button->m_CurrentAction);
						else if (behaviour_npc)
							SEPlayAnimation(name, behaviour_npc->m_currentAction);
						else if (behaviour_evidence)
							SEPlayAnimation(name, behaviour_evidence->m_currentAction);
						else if (behaviour_GameUI)
							SEPlayAnimation(name, behaviour_GameUI->m_currenAction);
						else if (behaviour_arrow)
							SEPlayAnimation(name, behaviour_arrow->m_currentAction);
						else if (behaviour_esclamation)
							SEPlayAnimation(name, behaviour_esclamation->m_currentAction);
						else if (behaviour_splash)
							SEPlayAnimation(name, behaviour_splash->m_currentAction);
						else if (behaviour_credits)
							SEPlayAnimation(name, behaviour_credits->m_currentAction);
						else
						{
							// by right, there should only be one texure associated with a game object that is a stil image.
							for (const auto& act : texs->AnimationTextures)
							{
								SEPlayAnimation(name, act.first);
							}
						}


						if (font && dialogue && color)
						{
							SEDrawFont(font->m_text, font->GetFont(), font->font_translate.x, font->font_translate.y, font->font_scale.x, color->getColor().r, color->getColor().g, color->getColor().b);
						}

						else if (font && dialogue)
						{
							SEDrawFont(font->m_text, font->GetFont(), font->font_translate.x, font->font_translate.y, font->font_scale.x, 255, 255, 255);
						}

						if (behaviour_GameUI) {
							if (gameObject->getName() == "obj0")
							{
								auto Obk = SEGetFont(FONT_PATH "IndieFlower.ttf");


								SEVec2 listpos = behaviour_GameUI->Parent_Pos;

								if (current_level == LEVEL::LEVEL_1)
								{
									ObjectiveList OBL_LV1(Obk, OBJLIST_PATH "Level_1_Objectives.json", listpos, 29, SEVec3{ 0, 0, 0 });
									assert(trans);
									OBL_LV1.m_OBpos = trans->getTranslate();
									if (behaviour_GameUI->m_UIstatus) {
										OBL_LV1.OL_DrawList();
									}
								}

								else
								{
									ObjectiveList OBL_LV2(Obk, OBJLIST_PATH "Level_2_Objectives.json", listpos, 29, SEVec3{ 0, 0, 0 });
									assert(trans);
									OBL_LV2.m_OBpos = trans->getTranslate();
									if (behaviour_GameUI->m_UIstatus) {
										OBL_LV2.OL_DrawList();
									}
								}
								

							}
						}

						
					}
				}

				if (cols)
				{
					/*if (dialogue)
					{
						if (dialogue->dialogue_type == SEDialogueType::OPTION)
						{*/
							auto aabb = cols->boundaries;
							cols->collisionPos.pos = trans->getTranslate();

							if (IG_debug) {
								SEDrawQuad(aabb.min.x, aabb.min.y, aabb.max.x, aabb.max.y, 1, 0, 0);
								auto FontPack = SEGetFont(FONT_PATH "Scada-Regular.ttf");
								SEDrawFont(name, FontPack, aabb.min.x, aabb.max.y, 25, 1, 0, 0);

							}
					/*	}
					}*/
				}
				//viewport box 
				if (gameObject->getName() == "Player9") {
					SEVec2 currentPlayerPos{ trans->getTranslate() };

					
					SEVec2 WinVect{ 888/*SEGetWindowWidth()*/,/*SEGetWindowHeight()*/498 };
					SEVec4 ViewportBox{ currentPlayerPos.x - WinVect.x ,currentPlayerPos.y - WinVect.y ,currentPlayerPos.x + WinVect.x,currentPlayerPos.y + WinVect.y };
					//std::cout << SEGetCameraHeight() << std::endl;
					if (IG_debug) {
						SEDrawQuad(ViewportBox.x, ViewportBox.y, ViewportBox.z, ViewportBox.w, 1, 1, 0);
						auto FontPack = SEGetFont(FONT_PATH "Scada-Regular.ttf");
						SEDrawFont("IN-GAME DEBUG IS ON", FontPack, currentPlayerPos.x - WinVect.x + 5, currentPlayerPos.y + WinVect.y - 25, 50, 1, 1, 0);


					}
				}
			}
		}
	}
}



// overloaded function.
void SELayerStackUpdate(const std::unordered_multimap<std::string, GameObject*>& container)
{
	SEDeleteEverything();

	if (SEIsKeyHeldDown(SE_KEY_LCTRL)) {
		if (SEIsKeyTriggered(SE_KEY_P)) {
			IG_debug = !IG_debug;
		}
	}

	for (auto& tmp : container)
	{
		GameObject* go = tmp.second;
		if (go)
		{
			Texture* pTexture = (Texture*)go->getComponent(CT_TEXTURE);
			std::string render_layer_name;
			SELayer* layer_pointer;
			if (pTexture)
			{
				switch (pTexture->m_render_layer)
				{
				case SERenderLayer::BACKGROUND:
					render_layer_name = "Background";
					break;
				case SERenderLayer::GAMEPLAY:
					render_layer_name = "Gameplay";
					break;
				case SERenderLayer::FOREGROUND:
					render_layer_name = "Foreground";
					break;
				case SERenderLayer::UI_ELEMENTS:
					render_layer_name = "UI Elements";
					break;
				case SERenderLayer::OVERLAY:
					render_layer_name = "Overlay";
					break;
				}
				layer_pointer = SELayerStackManager->SEFindLayer(render_layer_name);
				if (layer_pointer != nullptr)	// if the layer exists already. 
				{
					// Do not create a new layer. but find that layer and populate with this gameObject. 
					// std::pair<LEVEL, GameObject*> goObject 

					layer_pointer->AddLayerObject(go); // Add this object into the layer. 
				}
				else // layer does not exist
				{
					SELayerStackManager->m_selected_layer = new SELayer(render_layer_name);						// Creates a new layer object
					SELayerStackManager->m_selected_layer->AddLayerObject(go);						// Add this object into the layer
					SELayerStackManager->m_layer_stack.push_back(SELayerStackManager->m_selected_layer);		// Push back this new layer into the [Layer Stack]);				
				}
			}
		}
	}

	// Assign default render layer priority
	for (auto layer : SELayerStackManager->m_layer_stack)
	{
		if (layer->GetLayerName() == "Background")
		{
			layer->SetLayerPriority(1);
		}
		else if (layer->GetLayerName() == "Gameplay")
		{
			layer->SetLayerPriority(2);
		}
		else if (layer->GetLayerName() == "Foreground")
		{
			layer->SetLayerPriority(3);
		}
		else if (layer->GetLayerName() == "UI Elements")
		{
			layer->SetLayerPriority(4);
		}
		else if (layer->GetLayerName() == "Overlay")
		{
			layer->SetLayerPriority(5);
		}
		else
		{
			layer->SetLayerPriority(6);
		}
	}
	// Sorts layers by their [SERenderlayer - enum value] 
	SELayerStackManager->SortLayerPriority();

	// Sorts the layers in the stack to accordance to [Render Priority]
	for (auto layer : SELayerStackManager->m_layer_stack)
	{
		if (layer->GetLayerObjects().size() == 0) continue;
		layer->SortObjectPriority(); // sorts according the the correct priority. 
	}

	SEGameObjectForegroundLayerDepthCheck();
}

void SELayerStackUpdate(const std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>>& container)
{
	for (auto& go : container)
	{
		std::pair<LEVEL, GameObject*> goObject = go.second;

		if (goObject.second)
		{
			Texture* pTexture = (Texture*)goObject.second->getComponent(CT_TEXTURE);
			std::string render_layer_name;
			SELayer* layer_pointer;
			if (pTexture)
			{
				switch (pTexture->m_render_layer)
				{
				case SERenderLayer::BACKGROUND:
					render_layer_name = "Background";
					break;
				case SERenderLayer::GAMEPLAY:
					render_layer_name = "Gameplay";
					break;
				case SERenderLayer::FOREGROUND:
					render_layer_name = "Foreground";
					break;
				case SERenderLayer::UI_ELEMENTS:
					render_layer_name = "UI Elements";
					break;
				case SERenderLayer::OVERLAY:
					render_layer_name = "Overlay";
					break;
				}
				layer_pointer = SELayerStackManager->SEFindLayer(render_layer_name);
				if (layer_pointer != nullptr)	// if the layer exists already. 
				{
					// Do not create a new layer. but find that layer and populate with this gameObject. 
					// std::pair<LEVEL, GameObject*> goObject 

					layer_pointer->AddLayerObject(goObject.second); // Add this object into the layer. 
				}
				else // layer does not exist
				{
					SELayerStackManager->m_selected_layer = new SELayer(render_layer_name);						// Creates a new layer object
					SELayerStackManager->m_selected_layer->AddLayerObject(goObject.second);						// Add this object into the layer
					SELayerStackManager->m_layer_stack.push_back(SELayerStackManager->m_selected_layer);		// Push back this new layer into the [Layer Stack]);				
				}
			}
		}
	}

	// Assign default render layer priority
	for (auto layer : SELayerStackManager->m_layer_stack)
	{
		if (layer->GetLayerName() == "Background")
		{
			layer->SetLayerPriority(1);
		}
		else if (layer->GetLayerName() == "Gameplay")
		{
			layer->SetLayerPriority(2);
		}
		else if (layer->GetLayerName() == "Foreground")
		{
			layer->SetLayerPriority(3);
		}
		else if (layer->GetLayerName() == "UI Elements")
		{
			layer->SetLayerPriority(4);
		}
		else if (layer->GetLayerName() == "Overlay")
		{
			layer->SetLayerPriority(5);
		}
		else
		{
			layer->SetLayerPriority(6);
		}
	}

	// Sorts layers by their [SERenderlayer - enum value] 
	SELayerStackManager->SortLayerPriority();

	// Sorts the layers in the stack to accordance to [Render Priority]
	for (auto layer : SELayerStackManager->m_layer_stack)
	{
		if (layer->GetLayerObjects().size() == 0) continue;
		layer->SortObjectPriority(); // sorts according the the correct priority. 
	}
}

void SEAddLayer(std::string layer_name)
{
	SELayer* new_layer = new SELayer(layer_name);
	SELayerStackManager->m_layer_stack.push_back(new_layer);
}

void SEDeleteLayer(std::string layer_name)
{
	for (SELayer* layer : SELayerStackManager->SEGetLayerStack())
	{
		if (layer->GetLayerName() == layer_name)
		{
			delete layer; // delete this layer. 
		}
	}
}

void SEDeleteEverything()
{
	if (SELayerStackManager)
	{

		for (auto& layer : SELayerStackManager->m_layer_stack)
		{
			layer->GetLayerObjects().clear();
			delete layer;
		}

		SELayerStackManager->m_layer_stack.clear();
	}
}

void SEClearScene() {
	SEDeleteAllObject();
	SEDeleteEverything();
}

SELayer* SEGetLayer(std::string layer_name)
{
	for (SELayer* layer : SELayerStackManager->m_layer_stack)
	{
		if (layer->GetLayerName() == layer_name)
		{
			return layer;
		}
	}
	return 0;
}

// TODO : Asserts
void SELayerSetRenderPriority(std::string layer_name, std::string go_name, int render_priority)
{
	SELayer* layer_find = SELayerStackManager->SEFindLayer(layer_name);

	layer_find->SetObjectPriority(go_name, render_priority);


}

// Prints each element according to order from the [m_layer_stack] 
// - Iterates through each <Layer> 
// - Prints the name of each gameObject in each <Layer>
void SEPrintLayerElements()
{
	for (auto layer_go : SELayerStackManager->m_layer_stack)
	{
		std::cout << "-------------------------------------------------" << std::endl;
		std::cout << "Iterating through layer: " << layer_go->GetLayerName() << std::endl;
		std::cout << "-------------------------------------------------" << std::endl;

		for (auto go : layer_go->GetLayerObjects())
		{
			Texture* pTexture = (Texture*)go->getComponent(CT_TEXTURE);

			std::cout << go->getName() << ", " << pTexture->m_priority_level << std::endl;
		}
	}
}

void SEGameObjectForegroundLayerDepthCheck()
{
	GameObject* player_obj = SEFindObjectByName("Player9"); 

	if (player_obj == nullptr)
		return;

	Transform* player_trans = (Transform*)player_obj->getComponent(CT_TRANSFORM);
	Texture* player_tex = (Texture*)player_obj->getComponent(CT_TEXTURE);
	Collision* player_col = (Collision*)player_obj->getComponent(CT_COLLISION);

	// Buffer Object - to store the [y-value] to check the values.
	SEFloat y_value_player{};
	SEFloat y_value_object{};

	if (player_trans)
		y_value_player = player_trans->getTranslate().y;

	std::vector<GameObject*> foreground_go = SELayerStackManager->SEFindLayer("Foreground")->GetLayerObjects();

	for (GameObject* go : foreground_go)
	{
		Transform* go_trans = (Transform*)go->getComponent(CT_TRANSFORM);
		Collision* go_col = (Collision*)go->getComponent(CT_COLLISION);
		Texture* go_tex = (Texture*)go->getComponent(CT_TEXTURE);

		// Make sure youa aren't checking yourself.
		if (go_col)
		{
			if (go_col->isActive && go->getName() != "Player9")
			{
				if (go_trans)
				{
					y_value_object = go_trans->getTranslate().y;
				}

				if (SECheckCollision(player_col, go_col))
				{
					// When the player is on a [Higher y-value] as compared to the object it collided
					if (y_value_player >= y_value_object)
					{
						if (player_tex && go_tex)
						{
							// Player is rendering on top of this object
							if (player_tex->m_priority_level >= go_tex->m_priority_level)
							{
								//std::cout << "Player Level Swapped from " << player_tex->m_priority_level << "to";
								player_tex->m_priority_level = go_tex->m_priority_level - 1;
								go->priority_swapped = true;
								//std::cout << player_tex->m_priority_level << "successfully." << std::endl;
							}
						}
					}
				}
				
				else if (go->priority_swapped == true)
				{
					// When the player is on a [Lower y-value] as compared to the object it collided
					if (y_value_player <= y_value_object)
					{
						if (player_tex && go_tex)
						{
							// Player is rendering on below of this object
							if (player_tex->m_priority_level <= go_tex->m_priority_level)
							{
								//std::cout << "Player Level Swapped from " << player_tex->m_priority_level << "to";
								player_tex->m_priority_level = go_tex->m_priority_level + 1;
								go->priority_swapped = false;
								//std::cout << player_tex->m_priority_level << "successfully." << std::endl;
							}
						}
					}
				}

				//else // not collided anymore return to original priority level
				//{
				//	// When the player is on a [Lower y-value] as compared to the object it collided
				//	if (y_value_player <= y_value_object)
				//	{
				//		if (player_tex && go_tex)
				//		{
				//			// Player is rendering on below of this object
				//			if (player_tex->m_priority_level <= go_tex->m_priority_level)
				//			{
				//				std::cout << "Player Level Swapped from " << player_tex->m_priority_level << "to";
				//				player_tex->m_priority_level = go_tex->m_priority_level + 1;
				//				std::cout << player_tex->m_priority_level << "successfully." << std::endl;
				//			}
				//		}
				//	}
				//}
			}
		}



		//for (auto go_pair : goArray)
		//{
		//	GameObject* go = go_pair.second.second; 
		//	Transform* go_trans = (Transform*)go->getComponent(CT_TRANSFORM);
		//	Texture* go_tex = (Texture*)go->getComponent(CT_TEXTURE);
		//	Collision* go_col = (Collision*)go->getComponent(CT_COLLISION);

		//	if (go && go->getName()!= "Player9")
		//	{
		//		if (player_col && go_col)
		//		{
		//			if (go_col->isActive)
		//			{
		//				if (SECheckCollision(player_col, go_col))
		//				{
		//					// Check the [Y-position] of both objects
		//					if (go_trans)
		//					{
		//						y_value_object = go_trans->getTranslate().y;

		//						// If [Player] is on a [Higher y-value] than the [Object]
		//						if (y_value_player >= y_value_object)
		//						{
		//							// Swap [Render Priority Level] to [Lower Priority] (create hiding behind effect) - Render First
		//							if (player_tex && go_tex)
		//							{
		//								// if player [Higher Priority Level] (render later) than object.
		//								if (player_tex->m_priority_level >= go_tex->m_priority_level)
		//								{
		//									std::cout << "Player Level Swapped from " << player_tex->m_priority_level << "to";
		//									player_tex->m_priority_level = go_tex->m_priority_level - 1;
		//									std::cout << player_tex->m_priority_level << "successfully." << std::endl;

		//								}
		//							}
		//						}
		//					}
		//				}
		//			}

		//		}
		//	
		//	}

		//	
		//}
	}
}

// <------------------------------------------------------------->
//				[SELayerStack] class functions 
// <------------------------------------------------------------->

SELayer* SELayerStack::SEFindLayer(std::string layer_name)
{
	for (SELayer* layer : SELayerStackManager->SEGetLayerStack())
	{
		if (layer->GetLayerName() == layer_name)
		{
			SELayerStackManager->m_selected_layer = layer;
			return layer;
		}
	}
	SELayerStackManager->m_selected_layer = new SELayer(layer_name);
	SELayerStackManager->m_layer_stack.push_back(SELayerStackManager->m_selected_layer);

	return SELayerStackManager->m_selected_layer;
}

void SELayerStack::SortLayerPriority()
{
	std::sort(SELayerStackManager->m_layer_stack.begin(), SELayerStackManager->m_layer_stack.end(), CompareLayerPriority);
}

bool SELayerStack::CompareLayerPriority(SELayer* layer_1, SELayer* layer_2)
{
	int p1 = layer_1->GetLayerPriority();
	std::string name1 = layer_1->GetLayerName();
	int p2 = layer_2->GetLayerPriority();
	std::string name2 = layer_2->GetLayerName();

	bool final = p1 < p2;

	return final;
}

// [Accessor/Mutator]  
std::vector<SELayer*> SELayerStack::SEGetLayerStack() const
{
	return m_layer_stack;
}

SELayerStack::~SELayerStack()
{
	for (SELayer* layers : SELayerStackManager->m_layer_stack)
	{
		delete layers;
	}
}