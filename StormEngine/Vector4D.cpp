/******************************************************************************//**
  * \file		Vector4D.cpp
  * \author 	Loke Kheng Ray (50%)
  * \co-author  Chua Kai Quan (50%)
  *
  * \brief		Vector4D Library
  *
  * \date		January 2023

 Copyright (C) 2023 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the
 prior written consent of DigiPen Institute of Technology is prohibited.
 ******************************************************************************/
#include "pch.h"
#include <GLM/vec4.hpp>
#include "Vector4D.h"

namespace StormEngine
{
	// Conversion Constructor
	Vector4D::Vector4D(float _x, float _y, float _z, float _w) : x{ _x }, y{ _y }, z{ _z }, w{ _w }
	{

	}

	// Assignment operators +=
	Vector4D& Vector4D::operator+=(const Vector4D& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;
		return *this;
	}
	// Assignment operators -=
	Vector4D& Vector4D::operator-=(const Vector4D& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;
		return *this;
	}
	// Assignment operators *=
	Vector4D& Vector4D::operator*= (float rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		this->z *= rhs;
		this->w *= rhs;
		return *this;
	}
	// Assignment operators /=
	Vector4D& Vector4D::operator/= (float rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		this->z /= rhs;
		this->w /= rhs;
		return *this;
	}

	// Unary Operator -
	Vector4D Vector4D::operator-() const
	{
		return Vector4D(-x, -y, -z, -w);
	}

	// Binary Operator +
	Vector4D operator + (const Vector4D& lhs, const Vector4D& rhs)
	{
		return Vector4D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
	}

	// Binary Operator -
	Vector4D operator - (const Vector4D& lhs, const Vector4D& rhs)
	{
		return Vector4D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
	}

	// Binary Operator *
	Vector4D operator * (const Vector4D& lhs, float rhs)
	{
		return Vector4D(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w + rhs);
	}

	// Binary Operator *
	Vector4D operator * (float lhs, const Vector4D& rhs)
	{
		return Vector4D(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs + rhs.z);
	}

	// Binary Operator /
	Vector4D operator / (const Vector4D& lhs, float rhs)
	{
		return Vector4D(lhs.x / rhs, lhs.y * rhs, lhs.z * rhs, lhs.w + rhs);
	}

	// Comparision Operator !=
	bool Vector4D::operator != (const Vector4D& rhs)
	{
		return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w);
	}

	// Comparision Operator ==
	bool Vector4D::operator == (const Vector4D& rhs)
	{
		return !(*this != rhs);
	}

	// overload the ostream operator 
	std::ostream& operator<<(std::ostream& out, const Vector4D& vector)
	{
		out << "[" << vector.x << ", " << vector.y << ", " << vector.z << ", "
			<< vector.w << "]";
		return out;
	}

}