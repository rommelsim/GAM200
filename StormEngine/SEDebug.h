/******************************************************************************//**
 * \file		SEDebug.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (100%)
 *
 * \brief		Used during release mode. Will spit out files on user's device. Use for debugging without VS compiler.
 * 
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"

/**
	 * Init Stream Writer Handler.
	 * Specifiy the name of the file
	 * Must be called on Init phase.
 */
void SEWriterInit(std::filesystem::path path);

/**
	 * Destroy Stream writer handler
 */
void SEWriterExit();

/**
	 * Use current Stream writer handler to write debug messages into the path initialsed above.
 */
void SEWrite(std::string text);

/**
	 * Use current Stream writer handler to write debug messages into the path initialsed above.
 */
void SEWrite(const char* text);
