/***************************************************************************//**
\file		Matrix4x4.cpp
\author 	Chua Kai Quan (100%)
\par    	email: kaiquan.chua@digipen.edu
\brief		This file contains the implementation of Constructors and
			non-member funtions of class Matrix4x4.

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
********************************************************************************/
#include "pch.h"
#include "Matrix4x4.h"
#include <cmath>

#define PI			3.14159265358f

namespace StormEngine
{
    /**************************************************************************/
    /*!
        Parametrized Constructor - Pointer to Array
        This function constructs a Matrix3x3 and initialising it with values
        in pArr.
     */
     /**************************************************************************/
    Matrix4x4::Matrix4x4(const float* pArr)
        : m00{ *pArr }, m01{ *(pArr + 1) }, m02{ *(pArr + 2) }, m03{ *(pArr + 3) },
        m10{ *(pArr + 4) }, m11{ *(pArr + 5) }, m12{ *(pArr + 6) }, m13{ *(pArr + 7) },
        m20{ *(pArr + 8) }, m21{ *(pArr + 9) }, m22{ *(pArr + 10) }, m23{ *(pArr + 11) },
        m30{ *(pArr + 12) }, m31{ *(pArr + 13) }, m32{ *(pArr + 14) }, m33{ *(pArr + 15) } { }

    /**************************************************************************/
    /*!
        Parametrized Constructor - Pass in Values
        This function constructs a vector and initialising it with the values
        passed in.
     */
     /**************************************************************************/
    Matrix4x4::Matrix4x4(float _00, float _01, float _02, float _03,
        float _10, float _11, float _12, float _13,
        float _20, float _21, float _22, float _23,
        float _30, float _31, float _32, float _33)
        : m00{ _00 }, m01{ _01 }, m02{ _02 }, m03{ _03 },
        m10{ _10 }, m11{ _11 }, m12{ _12 }, m13{ _13 },
        m20{ _20 }, m21{ _21 }, m22{ _22 }, m23{ _23 },
        m30{ _30 }, m31{ _31 }, m32{ _32 }, m33{ _33 } { }

    /**************************************************************************/
    /*!
        Copy Assignment Operator (=)
        This function copies the values of each element from one Matrix3x3 to
        another Matrix3x3.
     */
     /**************************************************************************/
    Matrix4x4& Matrix4x4::operator=(const Matrix4x4& rhs)
    {
        m00 = rhs.m00;
        m01 = rhs.m01;
        m02 = rhs.m02;
        m03 = rhs.m03;
        m10 = rhs.m10;
        m11 = rhs.m11;
        m12 = rhs.m12;
        m13 = rhs.m13;
        m20 = rhs.m20;
        m21 = rhs.m21;
        m22 = rhs.m22;
        m23 = rhs.m23;
        m30 = rhs.m30;
        m31 = rhs.m31;
        m32 = rhs.m32;
        m33 = rhs.m33;

        return *this;
    }

