/******************************************************************************//**
 * \file		SEAnimation.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (100%)
 *
 * \brief		Handles Animation
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEFactory.h"
#include "SEGraphics.h"

void SEAnimationInit();
void SEAnimationUpdate();
void SEAnimationExit();
void SEPlayAnimation(std::string objectname, ACTION action);