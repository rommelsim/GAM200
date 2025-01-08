/******************************************************************************//**
 * \file			SEDialogue.cpp
 * \author 			cheong Ming Lun (100%)
 * \par    			m.cheong@digipen.edu
 *
 *
 * \brief	Dialogue System
 *
 * \date   	January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEDialogue.h"
#include "SELayerStack.h"

#define NODE_NAME std::string

// Initialize an unique instance of the "DialogueTree" 
std::unique_ptr<SEDialogueTree> SEDialogueManager = std::make_unique<SEDialogueTree>();
std::vector<std::pair<NODE_NAME, GameObject*>> node_database;

SEBool init_collider_once = false;

#pragma region [SEDialogue.cpp] core functions
void SEDialogueInit(std::string json_file_path)
{
	SELoadDialogueTree(json_file_path);  // [1] Clear current [DialogueTree] database, then [2] Deserialise & load data & populate the dialogueTree system.
}

void SEDialogueInit()
{
	SEPopulateDialogueTree();				//  Creation of [Prompts] + [Options]
}

void SEDialogueLoad(std::string dialogue_data_path)
{
	//SEDeserialiseJSON(dialogue_data_path, true); 
	//SEDeserialiseJSON(dialogue_data_path); 
}

void SEDialogueUpdate()
{
	//SELinkNodes();								// Need to link every loop (in case there's any new nodes introduced)

	if (SEFindObjectByName("NPC Muse32"))
	{
		SEDialogueTranslate("NPC Muse32");	
	}

	if (SEFindObjectByName("NPC Lala45"))
	{
		SEDialogueTranslate("NPC Lala45");	
	}

	if (SEFindObjectByName("Player9"))
	{
		SEDialogueTranslate("Player9");		
	}

	if (SEFindObjectByName("NPC Moose"))
	{
		SEDialogueTranslate("NPC Moose");	
	}


}

void SEDialogueExit()
{
	//SESerialiseJSON(goArray, "./Assets/Scenes/TestSavingDial.json");

	SEClearDialogueTree();
}

// Old Version
void SELoadDialogueTree(std::string file_path)
{
	SEClearDialogueTree(); // Might Depreciate

	//SEDeserialiseJSON(file_path);

	SEPopulateDialogueTree();				//  Creation of [Prompts] + [Options]

	SELinkNodes(); // Might Depreciate
}

void SELoadDialogue()
{

}

// 1) Find the [NPC] with the <DialogueHolder> component. 
// 2) Extract data for [Prompt] & [Options]
//void SEPopulateDialogueTree()
//{
//	for (auto go_pair : goArray)
//	{
//		GameObject* go = go_pair.second.second;
//		Dialogue* dial = (Dialogue*)go->getComponent(CT_DIALOGUE);
//		DialogueParent* dial_parent = (DialogueParent*)go->getComponent(CT_DIALOGUE_PARENT);
//
//		std::string parent_node_name_check;
//		std::string current_node_name;
//
//		SEDialogueType dialogue_type;
//		std::string dialogue_text;
//		SEInt suspicion_value;
//		std::string next_node_name;
//
//		std::string dialogue_parent; // to assign to the nodes. 
//
//		if (dial)
//		{
//			// [1] Create the (Parent Node) first.
//			// ------------------------------------
//			// [Check with] -> dialogue_database 
//			// - if it exists ... do not create
//			// - if it does not exist ... create a new node.
//
//			parent_node_name_check = dial->parent_node_name;
//			dialogue_type = dial->dialogue_type;
//			dialogue_text = dial->dialogue_text;
//			suspicion_value = dial->suspicion_value;
//			next_node_name = dial->next_node_name;
//
//
//			if (parent_node_name_check != "")
//			{
//				// What if the database is empty...
//				if (SEDialogueManager->dialogue_database.size() == 0)
//				{
//					// Create a new node.
//					SEDialogueManager->SECreateEmptyNode(parent_node_name_check); // Creates a new node with this name.
//					switch (dialogue_type)
//					{
//					case SEDialogueType::PROMPT:
//						SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->SEAddNewPrompt(go->getName(), dialogue_text);
//						SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->prompt_object->go_prompt = SEFindObjectByName(go->getName());
//						break;
//					case SEDialogueType::OPTION:
//						SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->SEAddNewOption(go->getName(), dialogue_text, suspicion_value, next_node_name);
//						SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->SEFindOption(go->getName())->go_option = SEFindObjectByName(go->getName());
//						break;
//					}
//
//				}
//
//				else
//				{
//					if (SEDialogueManager->SEDialogueNodeExist(parent_node_name_check)) // if this node exists...
//					{
//						// Access the exisiting node and populate data.
//						switch (dialogue_type)
//						{
//						case SEDialogueType::PROMPT:
//							//if(node->SEFindOption())
//							SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->SEAddNewPrompt(go->getName(), dialogue_text);
//							SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->prompt_object->go_prompt = SEFindObjectByName(go->getName());
//							break;
//						case SEDialogueType::OPTION:
//							SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->SEAddNewOption(go->getName(), dialogue_text, suspicion_value, next_node_name);
//							//SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->SEFindOption(go->getName())->go_option = SEFindObjectByName(go->getName());
//							break;
//						}
//					}
//
//					else // it does not exist...
//					{
//						// Create a new node.
//						SEDialogueManager->SECreateEmptyNode(parent_node_name_check); // Creates a new node with this name.
//						switch (dialogue_type)
//						{
//						case SEDialogueType::PROMPT:
//							SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->SEAddNewPrompt(go->getName(), dialogue_text);
//							SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->prompt_object->go_prompt = SEFindObjectByName(go->getName());
//							break;
//						case SEDialogueType::OPTION:
//							SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->SEAddNewOption(go->getName(), dialogue_text, suspicion_value, next_node_name);
//							SEDialogueManager->SEFindDialogueNode(parent_node_name_check)->SEFindOption(go->getName())->go_option = SEFindObjectByName(go->getName());
//							break;
//						}
//
//					}
//
//				}
//
//			}
//
//		}
//
//		if (dial_parent)
//		{
//			SEParentFindChildNode(go); // helps link NPC to starting dialogue.
//		}
//	}
//}


void SEPopulateDialogueTree()
{
	// Initialize the [node_database] based on their "NODE_NAME - std::string"
	for (auto go_pair : goArray)
	{
		GameObject* go = go_pair.second.second;
		Dialogue* dial = (Dialogue*)go->getComponent(CT_DIALOGUE);
		Collision* col = (Collision*)go->getComponent(CT_COLLISION);

		std::string parent_node_name; // NODE NAME 

		if (dial)
		{
			for (std::string parentnodename : dial->parent_node_names)
			{
				parent_node_name = parentnodename;
				std::pair<NODE_NAME, GameObject*> new_node = std::make_pair(parent_node_name, go);
				node_database.push_back(new_node);
			}

			if (col)
			{
				col->isActive = false;
			}
		}

		// Set inactive so there will not be overlapping buttons.
	
	}
}

void SEParentFindChildNode(GameObject* go)
{
	DialogueParent* cDialogueParent = (DialogueParent*)go->getComponent(CT_DIALOGUE_PARENT);

	for (SEDialogueNode* node : SEDialogueManager->dialogue_database)
	{
		if (node->node_name == cDialogueParent->starting_node_name)
		{
			node->parent_npc = go->getName();
		}
	}
}


// Q. How to freeze scene?
void SEDialogueFreezeScene()
{
	SESetDeltaTime(0);
}

//void SEDialogueUnfreezeScene()
//{
//	SESetDeltaTime();
//}

// Creates Sus Metre here
void SESusMeterInit()
{
	//SEDeserialiseJSON("./Assets/Scenes/Susmeter.json", true);
	//SEDeserialiseJSON("./Assets/Scenes/Susmeter.json");
}


// -----------------------------------------------------
//				Create GameObject Functions
// -----------------------------------------------------
// (1) Create [Dialogue Holder] - Textured Box
// (2) Create [Font Object]     - Text Prompt
void SECreateDialougeTextPromptObject(std::string object_name, std::string prompt_text, std::string font_path, float pos_x, float pos_y)
{
	(void)pos_x; (void)pos_y;
	std::unique_ptr<GameObject> go = std::make_unique<GameObject>(object_name);
	std::unique_ptr<Texture> texture = std::make_unique<Texture>();
	std::unique_ptr<Transform> transform = std::make_unique<Transform>();
	std::unique_ptr<Font> font = std::make_unique<Font>();
	std::unique_ptr<Dialogue> dialogue = std::make_unique<Dialogue>();
	std::unique_ptr<Color> color = std::make_unique<Color>();

	//if (go)
	//{	
	//	// [1] Transform
	//	prompt_trans->setTranslate({ pos_x, pos_y });

	//	// [2] Textures - Render Layer / Priority Level
	//	prompt_texture->m_render_layer = SERenderLayer::OVERLAY;
	//	prompt_texture->m_priority_level = 0;

	//	// [3] Font - font path/ font data.
	//	prompt_font->SetFont(font_path);	// This will load the font.

	//	// [4] Dialogue - dialogue type , prompt text.
	//	prompt_dialogue->dialogue_type = 0;
	//	prompt_dialogue->prompt_text = prompt_text; // user defined.

	//	// [5] Color 

	//	// [6] 

	//	go->addComponent(prompt_trans.release());
	//	go->addComponent(prompt_texture.release());
	//	go->addComponent(prompt_font.release());
	//	go->addComponent(prompt_dialogue.release());
	//	go->addComponent(prompt_color.release());

	//	GameObject* return_go = go.release();
	//	SECreateObject(LEVEL::LEVEL_1, return_go);
	//	
	//	// Remember to push the [SELayerStack]
	//	SELayer* layer_to_push = SELayerStackManager->SEFindLayer("Overlay");
	//	if (layer_to_push) // if this layer is not nullptr.
	//	{
	//		SELayerStackManager->SEFindLayer("Overlay")->AddLayerObject(return_go); // add this gameObject into the layer.
	//	}

	//}

}
			

	



void SELinkNodes()
{
	// (1) Update Node side data (next_node_name)
	for (SEDialogueNode* node : SEDialogueManager->dialogue_database) // for every node ... in the database...
	{
		for (SEDialogueOption* option : node->options) // for every option ... in the each node...
		{
			if (option->next_node_name != "NULL")
			{
				node->next_node_name = option->next_node_name;
				break;
			}

			else
			{
				node->next_node_name = "NULL"; // there is no next node.
			}
		}
	}

	// (2) Update Option side data 
	for (SEDialogueNode* node : SEDialogueManager->dialogue_database) // for every node ... in the database...
	{
		for (SEDialogueOption* option : node->options) // for every option ... in the each node...
		{
			if (option->next_node_name != "NULL")
			{
				if (SEDialogueManager->SEFindDialogueNode(option->next_node_name)) // only do the below if can find...
				{
					option->option_link_to_next_node = SEDialogueManager->SEFindDialogueNode(option->next_node_name);  // link the pointers.
					SEDialogueManager->SEFindDialogueNode(option->next_node_name)->prev_node_name = node->node_name;   // link previous node name. (for deletion and linkage)
				}
				else
				{
					option->option_link_to_next_node = nullptr;
				}
		

		
			}
		}
	}
}


void SEClearDialogueTree()
{
	for (SEDialogueNode* node : SEDialogueManager->dialogue_database)
	{
		for (SEDialogueOption* option : node->options)
		{
			delete option;	// delete all options first...
		}

		delete node->prompt_object;
		delete node; // ... then delete the node itself. 
	}

	SEDialogueManager->dialogue_database.erase(SEDialogueManager->dialogue_database.begin(), SEDialogueManager->dialogue_database.end()); // to make sure everything is deleted...
}

void SESaveDialoguetoJson(std::string out)
{	
	SESerialiseJSON(goArray, out);
}

void SEMakeParent(GameObject* go, std::string child_node_name)
{
	DialogueParent* cDialougeParent = (DialogueParent*)go->getComponent(CT_DIALOGUE_PARENT);

	if (SEDialogueManager->SEFindDialogueNode(child_node_name))
	{
		if (cDialougeParent)
		{
			cDialougeParent->starting_node_name = child_node_name;
		}

		else
		{
			std::unique_ptr<DialogueParent> pDialogueParent = std::make_unique<DialogueParent>("NULL"); // cannot find, so the child is empty. 
			go->addComponent(pDialogueParent.release());
		}
	}



}

void SEDialogueToggleOn(std::string npc_name, std::string node_name_toggle)
{
	// Retrieve the [NPC]'s child node. 
	GameObject* npc_go = SEFindObjectByName(npc_name);
	DialogueParent* npc_parent = (DialogueParent*)npc_go->getComponent(CT_DIALOGUE_PARENT);
	Transform* npc_transform = (Transform*)npc_go->getComponent(CT_TRANSFORM); (void)npc_transform;

	std::string parent_holding_node_name;

	std::vector<GameObject*> selected_node_elements;

	if (npc_parent)
	{
		for (int i = 0; i < npc_parent->starting_node_names.size(); ++i)
		{
			parent_holding_node_name = npc_parent->starting_node_names[i];

			// Find the [Node] that this [Parent is holding]
			for (auto node_obj : node_database)
			{
				if (node_obj.first == parent_holding_node_name) // check if the [Node  name that the NPC is holding] 
				{
					selected_node_elements.push_back(node_obj.second);
				}
			}
		}
	}

	for (GameObject* node : selected_node_elements)
	{
		Texture* node_render = (Texture*)node->getComponent(CT_TEXTURE);
		Dialogue* dial = (Dialogue*)node->getComponent(CT_DIALOGUE);
		Collision* col = (Collision*)node->getComponent(CT_COLLISION);

		if (dial)
		{
			if (node_render)
			{
				for (std::string parentnodename : dial->parent_node_names)
				{
					if (parentnodename == node_name_toggle)
					{
						node_render->isRendering = true;

						if (col)
						{
							col->isActive = true;
						}
					}
				}
			}
		}
	
	}

	// Additional : Render the name also
	if (npc_name == "NPC Lala45")
	{
		GameObject* npc_name_go = SEFindObjectByName("Lala Name");
		Texture* npc_name_texture = (Texture*)npc_name_go->getComponent(CT_TEXTURE);

		if (npc_name_go)
		{
			if (npc_name_texture)
			{
				npc_name_texture->isRendering = true;
			}
		}
	}

	else if (npc_name == "NPC Muse32")
	{
		GameObject* npc_name_go = SEFindObjectByName("Muse Name");
		Texture* npc_name_texture = (Texture*)npc_name_go->getComponent(CT_TEXTURE);
		
		// Temp fix for Node #9 issue
		Font* npc_name_font = (Font*)npc_name_go->getComponent(CT_FONT); (void)npc_name_font;
		Transform* npc_name_trans = (Transform*)npc_name_go->getComponent(CT_TRANSFORM); (void)npc_name_trans;
		Dialogue* npc_name_dial = (Dialogue*)npc_name_go->getComponent(CT_DIALOGUE);

		if (npc_name_go)
		{
			if (npc_name_texture)
			{
				for (std::string node_name : npc_name_dial->parent_node_names)
				{
					if (node_name == node_name_toggle)
						npc_name_texture->isRendering = true;
				}	
			}
		}
	}

	//else if (npc_name == "NPC Moose")
	//{
	//	GameObject* npc_name_go = SEFindObjectByName("Akai Name");
	//	Texture* npc_name_texture = (Texture*)npc_name_go->getComponent(CT_TEXTURE);

	//	if (npc_name_go)
	//	{
	//		if (npc_name_texture)
	//		{
	//			npc_name_texture->isRendering = true;
	//		}
	//	}
	//}
}

void SEDialogueToggleOff(std::string npc_name)
{
	// Retrieve the [NPC]'s child node. 
	GameObject* npc_go = SEFindObjectByName(npc_name);
	DialogueParent* npc_parent = (DialogueParent*)npc_go->getComponent(CT_DIALOGUE_PARENT);
	Transform* npc_transform = (Transform*)npc_go->getComponent(CT_TRANSFORM); (void)npc_transform;

	std::string parent_holding_node_name;

	std::vector<GameObject*> selected_node_elements;

	if (npc_parent)
	{
		for (int i = 0; i < npc_parent->starting_node_names.size(); ++i)
		{
			parent_holding_node_name = npc_parent->starting_node_names[i];

			// Find the [Node] that this [Parent is holding]
			for (auto node_obj : node_database)
			{
				if (node_obj.first == parent_holding_node_name) // check if the [Node  name that the NPC is holding] 
				{
					selected_node_elements.push_back(node_obj.second);
				}
			}
		}
	}

	for (GameObject* node : selected_node_elements)
	{
		Texture* node_render = (Texture*)node->getComponent(CT_TEXTURE);
		Dialogue* dial = (Dialogue*)node->getComponent(CT_DIALOGUE);
		Collision* col = (Collision*)node->getComponent(CT_COLLISION);
		
		// Temp fix for Node #9 issue
		Font* font = (Font*)node->getComponent(CT_FONT);
		Transform* trans = (Transform*)node->getComponent(CT_TRANSFORM);

		if (node_render && dial)
		{
			if (dial->parent_npc == npc_name) // if node matches npc's name.
			{
				node_render->isRendering = false;
			}
		}

		if (col)
		{
			col->isActive = false;
		}

		if (font)
		{
			font->font_translate = SEVec2{trans->getTranslate().x - 110, trans->getTranslate().y + 110 };
		}
	}
}

void SESpecificDialogueToggleOff(std::string npc_name, std::string dialogue_name) // for let's say when multiple dialogues are tagged to the [same npc/entity/player] -> custom choose which one to turn off (not render)
{
	// Retrieve the [NPC]'s child node. 
	GameObject* npc_go = SEFindObjectByName(npc_name);
	DialogueParent* npc_parent = (DialogueParent*)npc_go->getComponent(CT_DIALOGUE_PARENT);
	Transform* npc_transform = (Transform*)npc_go->getComponent(CT_TRANSFORM); (void)npc_transform;

	std::string parent_holding_node_name;

	std::vector<GameObject*> selected_node_elements;

	if (npc_parent)
	{
		for (int i = 0; i < npc_parent->starting_node_names.size(); ++i)
		{
			parent_holding_node_name = npc_parent->starting_node_names[i];

			// Find the [Node] that this [Parent is holding]
			for (auto node_obj : node_database)
			{
				if (node_obj.first == parent_holding_node_name) // check if the [Node  name that the NPC is holding] 
				{
					selected_node_elements.push_back(node_obj.second);
				}
			}
		}
	}

	for (GameObject* element : selected_node_elements)
	{
		Texture* element_render = (Texture*)element->getComponent(CT_TEXTURE);
		Dialogue* dial = (Dialogue*)element->getComponent(CT_DIALOGUE);

		if (element_render && dial)
		{
			for (std::string node_names : dial->parent_node_names)
			{
				if (dial->parent_npc == npc_name && node_names == dialogue_name)
				{
					element_render->isRendering = false;
				}
			}
	
		}
	}






}

// Done in Behaviour
void SEDialogueStopNPCMove(std::string npc_name)
{
}

void SEDialogueTranslate(std::string npc_go_name)
{
	SEVec2 go_trans, go_scale;
	SEVec2 camera_position = { GetCameraPositionX() , GetCameraPositionY() };
	SEBool node_element_found = false; (void)node_element_found;

	std::string parent_holding_node_name;

	// Retrieve the [NPC]'s child node. 
	GameObject* npc_go = SEFindObjectByName(npc_go_name);
	DialogueParent* npc_parent = (DialogueParent*)npc_go->getComponent(CT_DIALOGUE_PARENT);
	Transform* npc_transform = (Transform*)npc_go->getComponent(CT_TRANSFORM);

	std::vector<GameObject*> selected_node_elements;

	if (npc_parent)
	{
		for (int i = 0; i < npc_parent->starting_node_names.size(); ++i)
		{
			parent_holding_node_name = npc_parent->starting_node_names[i];
			
			// Find the [Node] that this [Parent is holding]
			for (auto node_obj : node_database)
			{
				if (node_obj.first == parent_holding_node_name) // check if the [Node  name that the NPC is holding] 
				{
					selected_node_elements.push_back(node_obj.second);
				}
			}
		}
	}

	// Retrieve [NPC]'s transform details
	go_trans = npc_transform->getTranslate();
	go_scale = npc_transform->getScale();

	int option_no = 0;

	for (GameObject* go : selected_node_elements) // traverse through all the [Node Elements] in the selected [Node]
	{
		Transform* boarder_trans = (Transform*)go->getComponent(CT_TRANSFORM);
		Font* dial_font_trans = (Font*)go->getComponent(CT_FONT);
		Dialogue* dialogue = (Dialogue*)go->getComponent(CT_DIALOGUE);
		Collision* collision = (Collision*)go->getComponent(CT_COLLISION);

		// Temp Fix for Node #9 scaling issues 
		Texture* dial_tex = (Texture*)go->getComponent(CT_TEXTURE); (void)dial_tex;
		/*if (go->getName() == "Muse Name")
		{
			dial_font_trans->font_scale = SEVec2{ 60,60 };
			dial_font_trans->font_translate = SEVec2{ dial_font_trans->font_translate.x + 40, dial_font_trans->font_translate.y - 80};
		}

		if(go->getName() == "Lala Name")
		{
			dial_font_trans->font_scale = SEVec2{ 60,60 };
			dial_font_trans->font_translate = SEVec2{ dial_font_trans->font_translate.x , dial_font_trans->font_translate.y };
		}*/


		if (dialogue->dialogue_type == SEDialogueType::NPC_NAME)
		{
			dial_font_trans->font_scale = SEVec2{ 60,60 };
			dial_font_trans->font_translate = { go_trans.x - 110, go_trans.y + 110 };

			if (go->getName() == "Muse Name - Node #9")
			{
				dial_font_trans->font_translate = { go_trans.x - 130, go_trans.y + 110 };
			}
			
			
		}

	
		if (dialogue->dialogue_type == SEDialogueType::PROMPT)
		{
			boarder_trans->setScale(SEVec2{ 150,150 });
			boarder_trans->setTranslate({ go_trans.x - 80 , go_trans.y + 100});

			SEVec2 fixed_prompt_text_pos = SEVec2{ boarder_trans->getTranslate().x , boarder_trans->getTranslate().y };

			SEVec2 fixed_prompt_text_scale = SEVec2{ 30, 25 };

			/*if (dial_font_trans->font_scale.x <= 40)
			{
				dial_font_trans->font_translate = { go_trans.x - 400 , go_trans.y + 100 };
			}

			else
			{
				dial_font_trans->font_translate = { go_trans.x - 350, go_trans.y + 100 };
			}*/

			// Set Default Scale		

			// Node #1 - [NPC Muse] + [Real Paper] 
			if (go->getName() == "Holding Picture Q1")
			{
				dial_font_trans->font_translate = {fixed_prompt_text_pos.x - 90 , fixed_prompt_text_pos.y - 10};
			}

			// Node #7 - [NPC Lala] + [Real Paper] 
			if (go->getName() == "Lala Asks Note")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x + 5, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 90, fixed_prompt_text_pos.y - 10};
			}

			// Node #2 - [NPC Lala] - Blocking Access to Lv2
			if (go->getName() == "Lala Unchecked Warning")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x - 3, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 130, fixed_prompt_text_pos.y - 10 };
			}

			// Node #11 - [NPC Lala] - [Real Photo]
			if (go->getName() == "Lala Questions Real Photo")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x - 5, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 125, fixed_prompt_text_pos.y - 10 };
			}

			// Node #11 - [NPC Muse] - [Real Photo]
			if (go->getName() == "Muse Questions Photo")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x + 5, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 120, fixed_prompt_text_pos.y - 10 };
			}

			// Node #9 - (NPC Muse -> Broom)
			if (go->getName() == "Muse Questions Broom")
			{
				boarder_trans->setScale(SEVec2{ 200, 200 });
				boarder_trans->setTranslate({ go_trans.x - 110 , go_trans.y + 100 });
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x - 3, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 215, fixed_prompt_text_pos.y - 10 };

				// Doesn't Work (Fix later)
				GameObject* muse_name = SEFindObjectByName("Muse Name");
				Transform* muse_name_trans = (Transform*)muse_name->getComponent(CT_TRANSFORM); 

				if (muse_name)
				{
					if (muse_name_trans)
					{
						muse_name_trans->setTranslate({ muse_name_trans->getTranslate().x - 100, muse_name_trans->getTranslate().y + 10}); 
					}
				}
				
			}

			// Node #14 - (NPC Lala -> Broom)
			if (go->getName() == "Lala Questions Broom")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 130, fixed_prompt_text_pos.y - 10 };
			}

			// Node #15 - (NPC Lala -> Thrash)
			if (go->getName() == "Lala Questions Thrash")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 125, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Muse Questions Thrash")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 75, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Muse Questions Fake Photo")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 85, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Lala Questions Fake Photo")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 100, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Muse Questions Mop")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 80, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Moose asks about mop")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 100, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Lala asks about mop")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 80, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Muse asks why's the mop dirty?")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 60, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Akai asks why's the mop dirty?")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 80, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Lala asks why's the mop dirty?")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 100, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Akai asks about filled bucket")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 90, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Muse asks about filled bucket")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 90, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Lala asks about filled bucket")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 100, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Akai asks about empty bucket")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 100, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Muse asks about empty bucket")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 90, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Lala asks about empty bucket")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 90, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Akai asks about bloody knife")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 50, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Muse asks about bloody knife")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 80, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Lala asks about bloody knife")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 80, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Akai asks about clean knife")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 100, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Muse asks about clean knife")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 80, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Lala asks about clean knife")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 90, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Akai asks about candy corn")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 90, fixed_prompt_text_pos.y - 10 };
			}

			if (go->getName() == "Muse asks about candy corn")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 80, fixed_prompt_text_pos.y - 10  };
			}

			if (go->getName() == "Lala asks about candy corn")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 50, fixed_prompt_text_pos.y - 10  };
			}

			if (go->getName() == "Akai asks about candy horn")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 40, fixed_prompt_text_pos.y - 10  };
			}

			if (go->getName() == "Muse asks about candy horn")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 90, fixed_prompt_text_pos.y - 10  };
			}

			if (go->getName() == "Lala asks about candy horn")
			{
				dial_font_trans->font_scale = SEVec2{ fixed_prompt_text_scale.x, fixed_prompt_text_scale.y };
				dial_font_trans->font_translate = { fixed_prompt_text_pos.x - 50, fixed_prompt_text_pos.y - 10 };
			}
		}

		if (dialogue->dialogue_type == SEDialogueType::OPTION)
		{
			SEVec2 fixed_camera_button_1_pos = SEVec2{ GetCameraPositionX() - 200 , GetCameraPositionY() - 150 };
			SEVec2 fixed_camera_button_2_pos = SEVec2{ GetCameraPositionX() + 200 , GetCameraPositionY() - 150 };
			SEVec2 fixed_text_button_1_pos = SEVec2{ GetCameraPositionX() - 200 , GetCameraPositionY() - 150 };
			SEVec2 fixed_text_button_2_pos = SEVec2{ GetCameraPositionX() + 200 , GetCameraPositionY() - 150 };

			float boarder_scale_x = 180; 
			float boarder_scale_y = 150;

			SEVec2 fixed_button_collision_1_pos = SEVec2{ GetCameraPositionX() - 200 , GetCameraPositionY() - 150};
			SEVec2 fixed_button_collision_2_pos = SEVec2{ GetCameraPositionX() + 200 , GetCameraPositionY() - 150 };
			SEVec2 fixed_button_collision_col = SEVec2{ 180 , 50 };
			
			SEVec2 template_button_text_scale = SEVec2{35, 30};
			float fixed_button_1_text_posx = fixed_camera_button_1_pos.x - 50; 
			float fixed_button_2_text_posx = fixed_camera_button_2_pos.x - 50;

			// Share same y-level
			float fixed_button_text_posy = fixed_camera_button_1_pos.y - 10; 

			boarder_trans->setScale(SEVec2{ boarder_scale_x, boarder_scale_y });
			
			if (option_no % 2 == 1)
			{
				boarder_trans->setTranslate(fixed_camera_button_1_pos);

				dial_font_trans->font_translate = fixed_text_button_1_pos;

				collision->SetCollision(fixed_button_collision_1_pos, fixed_button_collision_col);

				// Node #1 - (NPC Muse -> Real Paper)
				if (go->getName() == "Officer Moose Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 100 , fixed_button_text_posy };
				}

				// Node #7 - (NPC Lala -> Real Paper)
				if (go->getName() == "Personal Notes Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 70 , fixed_button_text_posy };
				}

				// Node #11 - (NPC Muse -> Real Photo)
				if (go->getName() == "Bring back photo Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				// Node #13 - (NPC Lala -> Real Photo)
				if (go->getName() == "Officer Gave Me Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				// Node #9 - (NPC Lala -> Broom)
				if (go->getName() == "Bringing in Test Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				// Node #14 - (NPC Lala -> Broom)
				if (go->getName() == "Broom Blocking Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 60 , fixed_button_text_posy };
				}

				if (go->getName() == "Thrash Check Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				if (go->getName() == "Forensics Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 40 , fixed_button_text_posy };
				}

				if (go->getName() == "Pass to Lala Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 70 , fixed_button_text_posy };
				}

				if (go->getName() == "Photo Pass to Muse Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 80 , fixed_button_text_posy };
				}

				if (go->getName() == "I'm placing this aside.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 20 , fixed_button_text_posy };
				}

				if (go->getName() == "Just going to bring it to the store room.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 90 , fixed_button_text_posy };
				}

				if (go->getName() == "I want to use it to clean something up.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 90 , fixed_button_text_posy };
				}

				if (go->getName() == "I don't know.. I found it like this.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 70 , fixed_button_text_posy };
				}

				if (go->getName() == "No, I used this to mop up paint.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 70 , fixed_button_text_posy };
				}

				if (go->getName() == "Yeah but I decided to mop up some paint.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 100 , fixed_button_text_posy };
				}

				if (go->getName() == "I want to paint my walls at home with this.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 100 , fixed_button_text_posy };
				}

				if (go->getName() == "It's so pretty, don't you think?")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				if (go->getName() == "I want to check if it's blood.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				if (go->getName() == "That was a different bucket.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				if (go->getName() == "Yeah but I threw it all out.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 40 , fixed_button_text_posy };
				}

				if (go->getName() == "I want to get this bucket checked just in case.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 110 , fixed_button_text_posy };
				}

				if (go->getName() == "Nah, looks like nothing to me.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				if (go->getName() == "Gonna get it all cleaned up!")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 40 , fixed_button_text_posy };
				}

				if (go->getName() == "Just bringing it somewhere else..")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 60 , fixed_button_text_posy };
				}

				if (go->getName() == "Yep, I cleaned it.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 10 , fixed_button_text_posy };
				}

				if (go->getName() == "No, it was clean when I found it.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 60 , fixed_button_text_posy };
				}

				if (go->getName() == "No, that was another knife.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 40 , fixed_button_text_posy };
				}

				if (go->getName() == "I'm going to snack on this.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 40 , fixed_button_text_posy };
				}

				if (go->getName() == "Yeah you can take some from the pile!")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 90 , fixed_button_text_posy };
				}

				if (go->getName() == "Candy I found on the floor.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				if (go->getName() == "Don't worry, it's candy corn.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				if (go->getName() == "There's nothing weird about it!!")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 50 , fixed_button_text_posy };
				}

				if (go->getName() == "Yeah. I'm going to get it checked.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_1_text_posx - 60 , fixed_button_text_posy };
				}
			}

			else if (option_no % 2 == 0)
			{
				// [Boarder - Position] relative to camera positons
				boarder_trans->setTranslate(fixed_camera_button_2_pos);

				// [Option Font - Position] relative to [Boarder - Position]
				dial_font_trans->font_translate = fixed_text_button_2_pos;

				collision->SetCollision(fixed_button_collision_2_pos, fixed_button_collision_col);

				// [Node #1] Muse - Real Paper
				if (go->getName() == "Secure Bag Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 80, fixed_button_text_posy };
				}

				// [Node #7] Lala - Real Paper
				if (go->getName() == "Found a Paper Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 50 , fixed_button_text_posy };
				}

				// [Nod #11] Muse - Real Photo
				if (go->getName() == "Got a clue Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 50 , fixed_button_text_posy };
				}

				// Node #13 - (NPC Lala -> Real Photo)
				if (go->getName() == "Found it on table Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 50 , fixed_button_text_posy };
				}

				// Node #9 - (NPC Muse -> Broom Pickup)
				if (go->getName() == "Just Checking Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 20 , fixed_button_text_posy };
				}

				// Node #14 - (NPC Lala -> Broom)
				if (go->getName() == "Broom Suspicious Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 60 , fixed_button_text_posy };
				}
				
				if (go->getName() == "Pass to Muse Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 60 , fixed_button_text_posy };
				}


				if (go->getName() == "Shifting to one side Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 60 , fixed_button_text_posy };
				}

				
				if (go->getName() == "Send Evidence Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 60 , fixed_button_text_posy };
				}


				if (go->getName() == "Checking Fake Photo Excuse Option")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x, template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 30 , fixed_button_text_posy };
				}
			
				if (go->getName() == "It might be potential evidence.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 50, fixed_button_text_posy };
				}

				if (go->getName() == "I'm going to report this item.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 50, fixed_button_text_posy };
				}

				if (go->getName() == "I'm going to get this checked.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 50, fixed_button_text_posy };
				}
			
				if (go->getName() == "It's dirty because I was using it.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 60, fixed_button_text_posy };
				}
	
				if (go->getName() == "Looks like the suspect tried to hide the blood!")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 110, fixed_button_text_posy };
				}

				if (go->getName() == "No that was a different mop.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 60, fixed_button_text_posy };
				}

				if (go->getName() == "Officer Muse asked me to bring it to him.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 90, fixed_button_text_posy };
				}

				if (go->getName() == "It looks like blood. I want to get it checked.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 95, fixed_button_text_posy };
				}

				if (go->getName() == "I just brought it here to show you!")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 70, fixed_button_text_posy };
				}

				if (go->getName() == "I already used up all the paint.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 60, fixed_button_text_posy };
				}

				if (go->getName() == "No, that was another bucket.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 50, fixed_button_text_posy };
				}

				if (go->getName() == "Just bringing this to the store room.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 70, fixed_button_text_posy };
				}

				if (go->getName() == "Yeah. I'll go get it checked.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 40, fixed_button_text_posy };
				}

				if (go->getName() == "I'm bring it to forensics.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 30, fixed_button_text_posy };
				}

				if (go->getName() == "I'm going to turn it in as evidence.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 60, fixed_button_text_posy };
				}

				if (go->getName() == "That was a different knife.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 50, fixed_button_text_posy };
				}

				if (go->getName() == "Yeah, it was too dirty.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 30, fixed_button_text_posy };
				}

				if (go->getName() == "Yeah, but I decided to clean it.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 50, fixed_button_text_posy };
				}

				if (go->getName() == "Back to the candy pile!")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 30, fixed_button_text_posy };
				}

				if (go->getName() == "Yeah. I brought it from home for lunch.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 80, fixed_button_text_posy };
				}

				if (go->getName() == "Just a piece of candy I brought.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 60, fixed_button_text_posy };
				}

				if (go->getName() == "Definitely not a horn...")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 30, fixed_button_text_posy };
				}

				if (go->getName() == "It might be old and expired.")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 40, fixed_button_text_posy };
				}

				if (go->getName() == "No... It's a hundred percent candy!")
				{
					dial_font_trans->font_scale = SEVec2{ template_button_text_scale.x , template_button_text_scale.y };
					dial_font_trans->font_translate = { fixed_button_2_text_posx - 70, fixed_button_text_posy };
				}
				//// Hard Coded Adjustments (To centralize)
			
			}

			option_no++;
		}

		if (dialogue->dialogue_type == SEDialogueType::THOUGHT_BUBBLE)
		{
			boarder_trans->setTranslate({ go_trans.x - 50, go_trans.y + 70 });
			boarder_trans->setScale(100, 100);

			if (dial_font_trans)
			{
				dial_font_trans->font_translate = { go_trans.x - 200, go_trans.y + 150 };
			}
		}


		if (dialogue->dialogue_type == SEDialogueType::MISCELLEANEOUS) // Esclamation Mark
		{
			boarder_trans->setTranslate({ go_trans.x, go_trans.y + 100 });	
		}

		// Hard-coded adjustments


	


		

	
	}
}

