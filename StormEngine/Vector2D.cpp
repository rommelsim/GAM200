/******************************************************************************/
/*!
\file		Vector2D.cpp
\author 	Loke Kheng Ray (50%)
\co-author  Chua Kai Quan (50%)
\par    	email: kaiquan.chua@digipen.edu
\brief		This file contains the implementation of Constructors and
			non-member funtions of class Vector2D.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

/******************************************************************************//**
  * \file		Vector2D.cpp
  * \author 	Loke Kheng Ray (50%)
  * \co-author  Chua Kai Quan (50%)
  *
  * \brief		Vector2D Library
  *
  * \date		January 2023

 Copyright (C) 2023 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the
 prior written consent of DigiPen Institute of Technology is prohibited.
 ******************************************************************************/
#include "pch.h"
#include "Vector2D.h"

namespace StormEngine
{
	// Conversion Constructor
	Vector2D::Vector2D(float _x, float _y) : x{ _x }, y{ _y }
	{

	}

	// Assignment operators +=
	Vector2D& Vector2D::operator+=(const Vector2D& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	bool Vector2D::operator!=(const Vector2D& rhs)
	{
		if (this->x == rhs.x || this->y == rhs.y) {
			return false;
		}
		return true;
	}

	bool Vector2D::operator==(const Vector2D& rhs)
	{
		if (this->x == rhs.x || this->y == rhs.y) {
			return true;
		}
		return false;
	}
	// Assignment operators -=
	Vector2D& Vector2D::operator-=(const Vector2D& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}
	// Assignment operators *=
	Vector2D& Vector2D::operator*= (float rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}
	// Assignment operators /=
	Vector2D& Vector2D::operator/= (float rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	}

	// Unary Operator -
	Vector2D Vector2D::operator-() const
	{
		return Vector2D(-x, -y);
	}

	// Binary Operator +
	Vector2D operator + (const Vector2D& lhs, const Vector2D& rhs)
	{
		Vector2D tmp{ lhs };
		return tmp.operator+=(rhs);
	}

	// Binary Operator -
	Vector2D operator - (const Vector2D& lhs, const Vector2D& rhs)
	{
		Vector2D tmp{ lhs };
		return tmp.operator-=(rhs);
	}
	// Binary Operator *
	Vector2D operator * (const Vector2D& lhs, float rhs)
	{
		Vector2D tmp{ lhs };
		return tmp.operator*=(rhs);
	}
	// Binary Operator *
	Vector2D operator * (float lhs, const Vector2D& rhs)
	{
		Vector2D tmp{ rhs };
		return tmp.operator*=(lhs);
	}
	// Binary Operator /
	Vector2D operator / (const Vector2D& lhs, float rhs)
	{
		Vector2D tmp{ lhs };
		return tmp.operator/=(rhs);
	}
	// Unit Vector
	void Vector2DNormalize(Vector2D& pResult, const Vector2D& pVec0)
	{
		float magnitude = Vector2DLength(pVec0);
		if (magnitude == 0.0f)
		{
			pResult.x = 0.0f;
			pResult.y = 0.0f;
		}
		else
		{
			pResult.x = pVec0.x / magnitude;
			pResult.y = pVec0.y / magnitude;
		}
	}
	// Length of the Vector
	float Vector2DLength(const Vector2D& pVec0)
	{
		float magnitude = Vector2DSquareLength(pVec0);
		magnitude = sqrt(magnitude);
		return magnitude;
	}
	// Square Length of the Vector
	float Vector2DSquareLength(const Vector2D& pVec0)
	{
		float sqr_len = (pVec0.x * pVec0.x) + (pVec0.y * pVec0.y);
		return sqr_len;
	}
	// Distance between 2 Vectors
	float Vector2DDistance(const Vector2D& pVec0, const Vector2D& pVec1)
	{
		float distance = Vector2DLength(pVec1 - pVec0);
		return distance;
	}
	// Square Distance between 2 Vectors
	float Vector2DSquareDistance(const Vector2D& pVec0, const Vector2D& pVec1)
	{
		float sqr_distance = Vector2DSquareLength(pVec1 - pVec0);
		return sqr_distance;
	}
	// Dot Product between 2 Vectors
	float Vector2DDotProduct(const Vector2D& pVec0, const Vector2D& pVec1)
	{
		float a = pVec0.x * pVec1.x;
		float b = pVec0.y * pVec1.y;
		// a.b
		return a + b;
	}
	//  Cross Product between 2 Vectors
	float Vector2DCrossProductMag(const Vector2D& pVec0, const Vector2D& pVec1)
	{
		float a = pVec0.x * pVec1.y;
		float b = pVec0.y * pVec1.x;
		// a x b
		return a - b;
	}
}