/******************************************************************************//**
 * \file			SELogger.h
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
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
namespace SE {
	class Logs
	{
	public:

		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
			return s_CoreLogger;
		}
		static std::shared_ptr<spdlog::logger>& GetClientLogger() {
			return s_ClientLogger;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

//Macros for logging
//Trace is to track variable changes during execution like window size 
#define SE_CORE_TRACE(...) ::SE::Logs::GetCoreLogger()->trace(__VA_ARGS__)
#define SE_CORE_INFO(...)  ::SE::Logs::GetCoreLogger()->info(__VA_ARGS__)
#define SE_CORE_WARN(...)  ::SE::Logs::GetCoreLogger()->warn(__VA_ARGS__)
#define SE_CORE_ERROR(...) ::SE::Logs::GetCoreLogger()->error(__VA_ARGS__)
#define SE_CORE_CRITICAL(...) ::SE::Logs::GetCoreLogger()->critical(__VA_ARGS__)
//Check if assertion failed and print on console window and where it happened
#define SE_CORE_ASSERT(x, ...) {if((x)) { SE_CORE_CRITICAL("Assert Failed: {} {}\n\tIn File:{}\n\tLine: {}",x, __VA_ARGS__,__FILE__,__LINE__);__debugbreak();}}

#define SE_TRACE(...) ::SE::Logs::GetClientLogger()->trace(__VA_ARGS__)
#define SE_INFO(...)  ::SE::Logs::GetClientLogger()->info(__VA_ARGS__)
#define SE_WARN(...)  ::SE::Logs::GetClientLogger()->warn(__VA_ARGS__)
#define SE_ERROR(...) ::SE::Logs::GetClientLogger()->error(__VA_ARGS__)
#define SE_FATAL(...) ::SE::Logs::GetClientLogger()->fatal(__VA_ARGS__)