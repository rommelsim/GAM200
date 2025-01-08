/******************************************************************************//**
 * \file			SERigidBody.cpp
 * \author 			Loke Kheng Ray (100%)
 * \par				l.khengray@digipen.edu
 *
 * \brief			Physics Collision System
 *
 * \date   			January 2023

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.

******************************************************************************/
#pragma once
#include "pch.h"

namespace StormEngine
{
	/************************************************
	* @class		RigidBody
	* @brief		RigidBody Class
	***********************************************/
	class RigidBody
	{
		float MAX_HOR_VEL{ 200.f };
		float MAX_VER_VEL{ 200.f };

	public:
		// To be Initialized

		struct PhysValues
		{
			glm::vec2 bVel;
		};

		RigidBody::PhysValues p_val;

		// Default Constructor
		RigidBody();

		void Set_Velocity(glm::vec2 vel);
		glm::vec2 Get_Velocity();
	};
}