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
#include "SERigidBody.h"

namespace StormEngine
{

	RigidBody::RigidBody()
	{
		p_val.bVel = glm::vec2{ 0.f, 0.f };
	}

	void RigidBody::Set_Velocity(glm::vec2 vel)
	{
		(void)vel;
		p_val.bVel = glm::vec2{ 0.f, 0.f };
	}

	glm::vec2 RigidBody::Get_Velocity()
	{
		return p_val.bVel;
	}
}