/******************************************************************************
 * \file		SEAudio.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (40%)
 * \par
 * \brief		Not used
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEFactory.h"



// Base Class - Game UI (inherited for general use)
class SEGuiElement
{
public:
	SEGuiElement(bool isActive, std::vector<Texture*> textures);

private:
	std::vector<Texture*> gui_textures; // a gui component might have multiple textures for multiple states (buttons - change texture when toggled)
	bool				  gui_isActive;
};

// For bodies like [Main Menu] that will contain - buttons etc. 
// [Example] : a main menu panel containing all the menu buttons
class SEGuiGroup 
{
public: 
	
private:
	std::vector<SEGuiElement*> panel_components; 

};

class SEButton : public SEGuiElement
{
public:
	enum class ButtonState // used to determine the state which the button is in and rendering the apprioriate textures. 
	{
		NOT_SELECTED,	
		SELECTED
	};
private:
	
	
};






// [Button Box] Background of this GUI (render box - possibly with textures)

// Different types of GUI
// 1. Panels (holding other forms of GUI)
// 2. Buttons 
// 3. Scroll Bar (?) - if that's needed.

// What consist of a button?
// 1. [Skeleton/Box]  - the 'motherboard' that holds other elements of ontop of it. 
// 2. [Collision Box] - to detect mouse-to-button key press / button press.
// 3. [Button State]  - (1) not active, (2) hovered, (3) active 


// What Systems Functions to include and what do they do?
// 1. [SEGuiSystemInit()] - to de-serialize the GUI components into the game

// What Data Members do we need?
// 1. [Texture UITexture] - Texture for the button         
// 2. [bool isActive]     - to render or not.