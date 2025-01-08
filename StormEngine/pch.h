// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

/******************************************************************************//**
 * \file		pch.h
 * \author 		Rommel Sim
 * \co-author
 * \par    		zhenpengrommel.sim@digipen.edu (100%)
 *
 * \brief		precompiled header files
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#ifndef PCH_H
#define PCH_H
#pragma message(" SOLDIR == " SOLUTIONDIR) // will print project dir during compilation

// add headers that you want to pre-compile here
#include "framework.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include <iomanip>
#include <array>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <array>
#include <algorithm>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdarg.h>
#include <filesystem>

#include <glm/gtc/type_ptr.hpp>
#include <glew.h>
#include <glfw3.h>

// Texture Loading
#include <stb_image.h>

// Font Loading
#include <ft2build.h>				
#include FT_FREETYPE_H

// Audio Loading
#include <fmod.hpp>

// JSON 
#define RAPIDJSON_HAS_STDSTRING	1	// allow us to use std::string with JSON stuffs.
#include <document.h>
#include <stringbuffer.h>
#include <writer.h>
#include <ostreamwrapper.h>


using SEBool	= bool;
using SEFloat	= float;
using SEInt		= int;
using SEUint	= GLuint;
using SEVec2	= glm::vec2;
using SEVec3	= glm::vec3;
using SEVec4	= glm::vec4;
using SEMat3	= glm::mat3;


static bool IG_debug = false;

inline std::ostream& operator<<(std::ostream& os, const SEVec2& vec)
{
	os << "(" << vec.x << ", " << vec.y << ")" << std::endl;
	return os;
}

#define PRINT(...)	printf(__VA_ARGS__)

#endif //PCH_H