void SEDialogueInitButtonCollider()
{
	for (auto go_pair : goArray)
	{
		GameObject* go = go_pair.second.second;
		Dialogue* dial = (Dialogue*)go->getComponent(CT_DIALOGUE);	     // For Dialogue Type
		Collision* col = (Collision*)go->getComponent(CT_COLLISION);	 // For Collision Box
		Transform* trans = (Transform*)go->getComponent(CT_TRANSFORM);   // For Boarder Translate

		if (dial)
		{
			if (dial->dialogue_type == SEDialogueType::OPTION)
			{
				col->collisionPos.pos = { trans->getTranslate().x , trans->getTranslate().y };
				
				SEVec2 new_collision_min = { col->collisionPos.pos.x - 0.5 * col->boundaries.scale.x, col->collisionPos.pos.y - 0.5 * col->boundaries.scale.y };
				SEVec2 new_collision_max = { col->collisionPos.pos.x + 0.5 * col->boundaries.scale.x , col->collisionPos.pos.y + 0.5 * col->boundaries.scale.y };

				col->SetCollision(new_collision_min, new_collision_max);
			}
		}
	}
}

#pragma endregion


#pragma region [SEDialougeTree]
//SEDialogueTree::SEDialogueTree()
//{
//
//}

SEDialogueTree::~SEDialogueTree()
{

}

