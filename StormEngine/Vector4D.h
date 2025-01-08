/******************************************************************************/
/*!
\file		Vector4D.h
\author 	Chua Kai Quan (100%)
\par    	email: kaiquan.chua@digipen.edu
\brief		This file contains the implementation of Constructors and
			non-member funtions of class Vector4D.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
#include <iostream>

namespace StormEngine
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif

	typedef union Vector4D
	{
		struct
		{
			float x, y, z, w;
		};

		float m[2];

		// Constructors
		Vector4D() = default;
		Vector4D(float _x, float _y, float _z, float _w);

		// Assignment operators
		Vector4D& operator += (const Vector4D& rhs);
		Vector4D& operator -= (const Vector4D& rhs);
		Vector4D& operator *= (float rhs);
		Vector4D& operator /= (float rhs);

		// Unary operators
		Vector4D operator -() const;

		// Comparision opeators
		bool operator != (const Vector4D& rhs);
		bool operator == (const Vector4D& rhs);

		// overload the std::ostream operator
		friend std::ostream& operator<<(std::ostream& out, const Vector4D& vector);

	} Vector4D, Vec4, Point4D, Pt4;

// Binary operators
	Vector4D operator + (const Vector4D& lhs, const Vector4D& rhs);
	Vector4D operator - (const Vector4D& lhs, const Vector4D& rhs);
	Vector4D operator * (const Vector4D& lhs, float rhs);
	Vector4D operator * (float lhs, const Vector4D& rhs);
	Vector4D operator / (const Vector4D& lhs, float rhs);
}