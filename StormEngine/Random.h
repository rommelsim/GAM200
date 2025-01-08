/******************************************************************************//**
 * \file			Random.h
 * \author 			Khairul	    (100%)
 * \par    			m.binroslan@digipen.edu
 *
 *
 * \brief			Random Class
 *
 * \date			January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include <random>
class Random
{
public:
    static void Init()
    {
        s_RandomEngine.seed(std::random_device()());
    }

    static float Float()
    {
        return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max(); // returns number b/w 0 and 1
    }

private:
    inline static std::mt19937 s_RandomEngine;
    inline static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};