// Logical Functionalities
// -----------------------------------------------
SEBool SEDialogueTree::SEDialogueNodeExist(std::string node_name) const
{
	for (SEDialogueNode* node : SEDialogueManager->dialogue_database)
	{
		if (node->SEGetDialogueName() == node_name)
		{
			return true;
		}
	}

	return false;
}

SEDialogueNode* SEDialogueTree::SEFindDialogueNode(std::string node_name)
{
	for (SEDialogueNode* node : SEDialogueManager->dialogue_database)
	{
		if (node->SEGetDialogueName() == node_name)
		{
			return node;
		}
	}
	return nullptr;
}

// (1) Adding a new [Node]
void SEDialogueTree::SECreateEmptyNode(std::string node_name)
{
	SEDialogueNode* new_node = new SEDialogueNode(node_name);
	SEDialogueManager->dialogue_database.push_back(new_node);
}

SEDialogueNode* SEDialogueTree::SECreateEmptyNodeReturn(std::string node_name)
{
	SEDialogueNode* new_node = new SEDialogueNode(node_name);
	new_node->prompt_object = new SEDialoguePrompt("Empty Prompt", "");
	SEDialogueManager->dialogue_database.push_back(new_node);
	return new_node;
}

// (2) Deleting a new [Node] (1 node at a time)
void SEDialogueTree::SEDeleteNode(std::string node_name)
{	
	// Things to delete
	SEDialogueNode* node_to_delete; 
	SEDialogueOption* option_to_delete; (void)option_to_delete;
	SEDialogueNode* prev_node;
	SEDialogueNode* next_node;

	int counter = 0;

	// (1) Find the node we want to delete.
	for (SEDialogueNode* node : dialogue_database)
	{
		if (node->node_name == node_name)
		{
			node_to_delete = node;

			// (2) Disable link with this node. (node_to_delete)
			prev_node = SEDialogueManager->SEFindDialogueNode(node_to_delete->prev_node_name);
			next_node = SEDialogueManager->SEFindDialogueNode(node_to_delete->next_node_name);

			if (prev_node != nullptr) // check guard (is there a previous node...)
			{
				for (SEDialogueOption* option : prev_node->options) // unlink all options linkage to dialogue
				{
					option->option_link_to_next_node = nullptr;		// set all options to point to nothing.
					option->next_node_name = "NULL";
				}
			}
			
			// Deletion is here (current node) -> options
			for (SEDialogueOption* option : node->options) // don't need to check -> current node exists.
			{
				option->option_link_to_next_node = nullptr;
				option->next_node_name = "NULL";

				SEDeleteObjectByName<LEVEL::LEVEL_1>(option->option_name); // delete the actual gameObject too. (must be before)
				delete option;
			}

			node->options.clear();								// clear the vector.

			// Deletion is here (current node) -> prompt
			if (node->prompt_object != nullptr)
			{
				SEDeleteObjectByName<LEVEL::LEVEL_1>(node->prompt_object->prompt_name); // delete the actual gameObject too. (must be before)
				delete node->prompt_object;							// delete this prompt object.
			}

			
			// Update Data Record
			if (prev_node != nullptr) 
			{
				prev_node->next_node_name = "NULL";
			}
			
			if (next_node != nullptr)
			{
				next_node->prev_node_name = "NULL";	
			}

			delete node;
			// I will need to delete the [Node]'s [GameObject*] in [GoArray]
			SEDeleteObjectByName<LEVEL::LEVEL_1>(node_name);

			break;
		}
		++counter;
	}

	dialogue_database.erase(dialogue_database.begin() + counter);


}

