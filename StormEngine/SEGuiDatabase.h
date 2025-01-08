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
#include "SEGui.h" // [SEGuiGroup]

void SEGuiElementsInit();


class SEGuiCollection
{
public:

private:
	std::vector<SEGuiGroup*> gui_database;
};