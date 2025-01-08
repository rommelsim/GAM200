/******************************************************************************//**
 * \file			SELayers.cpp
 * \author 			Cheong Ming Lun  (100%)
 * \par    			m.cheong@digipen.edu
 *
 *
 * \brief			Layer System
 *
 * \date   			January 2023

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SELayers.h"
#include "SEFactory.h"  // Access to goArray (for <Texture> component) -> has our layer data

// Constructor with [Layer's Name]
SELayer::SELayer(std::string layer_name) : m_layer_name(layer_name)
{

}

// [Helper Functions]
void SELayer::AddLayerObject(GameObject* gameObject)
{
	// Fixed on 25/1/23 by Rommel.
	// TODO: Stress test logic. Current logic implemented here is to prevent infinite adding of objects into layer.
	bool HasDuplicated = false;

	std::string layername = m_layer_name;

	Texture* go_tex = (Texture*)gameObject->getComponent(CT_TEXTURE); // we need the render layer from this component. 

	if (go_tex)
	{
		if (m_layer_name == "Background")
		{
			go_tex->m_render_layer = SERenderLayer::BACKGROUND;
		}

		else if (m_layer_name == "Gameplay")
		{
			go_tex->m_render_layer = SERenderLayer::GAMEPLAY;
		}

		else if (m_layer_name == "Foreground")
		{
			go_tex->m_render_layer = SERenderLayer::FOREGROUND;
		}

		else if (m_layer_name == "UI Elements")
		{
			go_tex->m_render_layer = SERenderLayer::UI_ELEMENTS;
		}

		else if (m_layer_name == "Overlay")
		{
			go_tex->m_render_layer = SERenderLayer::OVERLAY;
		} 
	}

	if(m_layer_gameobjects.empty())
		m_layer_gameobjects.push_back(gameObject); // pushes this object into the stack. 

	
	for (const auto& obj : m_layer_gameobjects)
	{
		if (obj == gameObject)
			HasDuplicated = true;
	}
	

	if (!HasDuplicated)
		m_layer_gameobjects.push_back(gameObject);

	HasDuplicated = false;
}

void SELayer::SortObjectPriority()
{	
	std::sort(m_layer_gameobjects.begin(), m_layer_gameobjects.end(), CompareObjectPriority);
	
}

void SELayer::DeleteEverything()
{
	for (GameObject* go : m_layer_gameobjects)
	{
		delete go;
	}
}

void SELayer::DeleteLayerObject(std::string go_name)
{	
	int i = 0;
	for (GameObject* go : m_layer_gameobjects)
	{
		if (go->getName() == go_name)
		{
			m_layer_gameobjects.erase(m_layer_gameobjects.begin() + i);
			break;
		}
		i++;
	}
	// check if current layer contains any objects? If dont have, clear all entites in the layer!

}

bool SELayer::CompareObjectPriority(GameObject* first, GameObject* second)
{
	if (first && second)
	{
		Texture* go1 = (Texture*)first->getComponent(CT_TEXTURE);
		Texture* go2 = (Texture*)second->getComponent(CT_TEXTURE);

		if (go1 && go2)
		{

			int p1 = go1->m_priority_level; (void)p1;
			int p2 = go2->m_priority_level; (void)p2;

			bool final = go1->m_priority_level < go2->m_priority_level;

			return final;
		}
	}
	return 0;
}

std::string SELayer::GetLayerName() const
{
	return m_layer_name;
}

std::vector<GameObject*> SELayer::GetLayerObjects() const
{
	return m_layer_gameobjects;
}

SEUint SELayer::GetLayerPriority() const
{
	return m_layer_priority;

}

void SELayer::SetObjectPriority(std::string object_name, int priority_level)
{
	for (GameObject* go : m_layer_gameobjects)
	{
		if (go->getName() == object_name)
		{
			Texture* tex = (Texture*)go->getComponent(CT_TEXTURE);

			if (tex)
				tex->m_priority_level = priority_level;
		}
	}

	SortObjectPriority(); 
}

void SELayer::SetLayerPriority(int priority_level)
{
	m_layer_priority = priority_level; 
}