// (3) Retrieve data of a relevant Node.
#pragma endregion

#pragma region [SEDialogueNode]
SEDialogueNode::SEDialogueNode() 
{
	prompt_object = new SEDialoguePrompt("Empty Prompt", "");
}

SEDialogueNode::SEDialogueNode(std::string node_name)
{
	this->node_name = node_name;
}

SEDialogueNode::SEDialogueNode(std::string node_name, SEDialoguePrompt* prompt_object, SEDialogueOption* option_1, SEDialogueOption* option_2)
{
	this->node_name = node_name;
	this->prompt_object = prompt_object;
	options.push_back(option_1);
	options.push_back(option_2);
	//this->next_node = next_node;
}


// (1) Adding a [Prompt] / [Option] to a specified [Node]
// - [Adding to the back of the database]
void SEDialogueNode::SEAddNewPromptObject(std::string prompt_name, std::string prompt_text, SEVec2 trans_scale, SEVec2 trans_translate,
					 SEVec2 font_scale, SEVec2 font_translate,
					 std::string boarder_texture_path,
					 std::string font_path)
{
	// Update Database Side.
	SEDialoguePrompt* new_prompt = new SEDialoguePrompt(prompt_name, prompt_text);
	prompt_object = new_prompt;

	// Need to create a legit [GameObject]
	std::unique_ptr<GameObject> go = std::make_unique<GameObject>(prompt_name);
	std::unique_ptr<Transform> transform = std::make_unique<Transform>();
	std::unique_ptr<Texture> texture = std::make_unique<Texture>();
	std::unique_ptr<Font> font = std::make_unique<Font>(font_path, prompt_text);
	std::unique_ptr<Dialogue> dial = std::make_unique<Dialogue>();

	transform->setScale(trans_scale);
	transform->setTranslate(trans_translate);

	texture->m_texture_path = boarder_texture_path;

	font->font_scale = font_scale;
	font->font_translate = font_translate;

	dial->dialogue_type = SEDialogueType::PROMPT;
	dial->dialogue_text = prompt_text;
	dial->parent_node_name = this->node_name;

	go->addComponent(transform.release());
	go->addComponent(texture.release());
	go->addComponent(dial.release());
	go->addComponent(font.release());

	SECreateObject(LEVEL::LEVEL_1, go.release());

}
void SEDialogueNode::SEAddNewPrompt(std::string prompt_name, std::string prompt_text)
{
	prompt_object = new SEDialoguePrompt(prompt_name, prompt_text);
	
}

