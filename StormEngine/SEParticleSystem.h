/******************************************************************************//**
 * \file		SEParticleSystem.h
 * \author 		Chua Kai Quan (100%)
 * \par    		email: kaiquan.chua@digipen.edu
 * \brief		This file contains the defination for ParticleSystem
 * \date   		March 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include <glm/glm.hpp>
#include <Timer.h>
#include <Random.h>

// resuable template
struct ParticleProps
{
    glm::vec2 Position;
    glm::vec2 Velocity, VelocityVariation;
    glm::vec4 ColorBegin, ColorEnd;
    float SizeBegin, SizeEnd, SizeVariation;
    float LifeTime = 1.0f;
};

class ParticleSystem {
public:
    ParticleSystem();

    void onUpdate(Timestep ts);
    void onRender();

    void Emit(const ParticleProps& particleProps); // emits 1 particle

private:
    
    // particle components
    struct Particle {
        glm::vec2 Position;
        glm::vec2 Velocity;
        glm::vec4 ColorBegin, ColorEnd;
        float Rotation = 0.0f;
        float SizeBegin, SizeEnd;

        float LifeTime = 1.0f;
        float LifeRemaining = 0.0f;

        bool Active = false;
    };

    std::vector<Particle> m_ParticlePool; // particlepool for efficiency
    uint32_t m_PoolIndex = 999; // rending in reverse order to blend in

    GLuint m_QuadVA = 0;

    //std::unique_ptr<GLCore::Utils::Shader> m_ParticleShader;
    GLint m_ParticleShaderViewProj{ 0 };
    GLint m_ParticleShaderTransform{ 0 };
    GLint m_ParticleShaderColor{ 0 };
};

