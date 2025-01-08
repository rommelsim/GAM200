/******************************************************************************//**
 * \file			main.cpp
 * \author 			Ming Lun(100%)
 * \brief	This file contains the declaration for layering stack.
 * \date   	January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SELayers.h"

//class SELayerStack;




// Interface - main loop
void SELayerStackInit();																			// Populates the layer stack and gets the layer sorted out. (in accordance to priority)
//void SELayerStackDraw();																			// Draws in accordance from the [SELayerStack] database.
void SELayerStackDraw(LEVEL _level);																			// Draws in accordance from the [SELayerStack] database.
void SELayerStackUpdate(const std::unordered_multimap<std::string, std::pair<LEVEL, GameObject*>>& container);	// sort stack in game loop
void SELayerStackUpdate(const std::unordered_multimap<std::string, GameObject*>& container);					// sort stack in game loop

// Layer - CRUD (more for editor)
void SEAddLayer(std::string layer_name);
void SEDeleteLayer(std::string layer_name);
void SEDeleteEverything();
void SEClearScene();
SELayer* SEGetLayer(std::string layer_name);

void SELayerSetRenderPriority(std::string layer_name, std::string go_name, int render_priority);	// Finds [gameObject] on a particular [layer] & set it's [render priority level]
void SEPrintLayerElements();																		// [Debug] - printing layer data.

// Layer Priority swap based on [Y-axis] 
// - Compares [Y-axis] value of [Player] & [all other enviornmental objects]
// - Higher the Y-axis (should render first) -> set render priority level to lesser than the game object in front 
void SEGameObjectForegroundLayerDepthCheck();

// Data Container 
class SELayerStack
{
public:
	SELayerStack()  = default;		// There's no need for allocation?
	~SELayerStack();			    // Deallocate the layers from m_layer_stack

	// [Helper Functions]
	SELayer* SEFindLayer(std::string layer_name);
	void SortLayerPriority();													// Sorts Layer Priority in accordance to each layer's (m_layer_priority)
	static bool CompareLayerPriority(SELayer* layer_1, SELayer* layer_2);		// [Helper Condition] - std::sort

	// [Accessor/Mutator]
	std::vector<SELayer*> SEGetLayerStack() const; 

	// Custom Iterators
	std::vector<SELayer*>::iterator begin() { return m_layer_stack.begin(); };
	std::vector<SELayer*>::iterator end()   { return m_layer_stack.end();   };

	
	
	std::vector<SELayer*> m_layer_stack;	// Stores all the layers available in the game. 
	SELayer* m_selected_layer;				// Pointer to toggle between which layer to operate on. 		
};		

extern std::unique_ptr<SELayerStack> SELayerStackManager;

//class SELayerSystem
//{
//public: 
//	SELayerInit();
//	SELayerUpdate();
//	SELayerExit();
//private:
//};