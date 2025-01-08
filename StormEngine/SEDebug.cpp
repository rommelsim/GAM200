/******************************************************************************//**
 * \file			main.cpp
 * \author 			Rommel Sim (100%)
 * \par    	email: zhenpengrommel.sim@digipen.edu
 * \brief	This file contains the declaration for debug.
 * \date   	January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SEDebug.h"

std::unique_ptr<std::ofstream> OStream;

void SEWriterInit(std::filesystem::path path)
{
	OStream = std::make_unique<std::ofstream>(path.string(), std::ofstream::out);
	*OStream << "file writer ok\n";
}

void SEWriterExit()
{
	*OStream << "file writer shutting down\n";
	(*OStream).close();
}

void SEWrite(std::string text)
{
	*OStream << text << std::endl;
}

void SEWrite(const char* text)
{
	*OStream << text << std::endl;
}