void SEDialogueNode::SEAddNewPrompt(SEDialoguePrompt* _prompt_object)
{
	this->prompt_object = _prompt_object;
}

void SEDialogueNode::SEAddNewOptionObject(std::string option_name, std::string option_text, int suspicion_value,
									SEVec2 trans_scale, SEVec2 trans_translate,
									SEVec2 font_scale, SEVec2 font_translate, std::string _next_node_name, std::string boarder_texture_path, std::string font_path)
{
	// Update Database Side.
	SEDialogueOption* new_option = new SEDialogueOption(option_name, option_text, suspicion_value, _next_node_name);
	options.push_back(new_option);

	// Need to create a legit [GameObject]
	std::unique_ptr<GameObject> go = std::make_unique<GameObject>(option_name);
	std::unique_ptr<Transform> transform = std::make_unique<Transform>();
	std::unique_ptr<Texture> texture = std::make_unique<Texture>();
	std::unique_ptr<Font> font = std::make_unique<Font>(font_path, option_text);
	std::unique_ptr<Dialogue> dial = std::make_unique<Dialogue>();

	transform->setScale(trans_scale);
	transform->setTranslate(trans_translate);

	texture->m_texture_path = boarder_texture_path;

	font->font_scale = font_scale;
	font->font_translate = font_translate;

	dial->dialogue_type = SEDialogueType::OPTION;
	dial->dialogue_text = option_text;
	dial->suspicion_value = suspicion_value;
	dial->next_node_name = next_node_name;
	dial->parent_node_name = this->node_name;


	go->addComponent(transform.release());
	go->addComponent(texture.release());
	go->addComponent(dial.release());
	go->addComponent(font.release());

	SECreateObject(LEVEL::LEVEL_1, go.release());
}


