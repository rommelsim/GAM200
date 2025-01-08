/******************************************************************************//**
 * \file		SEEngine.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (100%)
 *
 * \brief		Header file containing all neccsseary header files for sub systems.
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once


// pre-compiled headers
#include "pch.h"				


// Storm Engine Includes
#include "SEGraphics.h"
#include "SEResource.h"
#include "SEFactory.h"
#include "SECollision.h"
#include "SEFactory.h"
#include "SEAudio.h"
#include "SELogger.h"
#include "SEAnimation.h"
#include "SESerialiser.h"
#include "SELayers.h"
#include "SELayerStack.h"
#include "SEDebug.h"
#include "SEScripting.h"
#include "SceneManager.h"
#include "SEDialogue.h"

#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

/**
	 * Storm Engine Init
 */
void SEInit();

/**
	 * Storm Engine Init
 */
void SEExit();
