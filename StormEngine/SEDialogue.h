/******************************************************************************//**
 * \file		SEDialouge.h
 * \author 		Cheong Ming Lun (100%)
 * \co-author
 * \par    		m.cheong@digipen.edu
 *
 * \brief		Dialoge System
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEGraphics.h"
#include "SESerialiser.h"

// Forward Declaration
class SEDialogueTree;
class SEDialogueNode; 
class SEDialoguePrompt;
class SEDialogueOption;

void SEDialogueInit(std::string json_file_path);
void SEDialogueInit();
void SEDialogueUpdate();
void SEDialogueExit();

// Functionality
void SELoadDialogueTree(std::string file_path);
void SELoadDialogue();
void SEPopulateDialogueTree();								// Populates the [Dialogue Tree] w data from [goArray]
void SEParentFindChildNode(GameObject* go);
void SEClearDialogueTree();
void SELinkNodes();
void SESaveDialoguetoJson(std::vector<GameObject*>& container, std::string out);

// Parent-Child Functions
void SEMakeParent(GameObject* go, std::string child_node_name);
void SEParentTriggerDialogue(GameObject* go);

// Behaviour Stuff
void SEDialogueToggleOn(std::string npc_name, std::string node_name_toggle);
void SEDialogueToggleOff(std::string npc_name);
void SESpecificDialogueToggleOff(std::string npc_name, std::string node_name);
void SEDialogueStopNPCMove(std::string npc_name);
void SEDialogueTranslate(std::string go_name); 
void SEDialogueTranslateMouth(std::string npc_name);


// Init Stuff
void SEDialogueInitButtonCollider();

class SEDialogueTree
{
public:
	std::vector<SEDialogueNode*> dialogue_database;  // Contains a series of nodes that are connect to one another.

public: 
	// Custom Iterators
	std::vector<SEDialogueNode*>::iterator begin() { return dialogue_database.begin(); };
	std::vector<SEDialogueNode*>::iterator end() { return dialogue_database.end(); };

public:
	// Constructors/Destructors
	SEDialogueTree() = default;									// [Default Constructor] - creates an empty tree.
	~SEDialogueTree();											// [Destructor]			 - destroys the tree
public:
	// Logical Functionalities
	// -----------------------------------------------
	SEBool SEDialogueNodeExist(std::string node_name) const;

	// CRUD Functionalities
	// ------------------------------
	// (1) Adding a new [Node]
	void SECreateEmptyNode(std::string node_name);
	SEDialogueNode* SECreateEmptyNodeReturn(std::string node_name);

	// (2) Deleting a new [Node] 
	void SEDeleteNode(std::string node_name);

	// (3) Retrieve data of a relevant Node.
	SEDialogueNode* SEFindDialogueNode(std::string node_name);

};

class SEDialogueNode
{
public:
	// Think of the elements of a "Dialogue" system.
	SEDialoguePrompt* prompt_object;				    // [1] Can only have (1 prompt text - question)
	std::vector<SEDialogueOption*> options;				// [2] Can have (1 or many options)
	std::string node_name;								// [3] Name Identifier
	std::string parent_npc;								// [4] Which[GameObject] this node belongs to.

	std::string next_node_name = "NULL";
	std::string prev_node_name = "NULL";
	//SEDialogueNode* next_node;
public: 
	SEDialogueNode();
	SEDialogueNode(std::string node_name);		// Empty Node
	SEDialogueNode(std::string node_name, SEDialoguePrompt* prompt_object, SEDialogueOption* option_1, SEDialogueOption* option_2);   // [1] Paraemeterized #1 - Feed in actual objects (1 prompt, 2 options)
	~SEDialogueNode() = default;
public:
	// CRUD Functionalities
	// ------------------------------
	
	// (1) Adding a [Prompt] / [Option] to a specified [Node]

	// [Adding to the back of the database] + Create GameObject
	void SEAddNewPromptObject(std::string prompt_name, std::string prompt_text, SEVec2 trans_scale, SEVec2 trans_translate,
							 SEVec2 font_scale, SEVec2 font_translate,
							 std::string boarder_texture_path = "./Assets/Textures/UI_Dialogue_TextBox.png",
							 std::string font_path = "./Assets/Fonts/GoudyBookletter1911.ttf");
	void SEAddNewPrompt(std::string prompt_name, std::string prompt_text);   // [1] Adding new Prompt Object (defining variables)
	void SEAddNewPrompt(SEDialoguePrompt* prompt_object);					 // [2] Adding new Prompt Object (funneling object)


	// Creates a [GoObject] (use this to create new object)
	// - No Linkage (will link during update loop)
	void SEAddNewOptionObject(std::string option_name, std::string option_text, int suspicion_value = 0, 
							  SEVec2 trans_scale = { 200,200 }, SEVec2 trans_translate = SEVec2{ 200,200 },
							  SEVec2 font_scale = { 200,200 }, SEVec2 font_translate = SEVec2{ 200,200 }, std::string next_node_name = "NULL",
							  std::string boarder_texture_path = "./Assets/Textures/UI_Dialogue_TextBox.png",
							  std::string font_path = "./Assets/Fonts/GoudyBookletter1911.ttf");
	
	// Creates a [Option] object inside the database.
	void SEAddNewOption(std::string option_name, std::string option_text, int suspicion_value, std::string next_node_name = "NULL");
	void SEAddNewOption(SEDialogueOption* option_object);

	// (2) Updating a [Prompt] / [Option] of an existing [Node]
	void SEUpdatePrompt(std::string prompt_name, std::string prompt_text);
	void SEUpdateOption(std::string option_name,std::string option_text, int suspicion_value, std::string next_node_name = "NULL");
	
	//void SEUpdateNextNodePointer(std::string next_node_name);

	// (3) Deleting a [Prompt] / [Option] of an existing [Node] (from this database)
	void SEDeletePrompt();							
	void SEDeleteOption(std::string option_name);

	// (4) Retrieve details about a certain [Prompt] / [Option]
	std::string SEGetDialogueName() const;									 // [1] Retrieve the name of this dialogue node.
	std::vector<SEDialogueOption*> SEGetDialogueOptions();					 // [2] Direct access to [Options] databse under this node.

	// (5) Logic Function
	SEDialogueOption* SEFindOption(std::string option_name) const;

public:
	// Custom Iterators
	std::vector<SEDialogueOption*>::iterator begin() { return options.begin(); };
	std::vector<SEDialogueOption*>::iterator end()   { return options.end(); };





	
};

class SEDialoguePrompt
{
public:
	// A [Prompt] consists off...
	std::string prompt_text = "";					// [1] Text to be displayed as "prompt"
	std::string prompt_name;						// [2] Prompt Identifier (name / ID)
	GameObject* go_prompt;							// [3] Game Object link
public:
	SEDialoguePrompt() = default;
	SEDialoguePrompt(std::string prompt_name , std::string prompt_text);
	~SEDialoguePrompt() = default;
};

class SEDialogueOption
{
public:
	// An [Option] consists off...
	std::string option_text;
	std::string option_name;
	SEInt suspicion_value;
	std::string next_node_name;
	GameObject* go_option;
	SEDialogueNode* option_link_to_next_node;
public:
	SEDialogueOption() = default;
	SEDialogueOption(std::string option_name, std::string option_text,  int suspicion_value, std::string next_node_name = "NULL");


};

extern std::unique_ptr<SEDialogueTree> SEDialogueManager;