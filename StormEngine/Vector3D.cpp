/******************************************************************************//**
  * \file		Vector3D.cpp
  * \author 	Loke Kheng Ray (50%)
  * \co-author  Chua Kai Quan (50%)
  *
  * \brief		Vector3D Library
  *
  * \date		January 2023

 Copyright (C) 2023 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the
 prior written consent of DigiPen Institute of Technology is prohibited.
 ******************************************************************************/
#include "pch.h"
#include "Vector3D.h"
#include <cmath>

namespace StormEngine
{
	/**************************************************************************/
	/*!
		Conversion Constructor
		This function constructs a vector and initialising it with float _x,
		float _y amd float _z.
	 */
	 /**************************************************************************/
	Vector3D::Vector3D(float _x, float _y, float _z) : x{ _x }, y{ _y }, z{ _z }
	{ }

	/**************************************************************************/
	/*!
		Assignment Operator (+=)
		This function returns the vector after adding by vector rhs.
	 */
	 /**************************************************************************/
	Vector3D& Vector3D::operator += (const Vector3D& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	/**************************************************************************/
	/*!
		Assignment Operator (-=)
		This function returns the vector after subtracting by vector rhs.
	 */
	 /**************************************************************************/
	Vector3D& Vector3D::operator -= (const Vector3D& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	/**************************************************************************/
	/*!
		Assignment Operator (*=)
		This function returns the vector after multiplying by float rhs.
	 */
	 /**************************************************************************/
	Vector3D& Vector3D::operator *= (float rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	/**************************************************************************/
	/*!
		Assignment Operator (/=)
		This function returns the vector after dividing by float rhs.
	 */
	 /**************************************************************************/
	Vector3D& Vector3D::operator /= (float rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	/**************************************************************************/
	/*!
		Unary Operator
		This function returns the negative vector.
	 */
	 /**************************************************************************/
	Vector3D Vector3D::operator -() const
	{
		Vector3D sub{ -x, -y, -z };
		return sub;
	}

	/**************************************************************************/
	/*!
		Binary Operator (+)
		This function returns the addition of vector lhs and vector rhs.
	 */
	 /**************************************************************************/
	Vector3D operator + (const Vector3D& lhs, const Vector3D& rhs)
	{
		Vector3D add{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
		return add;
	}

	/**************************************************************************/
	/*!
		Binary Operator (-)
		This function returns the difference between vector lhs and vector rhs.
	 */
	 /**************************************************************************/
	Vector3D operator - (const Vector3D& lhs, const Vector3D& rhs)
	{
		Vector3D sub{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
		return sub;
	}

	/**************************************************************************/
	/*!
		Binary Operator (*) - Overloaded
		This function returns the multiplication of vector lhs and float rhs.
	 */
	 /**************************************************************************/
	Vector3D operator * (const Vector3D& lhs, float rhs)
	{
		Vector3D mul{ lhs.x * rhs, lhs.y * rhs, lhs.z * rhs };
		return mul;
	}

	/**************************************************************************/
	/*!
		Binary Operator (*) - Overloaded
		This function returns the multiplication of float lhs and vector rhs.
	 */
	 /**************************************************************************/
	Vector3D operator * (float lhs, const Vector3D& rhs)
	{
		Vector3D mul{ lhs * rhs.x, lhs * rhs.y, lhs * rhs.z };
		return mul;
	}

	/**************************************************************************/
	/*!
		Binary Operator (/)
		This function returns the division of vector lhs and vector rhs.
	 */
	 /**************************************************************************/
	Vector3D operator / (const Vector3D& lhs, float rhs)
	{
		Vector3D div{ lhs.x / rhs, lhs.y / rhs, lhs.z / rhs };
		return div;
	}

	/**************************************************************************/
	/*!
		In this function, pVec0 will be initialized to (0.f, 0.f, 0.f)
	 */
	 /**************************************************************************/
	void	Vector3DZero(Vector3D& pVec0)
	{
		pVec0.x = 0.f;
		pVec0.y = 0.f;
		pVec0.z = 0.f;
	}

	/**************************************************************************/
	/*!
		Normalize Function
		In this function, pResult will be the unit vector of pVec0
	 */
	 /**************************************************************************/
	void	Vector3DNormalize(Vector3D& pResult, const Vector3D& pVec0)
	{
		double mag =
			sqrt(static_cast<double>(pVec0.x) * static_cast<double>(pVec0.x) +
				static_cast<double>(pVec0.y) * static_cast<double>(pVec0.y) +
				static_cast<double>(pVec0.z) * static_cast<double>(pVec0.z));
		pResult = pVec0 / static_cast<float>(mag);
	}

	/**************************************************************************/
	/*!
		Vector Length Function
		This function returns the length of the vector pVec0
	 */
	 /**************************************************************************/
	float	Vector3DLength(const Vector3D& pVec0)
	{
		double mag =
			sqrt(static_cast<double>(pVec0.x) * static_cast<double>(pVec0.x) +
				static_cast<double>(pVec0.y) * static_cast<double>(pVec0.y) +
				static_cast<double>(pVec0.z) * static_cast<double>(pVec0.z));
		return (float)mag;
	}

	/**************************************************************************/
	/*!
		Vector Length Sqaured Function
		This function returns the square of pVec0's length.
		Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector3DSquareLength(const Vector3D& pVec0)
	{
		float sqmag = pVec0.x * pVec0.x + pVec0.y * pVec0.y + pVec0.z * pVec0.z;
		return sqmag;
	}

	/**************************************************************************/
	/*!
		Distance Function
		In this function, pVec0 and pVec1 are considered as 3D points.
		The distance between these 2 3D points is returned
	 */
	 /**************************************************************************/
	float	Vector3DDistance(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		float x = pVec1.x - pVec0.x;
		float y = pVec1.y - pVec0.y;
		float z = pVec1.z - pVec0.z;
		double dist = sqrt(static_cast<double>(x) * static_cast<double>(x)
			+ static_cast<double>(y) * static_cast<double>(y)
			+ static_cast<double>(z) * static_cast<double>(z));
		return static_cast<float>(dist);
	}

	/**************************************************************************/
	/*!
		Distance Squared Function
		In this function, pVec0 and pVec1 are considered as 3D points.
		The squared distance between these 2 3D points is returned.
		Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector3DSquareDistance(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		float x = pVec1.x - pVec0.x;
		float y = pVec1.y - pVec0.y;
		float z = pVec1.z - pVec0.z;
		float sqdist = x * x + y * y + z * z;
		return sqdist;
	}

	/**************************************************************************/
	/*!
		Dot Product Function
		This function returns the dot product between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vector3DDotProduct(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		float dpro = pVec1.x * pVec0.x + pVec1.y * pVec0.y + pVec1.z * pVec0.z;
		return dpro;
	}

	/**************************************************************************/
	/*!
		Cross Product Function
		This function returns the cross product magnitude between
		pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vector3DCrossProductMag(const Vector3D& pVec0, const Vector3D& pVec1)
	{
		float r1 = pVec0.y * pVec1.z - pVec1.y * pVec0.z;
		float r2 = pVec0.z * pVec1.x - pVec1.z * pVec0.x;
		float r3 = pVec0.x * pVec1.y - pVec1.x * pVec0.y;

		double cpro_mag = sqrt(static_cast<double>(r1) * static_cast<double>(r1)
			+ static_cast<double>(r2) * static_cast<double>(r2)
			+ static_cast<double>(r3) * static_cast<double>(r3));
		return static_cast<float>(cpro_mag);
	}
}
