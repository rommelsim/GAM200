/******************************************************************************//**
 * \file			SELogger.cpp
 * \author 			Rommel Sim (50%)
 * \co-author		Loke Kheng Ray (50%)
 * \par				zhenpengrommel.sim@digipen.edu
 * \par				l.khengray@digipen.edu
 * \brief			Logging System
 *
 * \date   			January 2023

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SELogger.h"

namespace SE {
	//Shared pointer declaration for Core and Client side
	std::shared_ptr<spdlog::logger> Logs::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logs::s_ClientLogger;
	void Logs::Init() {
		//set pattern output in console window
		spdlog::set_pattern("%^[%b:%d %T] %n[%l]: %v%$");

		//Create a multhithreaded console logger 
		s_CoreLogger = spdlog::stdout_color_mt("Core");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_CoreLogger = spdlog::stdout_color_mt("Client");
		s_CoreLogger->set_level(spdlog::level::trace);
#if _DEBUG
		SE_CORE_INFO("System Initialize");
#endif
	}

}