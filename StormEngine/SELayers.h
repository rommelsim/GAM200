/******************************************************************************//**
 * \file		SELayers.h
 * \author 		Cheong Ming Lun
 * \co-author
 * \par    		m.cheong@digipen.edu
 *
 * \brief		Layer System
 * \date   		January 2023

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
//#include "SEGraphics.h"		// For Rendering
#include "SEFactory.h"

class SELayer // Base class to create each layer that exists.
{
public:
	SELayer(std::string layer_name = "Layer");				// [Parameterized Constructor] - Define a name for this Layer.
	
	// [Functions that populate the layer array]
	void AddLayerObject(GameObject* gameobject);			// [Add Layer]      - Adds a new layer object into the vector container. 
	void SortObjectPriority();								// [Prioirty Queue] - sorts the gameObject in the order of render priority based on their "m_priority_level"
	void DeleteLayerObject(std::string go_name);
	void DeleteEverything();

	// [Accessor Functions]
	std::string GetLayerName() const;												// [Get] - this layer's name
	std::vector<GameObject*> GetLayerObjects() const;								// [Get] - a reference to our vector container storing the gameobjects in this layer. 
	SEUint GetLayerPriority() const;												// [Get][- this layer's priority level (used to determine render sequence)
	
	void SetObjectPriority(std::string object_name, int priority_level);			// [Set] - Specific gameObject in this layer's priority level. 
	void SetLayerPriority(int priority_level);										// [Set] - Set this layer's priority to the user defined [priority level] 

	//[Helper Functions] - std::sort algorithm
	static bool CompareObjectPriority(GameObject* first, GameObject* second);

private:
	std::vector<GameObject*> m_layer_gameobjects;									// [Container of GameObjects] - Vector to store the gameObjects to be rendered here. 													// [Priority Level]			  - determines when this layer gets rendered. (will be used in SELayerStack)
	SEUint m_layer_priority;
	std::string m_layer_name;														// [Layer's Name]			  - Give this layer a name!
	SERenderLayer m_layer;	
};


//class SELayerManager
//{
//public:
//	SELayerManager() = default; 
//	
//	// [Wrapper Function - to do populating of layer array based on component data] 
//	void SELayerInit();
//
//private:
//};
//