void SEDialogueNode::SEAddNewOption(std::string option_name, std::string option_text, int suspicion_value, std::string _next_node_name)
{
	// Update Database Side.
	SEDialogueOption* new_option = new SEDialogueOption(option_text, option_name, suspicion_value, _next_node_name);
	options.push_back(new_option);
}

void SEDialogueNode::SEAddNewOption(SEDialogueOption* option_object)
{
	options.push_back(option_object);

	//this->next_node_name = option_object->next_node_name;
}

// (2) Updating a [Prompt] / [Option] of an existing [Node]
void SEDialogueNode::SEUpdatePrompt(std::string prompt_name, std::string prompt_text)
{
	prompt_object->prompt_text = prompt_text;
	
	GameObject* go = SEFindObjectByName(prompt_name);
	Dialogue* dial = (Dialogue*)go->getComponent(CT_DIALOGUE);
	
	dial->dialogue_text = prompt_text;
}

void SEDialogueNode::SEUpdateOption(std::string option_name, std::string option_text, int suspicion_value, std::string _next_node_name)
{
	for (SEDialogueOption* option : options)
	{
		if (option->option_name == option_name)
		{
			option->option_text = option_text;
			option->suspicion_value = suspicion_value;
			option->next_node_name = _next_node_name;
		}
	}

	GameObject* go = SEFindObjectByName(option_name);
	Dialogue* dial = (Dialogue*)go->getComponent(CT_DIALOGUE);

	dial->dialogue_text = option_text;
	dial->suspicion_value = suspicion_value;
	dial->next_node_name = next_node_name;

}

