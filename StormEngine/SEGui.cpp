/******************************************************************************
 * \file		SEAudio.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (40%)
 * \par
 * \brief		Not Used
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEGui.h"


void SEGuiSystemInit()
{

}


SEGuiElement::SEGuiElement(bool isActive, std::vector<Texture*> texture) : gui_isActive(isActive), gui_textures(texture)
{

}



