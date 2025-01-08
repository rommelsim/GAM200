/******************************************************************************/
/*!
\file		Vector3D.h
\author 	Loke Kheng Ray (100%)
\par    	email: kaiquan.chua@digipen.edu
\brief		This file contains the implementation of Constructors and
			non-member funtions of class Vector3D.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#pragma once
namespace StormEngine
{
#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( disable : 4201 )
#endif
	/************************************************
	* @class		Vector3D
	* @brief		Union of Vec3
	***********************************************/
	typedef union Vector3D
	{
		struct
		{
			float x, y, z;
		};

		float m[3];

		// Constructors
		Vector3D() : m{ 0 } {};
		Vector3D(float _x, float _y, float _z);

		// Assignment operators
		Vector3D& operator += (const Vector3D& rhs);
		Vector3D& operator -= (const Vector3D& rhs);
		Vector3D& operator *= (float rhs);
		Vector3D& operator /= (float rhs);

		// Unary operators
		Vector3D operator -() const;

	} Vector3D, Vec3, Point3D, Pt3;


#ifdef _MSC_VER
	// Supress warning: nonstandard extension used : nameless struct/union
#pragma warning( default : 4201 )
#endif

// Binary operators
	Vector3D operator + (const Vector3D& lhs, const Vector3D& rhs);
	Vector3D operator - (const Vector3D& lhs, const Vector3D& rhs);
	Vector3D operator * (const Vector3D& lhs, float rhs);
	Vector3D operator * (float lhs, const Vector3D& rhs);
	Vector3D operator / (const Vector3D& lhs, float rhs);

	/**************************************************************************/
	/*!
		In this function, pVec0 will be initialized to (0.f, 0.f, 0.f)
	 */
	 /**************************************************************************/
	void	Vector3DZero(Vector3D& ppVec0);

	/**************************************************************************/
	/*!
		In this function, pResult will be the unit vector of pVec0
	 */
	 /**************************************************************************/
	void	Vector3DNormalize(Vector3D& pResult, const Vector3D& pVec0);

	/**************************************************************************/
	/*!
		This function returns the length of the vector pVec0
	 */
	 /**************************************************************************/
	float	Vector3DLength(const Vector3D& pVec0);

	/**************************************************************************/
	/*!
		This function returns the square of pVec0's length. Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector3DSquareLength(const Vector3D& pVec0);

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 3D points.
		The distance between these 2 3D points is returned
	 */
	 /**************************************************************************/
	float	Vector3DDistance(const Vector3D& pVec0, const Vector3D& pVec1);

	/**************************************************************************/
	/*!
		In this function, pVec0 and pVec1 are considered as 3D points.
		The squared distance between these 2 3D points is returned.
		Avoid the square root
	 */
	 /**************************************************************************/
	float	Vector3DSquareDistance(const Vector3D& pVec0, const Vector3D& pVec1);

	/**************************************************************************/
	/*!
		This function returns the dot product between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vector3DDotProduct(const Vector3D& pVec0, const Vector3D& pVec1);

	/**************************************************************************/
	/*!
		This function returns the cross product magnitude
		between pVec0 and pVec1
	 */
	 /**************************************************************************/
	float	Vector3DCrossProductMag(const Vector3D& pVec0, const Vector3D& pVec1);
}