    /**************************************************************************/
    /*!
        Assignment Operator (*=)
        This function returns the Matrix3x3 after multiplying with another
        Matrix3x3.
     */
     /**************************************************************************/
    Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& rhs)
    {
        m00 = m00 * rhs.m00 + m01 * rhs.m10 + m02 * rhs.m20 + m03 * rhs.m30;
        m01 = m00 * rhs.m01 + m01 * rhs.m11 + m02 * rhs.m21 + m03 * rhs.m31;
        m02 = m00 * rhs.m02 + m01 * rhs.m12 + m02 * rhs.m22 + m03 * rhs.m32;
        m03 = m00 * rhs.m03 + m01 * rhs.m13 + m02 * rhs.m23 + m03 * rhs.m33;
        m10 = m10 * rhs.m00 + m11 * rhs.m10 + m12 * rhs.m20 + m13 * rhs.m30;
        m11 = m10 * rhs.m01 + m11 * rhs.m11 + m12 * rhs.m21 + m13 * rhs.m31;
        m12 = m10 * rhs.m02 + m11 * rhs.m12 + m12 * rhs.m22 + m13 * rhs.m32;
        m13 = m10 * rhs.m03 + m11 * rhs.m13 + m12 * rhs.m23 + m13 * rhs.m33;
        m20 = m20 * rhs.m00 + m21 * rhs.m10 + m22 * rhs.m20 + m23 * rhs.m30;
        m21 = m20 * rhs.m01 + m21 * rhs.m11 + m22 * rhs.m21 + m23 * rhs.m31;
        m22 = m20 * rhs.m02 + m21 * rhs.m12 + m22 * rhs.m22 + m23 * rhs.m32;
        m23 = m20 * rhs.m03 + m21 * rhs.m13 + m22 * rhs.m23 + m23 * rhs.m33;
        m30 = m30 * rhs.m00 + m31 * rhs.m10 + m32 * rhs.m20 + m33 * rhs.m30;
        m31 = m30 * rhs.m01 + m31 * rhs.m11 + m32 + rhs.m21 + m33 * rhs.m31;
        m32 = m30 * rhs.m02 + m31 * rhs.m12 + m32 + rhs.m22 + m33 * rhs.m32;
        m33 = m30 * rhs.m03 + m31 * rhs.m13 + m32 * rhs.m23 + m33 * rhs.m33;

        return *this;
    }


    /**************************************************************************/
    /*!
        Matrix Multipication Function
        This operator multiplies the matrix pMtx with another matrix rhs
        and returns the result as a vector
     */
     /**************************************************************************/
    Matrix4x4 operator * (const Matrix4x4& lhs, const Matrix4x4& rhs)
    {
        Matrix4x4 mul;

        mul.m00 = lhs.m00 * rhs.m00 + lhs.m01 * rhs.m10 + lhs.m02 * rhs.m20 + lhs.m03 * rhs.m30;
        mul.m01 = lhs.m00 * rhs.m01 + lhs.m01 * rhs.m11 + lhs.m02 * rhs.m21 + lhs.m03 * rhs.m31;
        mul.m02 = lhs.m00 * rhs.m02 + lhs.m01 * rhs.m12 + lhs.m02 * rhs.m22 + lhs.m03 * rhs.m32;
        mul.m03 = lhs.m00 * rhs.m03 + lhs.m01 * rhs.m13 + lhs.m02 * rhs.m23 + lhs.m03 * rhs.m33;
        mul.m10 = lhs.m10 * rhs.m00 + lhs.m11 * rhs.m10 + lhs.m12 * rhs.m20 + lhs.m13 * rhs.m30;
        mul.m11 = lhs.m10 * rhs.m01 + lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31;
        mul.m12 = lhs.m10 * rhs.m02 + lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32;
        mul.m13 = lhs.m10 * rhs.m03 + lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33;
        mul.m20 = lhs.m20 * rhs.m00 + lhs.m21 * rhs.m10 + lhs.m22 * rhs.m20 + lhs.m23 * rhs.m30;
        mul.m21 = lhs.m20 * rhs.m01 + lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31;
        mul.m22 = lhs.m20 * rhs.m02 + lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32;
        mul.m23 = lhs.m20 * rhs.m03 + lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33;
        mul.m30 = lhs.m30 * rhs.m00 + lhs.m31 * rhs.m10 + lhs.m32 * rhs.m20 + lhs.m33 * rhs.m30;
        mul.m31 = lhs.m30 * rhs.m01 + lhs.m31 * rhs.m11 + lhs.m32 + rhs.m21 + lhs.m33 * rhs.m31;
        mul.m32 = lhs.m30 * rhs.m02 + lhs.m31 * rhs.m12 + lhs.m32 + rhs.m22 + lhs.m33 * rhs.m32;
        mul.m33 = lhs.m30 * rhs.m03 + lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33;

        return mul;
    }

    /**************************************************************************/
    /*!
        Matrix and Vector Multiplication Function
        This operator multiplies the matrix pMtx with the vector rhs
        and returns the result as a vector
     */
     /**************************************************************************/
    Vector3D  operator * (const Matrix4x4& pMtx, const Vector3D& rhs)
    {
        Vector3D mul;

        mul.x = pMtx.m00 * rhs.x + pMtx.m01 * rhs.y + pMtx.m02 * rhs.z + pMtx.m03 * 1.0f;
        mul.y = pMtx.m10 * rhs.x + pMtx.m11 * rhs.y + pMtx.m12 * rhs.z + pMtx.m13 * 1.0f;
        mul.z = pMtx.m20 * rhs.x + pMtx.m21 * rhs.y + pMtx.m23 * rhs.z + pMtx.m23 * 1.0f;

        return mul;
    }

    /**************************************************************************/
    /*!
        Identity Function
        This function sets the matrix pResult to the identity matrix
     */
     /**************************************************************************/
    void Mtx44Identity(Matrix4x4& pResult)
    {
        pResult.m00 = 1.0f;
        pResult.m01 = 0.0f;
        pResult.m02 = 0.0f;
        pResult.m03 = 0.0f;
        pResult.m10 = 0.0f;
        pResult.m11 = 1.0f;
        pResult.m12 = 0.0f;
        pResult.m13 = 0.0f;
        pResult.m20 = 0.0f;
        pResult.m21 = 0.0f;
        pResult.m22 = 1.0f;
        pResult.m23 = 0.0f;
        pResult.m30 = 0.0f;
        pResult.m31 = 0.0f;
        pResult.m32 = 0.0f;
        pResult.m33 = 1.0f;
    }

    /**************************************************************************/
    /*!
        Translate Function (Column Major)
        This function creates a translation matrix from x, y & z
        and saves it in pResult
     */
     /**************************************************************************/
    void Mtx44TranslateCM(Matrix4x4& pResult, float x, float y, float z)
    {
        pResult.m00 = 1.0f;
        pResult.m01 = 0.0f;
        pResult.m02 = 0.0f;
        pResult.m03 = x;
        pResult.m10 = 0.0f;
        pResult.m11 = 1.0f;
        pResult.m12 = 0.0f;
        pResult.m13 = y;
        pResult.m20 = 0.0f;
        pResult.m21 = 0.0f;
        pResult.m22 = 1.0f;
        pResult.m23 = z;
        pResult.m30 = 0.0f;
        pResult.m31 = 0.0f;
        pResult.m32 = 0.0f;
        pResult.m33 = 1.0f;
    }

    /**************************************************************************/
    /*!
        Translate Function (Row Major)
        This function creates a translation matrix from x, y & z
        and saves it in pResult
     */
     /**************************************************************************/
    void Mtx44TranslateRM(Matrix4x4& pResult, float x, float y, float z)
    {
        pResult.m00 = 1.0f;
        pResult.m01 = 0.0f;
        pResult.m02 = 0.0f;
        pResult.m03 = 0.0f;
        pResult.m10 = 0.0f;
        pResult.m11 = 1.0f;
        pResult.m12 = 0.0f;
        pResult.m13 = 0.0f;
        pResult.m20 = 0.0f;
        pResult.m21 = 0.0f;
        pResult.m22 = 1.0f;
        pResult.m23 = 0.0f;
        pResult.m30 = x;
        pResult.m31 = y;
        pResult.m32 = z;
        pResult.m33 = 1.0f;
    }


    /**************************************************************************/
    /*!
        Scale Function
        This function creates a scaling matrix from x, y & z
        and saves it in pResult
     */
     /**************************************************************************/
    void Mtx44Scale(Matrix4x4& pResult, float x, float y, float z)
    {
        pResult.m00 = x;
        pResult.m01 = 0.0f;
        pResult.m02 = 0.0f;
        pResult.m03 = 0.0f;
        pResult.m10 = 0.0f;
        pResult.m11 = y;
        pResult.m12 = 0.0f;
        pResult.m13 = 0.0f;
        pResult.m20 = 0.0f;
        pResult.m21 = 0.0f;
        pResult.m22 = z;
        pResult.m23 = 0.0f;
        pResult.m30 = 0.0f;
        pResult.m31 = 0.0f;
        pResult.m32 = 0.0f;
        pResult.m33 = 1.0f;
    }

    /**************************************************************************/
    /*!
        Rotation (Rad) Function
        This matrix creates a rotation matrix from "angle" whose value
        is in radian. Save the resultant matrix in pResult.
     */
     /**************************************************************************/
    void Mtx44RotRad(Matrix4x4& pResult, float angle)
    {
        pResult.m00 = static_cast<float>(cos(angle));
        pResult.m01 = static_cast<float>(-sin(angle));
        pResult.m02 = 0.0f;
        pResult.m03 = 0.0f;
        pResult.m10 = static_cast<float>(sin(angle));
        pResult.m11 = static_cast<float>(cos(angle));
        pResult.m12 = 0.0f;
        pResult.m13 = 0.0f;
        pResult.m20 = 0.0f;
        pResult.m21 = 0.0f;
        pResult.m22 = 1.0f;
        pResult.m23 = 0.0f;
        pResult.m30 = 0.0f;
        pResult.m31 = 0.0f;
        pResult.m32 = 0.0f;
        pResult.m33 = 1.0f;
    }

    /**************************************************************************/
    /*!
        Rotaton (Deg) Function
        This matrix creates a rotation matrix from "angle" whose value
        is in degree. Save the resultant matrix in pResult.
     */
     /**************************************************************************/
    void Mtx44RotDeg(Matrix4x4& pResult, float angle)
    {
        // Convert angle from degree to radian
        angle = (angle * PI) / 180;

        pResult.m00 = static_cast<float>(cos(angle));
        pResult.m01 = static_cast<float>(-sin(angle));
        pResult.m02 = 0.0f;
        pResult.m03 = 0.0f;
        pResult.m10 = static_cast<float>(sin(angle));
        pResult.m11 = static_cast<float>(cos(angle));
        pResult.m12 = 0.0f;
        pResult.m13 = 0.0f;
        pResult.m20 = 0.0f;
        pResult.m21 = 0.0f;
        pResult.m22 = 1.0f;
        pResult.m23 = 0.0f;
        pResult.m30 = 0.0f;
        pResult.m31 = 0.0f;
        pResult.m32 = 0.0f;
        pResult.m33 = 1.0f;
    }

    /**************************************************************************/
    /*!
        Matrix Transpose Function
        This functions calculated the transpose matrix of pMtx
        and saves it in pResult
     */
     /**************************************************************************/
    void Mtx44Transpose(Matrix4x4& pResult, const Matrix4x4& pMtx)
    {
        pResult.m00 = pMtx.m00;
        pResult.m01 = pMtx.m10;
        pResult.m02 = pMtx.m20;
        pResult.m03 = pMtx.m30;
        pResult.m10 = pMtx.m01;
        pResult.m11 = pMtx.m11;
        pResult.m12 = pMtx.m21;
        pResult.m13 = pMtx.m31;
        pResult.m20 = pMtx.m02;
        pResult.m21 = pMtx.m12;
        pResult.m22 = pMtx.m22;
        pResult.m23 = pMtx.m32;
        pResult.m30 = pMtx.m03;
        pResult.m31 = pMtx.m13;
        pResult.m32 = pMtx.m23;
        pResult.m33 = pMtx.m33;
    }

    /**************************************************************************/
    /*!
        Matrix Inverse Function
        This function calculates the inverse matrix of pMtx and saves the
        result in pResult. If the matrix inversion fails, pResult
        would be set to NULL.
    */
    /**************************************************************************/
    void Mtx44Inverse(Matrix4x4* pResult, float* determinant,
        const Matrix4x4& pMtx)
    {
        *determinant = pMtx.m00 * pMtx.m11 * pMtx.m22
            * pMtx.m33 + pMtx.m00 * pMtx.m12
            * pMtx.m23 * pMtx.m31 + pMtx.m00
            * pMtx.m13 * pMtx.m21 * pMtx.m32
            - pMtx.m00 * pMtx.m13 * pMtx.m22
            * pMtx.m31 - pMtx.m00 * pMtx.m12
            * pMtx.m21 * pMtx.m33 - pMtx.m00
            * pMtx.m11 * pMtx.m23 * pMtx.m32
            - pMtx.m01 * pMtx.m10 * pMtx.m22
            * pMtx.m33 - pMtx.m02 * pMtx.m10
            * pMtx.m23 * pMtx.m31 - pMtx.m03
            * pMtx.m10 * pMtx.m21 * pMtx.m32
            + pMtx.m03 * pMtx.m10 * pMtx.m22
            * pMtx.m31 + pMtx.m02 * pMtx.m10
            * pMtx.m21 * pMtx.m33 + pMtx.m01
            * pMtx.m10 * pMtx.m23 * pMtx.m32
            + pMtx.m01 * pMtx.m12 * pMtx.m20
            * pMtx.m33 + pMtx.m02 * pMtx.m13
            * pMtx.m20 * pMtx.m31 + pMtx.m03
            * pMtx.m11 * pMtx.m20 * pMtx.m32
            - pMtx.m03 * pMtx.m12 * pMtx.m20
            * pMtx.m31 - pMtx.m02 * pMtx.m11
            * pMtx.m20 * pMtx.m33 - pMtx.m01
            * pMtx.m13 * pMtx.m20 * pMtx.m32
            - pMtx.m01 * pMtx.m12 * pMtx.m23
            * pMtx.m30 - pMtx.m02 * pMtx.m13
            * pMtx.m21 * pMtx.m30 - pMtx.m03
            * pMtx.m11 * pMtx.m22 * pMtx.m30
            + pMtx.m03 * pMtx.m12 * pMtx.m21
            * pMtx.m30 + pMtx.m02 * pMtx.m11
            * pMtx.m23 * pMtx.m30 + pMtx.m01
            * pMtx.m13 * pMtx.m22 * pMtx.m30;

        Matrix4x4 temp;

        if (*determinant <= 0)
        {
            pResult = nullptr;
        }
        else
        {
            Mtx44Transpose(temp, pMtx);

            // Row 1
            pResult->m00 = (temp.m11 * temp.m22 * temp.m33
                + temp.m12 * temp.m23 * temp.m31
                + temp.m13 * temp.m21 * temp.m32
                - temp.m13 * temp.m22 * temp.m31
                - temp.m12 * temp.m21 * temp.m33
                - temp.m11 * temp.m23 * temp.m32) / *determinant;

            pResult->m01 = (-temp.m01 * temp.m22 * temp.m33
                - temp.m02 * temp.m33 * temp.m31
                - temp.m03 * temp.m21 * temp.m32
                + temp.m03 * temp.m22 * temp.m31
                + temp.m02 * temp.m21 * temp.m33
                + temp.m01 * temp.m23 * temp.m32) / *determinant;

            pResult->m02 = (temp.m11 * temp.m22 * temp.m33
                + temp.m12 * temp.m23 * temp.m31
                + temp.m13 * temp.m21 * temp.m32
                - temp.m13 * temp.m22 * temp.m31
                - temp.m12 * temp.m21 * temp.m33
                - temp.m11 * temp.m23 * temp.m32) / *determinant;

            pResult->m03 = (temp.m01 * temp.m12 * temp.m23
                + temp.m02 * temp.m13 * temp.m21
                + temp.m03 * temp.m11 * temp.m22
                - temp.m03 * temp.m12 * temp.m21
                - temp.m02 * temp.m11 * temp.m23
                - temp.m01 * temp.m13 * temp.m22) / *determinant;

            // Row 2
            pResult->m10 = (-temp.m10 * temp.m22 * temp.m33
                - temp.m12 * temp.m23 * temp.m30
                - temp.m13 * temp.m20 * temp.m32
                + temp.m13 * temp.m22 * temp.m30
                + temp.m12 * temp.m20 * temp.m33
                + temp.m10 * temp.m23 * temp.m32) / *determinant;


            pResult->m11 = (temp.m00 * temp.m22 * temp.m33
                + temp.m02 * temp.m23 * temp.m30
                + temp.m03 * temp.m20 * temp.m32
                - temp.m03 * temp.m22 * temp.m30
                - temp.m02 * temp.m20 * temp.m33
                - temp.m00 * temp.m23 * temp.m32) / *determinant;

            pResult->m12 = (-temp.m00 * temp.m12 * temp.m33
                - temp.m02 * temp.m13 * temp.m30
                - temp.m03 * temp.m10 * temp.m32
                + temp.m03 * temp.m12 * temp.m30
                + temp.m02 * temp.m10 * temp.m33
                + temp.m00 * temp.m13 * temp.m32) / *determinant;

            pResult->m13 = (temp.m00 * temp.m12 * temp.m23
                + temp.m02 * temp.m13 * temp.m20
                + temp.m13 * temp.m21 * temp.m32
                - temp.m03 * temp.m12 * temp.m20
                - temp.m02 * temp.m10 * temp.m23
                - temp.m00 * temp.m13 * temp.m22) / *determinant;

            // Row 3
            pResult->m20 = (temp.m10 * temp.m21 * temp.m33
                + temp.m11 * temp.m23 * temp.m30
                + temp.m13 * temp.m20 * temp.m31
                - temp.m13 * temp.m21 * temp.m30
                - temp.m11 * temp.m20 * temp.m33
                - temp.m10 * temp.m23 * temp.m31) / *determinant;

            pResult->m21 = (-temp.m00 * temp.m21 * temp.m33
                - temp.m01 * temp.m23 * temp.m30
                - temp.m13 * temp.m20 * temp.m31
                + temp.m13 * temp.m21 * temp.m30
                + temp.m11 * temp.m20 * temp.m33
                + temp.m10 * temp.m23 * temp.m31) / *determinant;

            pResult->m22 = (temp.m00 * temp.m11 * temp.m33
                + temp.m01 * temp.m10 * temp.m30
                + temp.m03 * temp.m10 * temp.m31
                - temp.m03 * temp.m11 * temp.m30
                - temp.m01 * temp.m10 * temp.m33
                - temp.m00 * temp.m13 * temp.m31) / *determinant;

            pResult->m23 = (-temp.m00 * temp.m11 * temp.m23
                - temp.m01 * temp.m13 * temp.m20
                - temp.m03 * temp.m10 * temp.m21
                + temp.m03 * temp.m11 * temp.m20
                + temp.m01 * temp.m10 * temp.m23
                + temp.m00 * temp.m13 * temp.m21) / *determinant;

            // Row 4
            pResult->m30 = (-temp.m10 * temp.m21 * temp.m32
                - temp.m11 * temp.m22 * temp.m30
                - temp.m03 * temp.m20 * temp.m31
                + temp.m12 * temp.m21 * temp.m30
                + temp.m32 * temp.m20 * temp.m32
                + temp.m10 * temp.m22 * temp.m31) / *determinant;

            pResult->m31 = (temp.m00 * temp.m21 * temp.m32
                + temp.m01 * temp.m22 * temp.m30
                + temp.m02 * temp.m20 * temp.m31
                - temp.m02 * temp.m21 * temp.m30
                - temp.m01 * temp.m20 * temp.m32
                - temp.m00 * temp.m22 * temp.m31) / *determinant;

            pResult->m32 = (-temp.m00 * temp.m11 * temp.m32
                - temp.m01 * temp.m12 * temp.m30
                - temp.m02 * temp.m10 * temp.m31
                + temp.m02 * temp.m11 * temp.m30
                + temp.m01 * temp.m10 * temp.m32
                + temp.m00 * temp.m12 * temp.m31) / *determinant;

            pResult->m33 = (temp.m00 * temp.m11 * temp.m22
                + temp.m01 * temp.m12 * temp.m20
                + temp.m02 * temp.m10 * temp.m21
                - temp.m02 * temp.m11 * temp.m20
                - temp.m01 * temp.m10 * temp.m22
                - temp.m00 * temp.m12 * temp.m21) / *determinant;
        }
    }

}