//void SEDialogueNode::SEUpdateNextNodePointer(std::string next_node_name)
//{
//	if (SEDialogueManager->SEFindDialogueNode(next_node_name))
//	{
//		next_node = SEDialogueManager->SEFindDialogueNode(next_node_name);
//	}
//}

// (3) Deleting a [Prompt] / [Option] of an existing [Node] 
void SEDialogueNode::SEDeletePrompt()
{

	SEDeleteObjectByName<LEVEL::LEVEL_1>(prompt_object->prompt_name);

	delete prompt_object;
	prompt_object = nullptr;

}

void SEDialogueNode::SEDeleteOption(std::string option_name)
{
	if (options.size() == 0) // nothing to delete.
		return;

	int i = 0;
	for (SEDialogueOption* option : options)
	{
		if (option->option_name == option_name)
		{
			SEDeleteObjectByName<LEVEL::LEVEL_1>(option->option_name);
			delete option;
			break;
		}
		++i;
	}

	options.erase(options.begin() + i);
}

// (4) Retrieve details about a certain [Prompt] / [Option]
std::string SEDialogueNode::SEGetDialogueName() const
{
	return node_name;
}

std::vector<SEDialogueOption*> SEDialogueNode::SEGetDialogueOptions()
{
	return options;
}

SEDialogueOption* SEDialogueNode::SEFindOption(std::string option_name) const
{
	for (SEDialogueOption* option : options)
	{
		if (option->option_name == option_name)
		{
			return option;
		}
	}

	return nullptr;
}



#pragma endregion

#pragma region [SEDialoguePrompt]
SEDialoguePrompt::SEDialoguePrompt(std::string prompt_name, std::string prompt_text)
{
	this->prompt_name = prompt_name;
	this->prompt_text = prompt_text;
}


#pragma endregion

#pragma region [SEDialogueOption]
SEDialogueOption::SEDialogueOption(std::string option_name, std::string option_text, int suspicion_value, std::string next_node_name)
{
	this->option_text = option_text;
	this->option_name = option_name;
	this->suspicion_value = suspicion_value;
	this->next_node_name = next_node_name;
}
#pragma endregion