/******************************************************************************/
/*!
\file		Matrix3x3.cpp
\author 	Chua Kai Quan (100%)
\par    	email: kaiquan.chua@digipen.edu
\brief		This file contains the implementation of Constructors and
			non-member funtions of class Matrix3x3.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#include "pch.h"
#include "Matrix3x3.h"
#include "Vector2D.h"

namespace StormEngine
{
	// Conversion Constructor
	Matrix3x3::Matrix3x3(const float* pArr) : m00{ pArr[0] }, m01{ pArr[1] }, m02{ pArr[2] },
		m10{ pArr[3] }, m11{ pArr[4] }, m12{ pArr[5] },
		m20{ pArr[6] }, m21{ pArr[7] }, m22{ pArr[8] }
	{
	}


	// Conversion Constructor
	Matrix3x3::Matrix3x3(float _00, float _01, float _02,
		float _10, float _11, float _12,
		float _20, float _21, float _22) : m00{ _00 }, m01{ _01 }, m02{ _02 },
		m10{ _10 }, m11{ _11 }, m12{ _12 },
		m20{ _20 }, m21{ _21 }, m22{ _22 }
	{

	}
	// Assignment operator
	Matrix3x3& Matrix3x3::operator=(const Matrix3x3& rhs)
	{
		for (int i = 0; i < 9; ++i)
		{
			m[i] = rhs.m[i];
		}
		return *this;
	}
	// Multiplication Assignment operator
	Matrix3x3& Matrix3x3::operator *= (const Matrix3x3& rhs)
	{
		Matrix3x3 tmp;
		tmp.m00 = (m00 * rhs.m00) + (m01 * rhs.m10) + (m02 * rhs.m20);
		tmp.m01 = (m00 * rhs.m01) + (m01 * rhs.m11) + (m02 * rhs.m21);
		tmp.m02 = (m00 * rhs.m02) + (m01 * rhs.m12) + (m02 * rhs.m22);
		tmp.m10 = (m10 * rhs.m00) + (m11 * rhs.m10) + (m12 * rhs.m20);
		tmp.m11 = (m10 * rhs.m01) + (m11 * rhs.m11) + (m12 * rhs.m21);
		tmp.m12 = (m10 * rhs.m02) + (m11 * rhs.m12) + (m12 * rhs.m22);
		tmp.m20 = (m20 * rhs.m00) + (m21 * rhs.m10) + (m22 * rhs.m20);
		tmp.m21 = (m20 * rhs.m01) + (m21 * rhs.m11) + (m22 * rhs.m21);
		tmp.m22 = (m20 * rhs.m02) + (m21 * rhs.m12) + (m22 * rhs.m22);

		return (*this = tmp);
	}
	// Multiplication operator for 2 Matrices
	Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs)
	{
		Matrix3x3 tmp{ lhs };
		return tmp.operator*=(rhs);
	}
	// Multiplication operator for Matrix and Vector
	Vector2D  operator * (const Matrix3x3& pMtx, const Vector2D& rhs)
	{
		Vector2D result;
		result.x = (pMtx.m00 * rhs.x) + (pMtx.m01 * rhs.y) + (pMtx.m02);
		result.y = (pMtx.m10 * rhs.x) + (pMtx.m11 * rhs.y) + (pMtx.m12);
		return result;
	}
	// Identity Matrix
	void Mtx33Identity(Matrix3x3& pResult)
	{
		for (int i = 0; i < 9; ++i)
		{
			if (i % 4 == 0)
				pResult.m[i] = 1.0f;
			else
				pResult.m[i] = 0.0f;
		}
	}
	// Translation Matrix
	void Mtx33Translate(Matrix3x3& pResult, float x, float y)
	{
		Mtx33Identity(pResult);
		pResult.m02 = x;
		pResult.m12 = y;
	}
	// Scaling Matrix
	void Mtx33Scale(Matrix3x3& pResult, float x, float y)
	{
		Mtx33Identity(pResult);
		pResult.m00 *= x;
		pResult.m11 *= y;
	}
	// Rotation Matrix (radians)
	void Mtx33RotRad(Matrix3x3& pResult, float angle)
	{
		Mtx33Identity(pResult);
		pResult.m00 = (float)cos(angle);
		pResult.m01 = (float)-(sin(angle));
		pResult.m10 = (float)sin(angle);
		pResult.m11 = (float)cos(angle);
	}
	// Rotation Matrix (degrees)
	void Mtx33RotDeg(Matrix3x3& pResult, float angle)
	{
		angle = angle * (PI / 180.0f);
		Mtx33Identity(pResult);
		pResult.m00 = (float)cos(angle);
		pResult.m01 = (float)-(sin(angle));
		pResult.m10 = (float)sin(angle);
		pResult.m11 = (float)cos(angle);
	}

	// Transpose Matrix
	void Mtx33Transpose(Matrix3x3& pResult, const Matrix3x3& pMtx)
	{
		//top right
		pResult.m01 = pMtx.m10;
		pResult.m02 = pMtx.m20;
		pResult.m12 = pMtx.m21;
		// bottom left
		pResult.m10 = pMtx.m01;
		pResult.m20 = pMtx.m02;
		pResult.m21 = pMtx.m12;
		// Diagonal
		pResult.m00 = pMtx.m00;
		pResult.m11 = pMtx.m11;
		pResult.m22 = pMtx.m22;
	}
	// Inverse Matrix
	void Mtx33Inverse(Matrix3x3* pResult, float* determinant, const Matrix3x3& pMtx)
	{
		Matrix3x3 tmp = pMtx;
		*determinant = (pMtx.m00 * pMtx.m11) - (pMtx.m01 * pMtx.m10);

		if (*determinant <= EPSILON)
		{
			pResult = NULL;
		}
		else
		{
			float inverdet = 1 / *determinant;

			pResult->m00 = tmp.m11 * inverdet;
			pResult->m01 = -tmp.m01 * inverdet;
			pResult->m02 = tmp.m02;
			pResult->m10 = -tmp.m10 * inverdet;
			pResult->m11 = tmp.m00 * inverdet;
			pResult->m12 = tmp.m12;
			pResult->m20 = tmp.m20;
			pResult->m21 = tmp.m21;
			pResult->m22 = tmp.m22;
		}
	}
}