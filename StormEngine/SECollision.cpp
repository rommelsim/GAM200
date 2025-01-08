/******************************************************************************//**
 * \file			SECollision.cpp
 * \author 			Loke Kheng Ray
 * \co-author
 * \contributions	Loke Kheng Ray			(100%)
 * \par    	email: l.khengray@digipen.edu
 *
 * \brief	Contains function definitions to facilitate calculations
 *			between two given objects
 *
 * \date   	January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/
#pragma once
#include "pch.h"
#include "SECollision.h"

namespace StormEngine
{

	//void UpdateCollision()
	//{
	//	// Looping thru object array for making temporary testing
	//	for (auto& objInGame : Factory::sceneObjectsArray)
	//	{

	//	}
	//}

	AABB GenerateBoundingBox(glm::vec2 points)
	{
		AABB newBoundBox;
		newBoundBox.min.x = points.x - 5.f;
		newBoundBox.min.y = points.y - 5.f;

		newBoundBox.max.x = points.x + 5.f;
		newBoundBox.max.y = points.y + 5.f;

		return newBoundBox;
	}

	bool Collision_Rect(const AABB& aabb1, const AABB& aabb2)
	{
		if (!(aabb1.max.x < aabb2.min.x ||	// if obj2 left intersect with obj1 right
			aabb1.min.x > aabb2.max.x ||	// if obj1 left intersect with obj2 right
			aabb1.max.y < aabb2.min.y ||	// if obj1 top intersect with obj2 btm
			aabb1.min.y > aabb2.max.y))		// if obj1 btm intersect with obj2 top
		{
			return true;
		}

		return false;
	}

	CollisionDir CollisionRectDir(const AABB& aabb1, const AABB& aabb2) {
		if (aabb1.min.x <= aabb2.max.x && ((aabb1.max.y > aabb2.min.y) || (aabb1.min.y < aabb2.max.y))) {
#ifdef _DEBUG
			std::cout << "COLLISION LEFT!" << std::endl;
#endif // _DEBUG
			return CollisionDir::CollideLeft;
		}

		if (aabb1.max.x >= aabb2.min.x && ((aabb1.max.y > aabb2.min.y) || (aabb1.min.y < aabb2.max.y))) {
#ifdef _DEBUG
			std::cout << "COLLISION RIGHT!" << std::endl;
#endif // _DEBUG
			return CollisionDir::CollideRight;
		}

		if (aabb1.max.y >= aabb2.min.y && ((aabb1.max.x > aabb2.min.x) || (aabb1.min.x < aabb2.max.x))) {
#ifdef _DEBUG
			std::cout << "COLLISION TOP!" << std::endl;
#endif // _DEBUG
			return CollisionDir::CollideTop;
		}

		if (aabb1.min.y <= aabb2.max.y && ((aabb1.max.x > aabb2.min.x) || (aabb1.min.x < aabb2.max.x))) {
#ifdef _DEBUG
			std::cout << "COLLISION BOTTOM!" << std::endl;
#endif // _DEBUG
			return CollisionDir::CollideBottom;
		}
		return CollisionDir::CollideNull;




	}


	bool Collision_Rect_wVelocity(const AABB& aabb1, const Vec2& vel1,
		const AABB& aabb2, const Vec2& vel2)
	{

		if (Collision_Rect(aabb1, aabb2))
			return true;


		// Variables for calculating collision
		float VRel_x = vel2.x - vel1.x;
		float VRel_y = vel2.y - vel1.y;

		float dFirst_x = aabb1.min.x - aabb2.max.x;
		float dLast_x = aabb1.max.x - aabb2.min.x;

		float dFirst_y = aabb1.min.y - aabb2.max.y;
		float dLast_y = aabb1.max.y - aabb2.min.y;

		float tFirst = 0;
		float tLast = 0;		// Check (?)
		//float tLast = (float)StormHelper::delta_time;		// Check (?)


		// Step 3: Working with one dimension
		if (VRel_x < 0)	// Solving x-axis first
		{
			// Case 1
			if (aabb1.min.x > aabb2.max.x)
			{
				printf("NEW CASE 2-a ENTERED \n");
				return false;
			}

			// Case 4
			if (aabb1.max.x < aabb2.min.x)
			{
				printf("NEW CASE 2-b ENTERED \n");
				tFirst = max(dFirst_x / VRel_x, tFirst);
			}

			if (aabb1.min.x < aabb2.max.x)
			{
				printf("NEW CASE 2-c ENTERED \n");
				tLast = min(dLast_x / VRel_x, tLast);		// Check (?)

			}

			if (VRel_x > 0)
			{
				// Case 2
				if (aabb1.min.x > aabb2.max.x)
				{
					printf("NEW CASE 3-a ENTERED \n");
					tFirst = max((dFirst_x / VRel_x), tFirst);
				}

				if (aabb1.max.x > aabb2.min.x)
				{
					printf("NEW CASE 3-b ENTERED \n");
					tLast = min(dLast_x / VRel_x, tLast);		// Check (?)
				}

				//Case 3 
				if (aabb1.max.x < aabb2.min.x)
				{
					printf("NEW CASE 3-c ENTERED \n");
					return false;
				}
			}

			// Case 5
			if (tFirst > tLast)						// Check (?)
			{
				printf("NEW CASE 4 ENTERED \n");
				return false;
			}

			// Step 3: Working with one dimension(y - axis)
			if (VRel_y < 0)	// Solving x-axis first
			{
				// Case 1
				if (aabb1.min.y > aabb2.max.y)
				{
					printf("NEW CASE 5-a ENTERED \n");
					return false;
				}


				// Case 4
				if (aabb1.max.y < aabb2.min.y)
				{
					printf("NEW CASE 5-b ENTERED \n");
					tFirst = max(dFirst_y / VRel_y, tFirst);
				}

				if (aabb1.min.y < aabb2.max.y)
				{
					printf("NEW CASE 5-c ENTERED \n");
					tLast = min(dLast_y / VRel_y, tLast);		// Check (?)
				}

			}

			if (VRel_y > 0)
			{
				// Case 2
				if (aabb1.min.y > aabb2.max.y)
				{
					printf("NEW CASE 6-a ENTERED \n");
					tFirst = max((dFirst_y / VRel_y), tFirst);
				}

				if (aabb1.max.y > aabb2.min.y)
				{
					printf("NEW CASE 6-b ENTERED \n");
					tLast = min(dLast_y / VRel_y, tLast);		// Check (?)
				}

				//Case 3 
				if (aabb1.max.y < aabb2.min.y)
				{
					printf("NEW CASE 6-c ENTERED \n");
					return false;
				}
			}

			// Case 5
			if (tFirst > tLast)			// TODO : Uncomment after getting dt
			{
				printf("NEW CASE 7 ENTERED \n");
				return false;
			}

		}
		//Step 5: Otherwise the rectangles intersect
		printf("NEW CASE DEFAULT ENTERED \n");
		return true;
	}


	int CollisionIntersection_CircleLineSegment(const Circle& circle,
		const Vec2& ptEnd,
		const LineSegment& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime,
		bool& checkLineEdges)
	{
		//UNREFERENCED_PARAMETER(checkLineEdges);
		(void)checkLineEdges;

		float circle_normal = Vector2DDotProduct(lineSeg.m_normal, circle.m_center);			// Check (?)
		float lineP0_normal = Vector2DDotProduct(lineSeg.m_normal, lineSeg.m_pt0);				// Check (?)

		if ((circle_normal - lineP0_normal) <= circle.m_radius)
		{
			Vec2 velocity = ptEnd - circle.m_center;
			Vec2 vel_norm{ velocity.y, -velocity.x };

			LineSegment point_Prime;
			point_Prime.m_pt0 = lineSeg.m_pt0 - circle.m_radius * lineSeg.m_normal;
			point_Prime.m_pt1 = lineSeg.m_pt1 - circle.m_radius * lineSeg.m_normal;



			if ((Vector2DDotProduct(vel_norm, point_Prime.m_pt0 - circle.m_center) *				// Check (?)
				Vector2DDotProduct(vel_norm, point_Prime.m_pt1 - circle.m_center)) < 0.f)			// Check (?)
			{
				interTime = (lineP0_normal - circle_normal - circle.m_radius) /
					(Vector2DDotProduct(lineSeg.m_normal, velocity));

				if (interTime >= 0.f && interTime <= 1.f)
				{
					interPt = circle.m_center + velocity * interTime;
					normalAtCollision = -lineSeg.m_normal;

					return 1; // collision

				}
			}
			else
			{
				return CheckMovingCircleToLineEdge(false, circle, ptEnd, lineSeg,
					interPt, normalAtCollision, interTime); //no collision
			}
		}

		// case 2
		else if ((circle_normal - lineP0_normal) >= circle.m_radius)
		{
			Vec2 velocity = ptEnd - circle.m_center;
			Vec2 vel_norm{ velocity.y, -velocity.x };

			LineSegment point_Prime;
			point_Prime.m_pt0 = lineSeg.m_pt0 + circle.m_radius * lineSeg.m_normal;
			point_Prime.m_pt1 = lineSeg.m_pt1 + circle.m_radius * lineSeg.m_normal;



			if ((Vector2DDotProduct(vel_norm, point_Prime.m_pt0 - circle.m_center) *				// TODO : Check (?)
				Vector2DDotProduct(vel_norm, point_Prime.m_pt1 - circle.m_center)) < 0.f)			// TODO : Check (?)
			{
				interTime = (lineP0_normal - circle_normal + circle.m_radius) /
					(Vector2DDotProduct(lineSeg.m_normal, velocity));

				if (interTime >= 0.f && interTime <= 1.f)
				{
					interPt = circle.m_center + velocity * interTime;
					normalAtCollision = lineSeg.m_normal;

					return 1; // collision
				}
			}
			else
			{
				return CheckMovingCircleToLineEdge(false, circle, ptEnd, lineSeg,
					interPt, normalAtCollision, interTime);
			}
		}

		return CheckMovingCircleToLineEdge(true, circle, ptEnd, lineSeg,
			interPt, normalAtCollision, interTime); // no intersection
	}


	int CheckMovingCircleToLineEdge(bool withinBothLines,
		const Circle& circle,
		const Vec2& ptEnd,
		const LineSegment& lineSeg,
		Vec2& interPt,
		Vec2& normalAtCollision,
		float& interTime)
	{


		Vec2 velocity = ptEnd - circle.m_center;	//V
		Vec2 vel_norm{ velocity.y, -velocity.x };	//M

		Vec2 BsP0 = lineSeg.m_pt0 - circle.m_center;
		Vec2 BsP1 = lineSeg.m_pt1 - circle.m_center;

		float m0 = Vector2DDotProduct(BsP0, velocity);		// Check (?)
		float m1 = Vector2DDotProduct(BsP1, velocity);		// Check (?)

		if (withinBothLines)
		{
			Vec2 P0P1 = lineSeg.m_pt0 - lineSeg.m_pt1;


			if (Vector2DDotProduct(BsP0, P0P1) > 0)					// Check (?)
			{
				//float m = Vector2DDotProduct(BsP0, velocity);
				if (m0 > 0)
				{
					//M is normalized outward normal of V

					float dist0 = Vector2DDotProduct(BsP0, vel_norm);
					if (abs(dist0) > circle.m_radius)
					{
						return 0;	// no collision
					}

					float s = sqrt(circle.m_radius * circle.m_radius - dist0 * dist0);
					interTime = (m0 - s) / Vector2DLength(velocity);

					if (interTime <= 1)
					{
						interPt = circle.m_center + velocity * interTime;

						//Normal of reflection is P0Bi normalized
						Vec2 P0Bi = interPt - lineSeg.m_pt0;
						Vector2DNormalize(normalAtCollision, P0Bi);

						return 1;
					}
				}
			}
			else if (Vector2DDotProduct(BsP1, P0P1) > 0)
			{

				if (m1 > 0)
				{
					float dist1 = Vector2DDotProduct(BsP1, vel_norm);
					if (abs(dist1) > circle.m_radius)
					{
						return 0;	// no collision
					}

					float s = sqrt(circle.m_radius * circle.m_radius - dist1 * dist1);
					interTime = (m1 - s) / Vector2DLength(velocity);

					if (interTime <= 1)
					{
						interPt = circle.m_center + velocity * interTime;
						Vec2 P1Bi = interPt - lineSeg.m_pt1;
						Vector2DNormalize(normalAtCollision, P1Bi);

						return 1;
					}
				}
			}
		}
		else
		{

			bool P0Side = false;
			float dist0 = Vector2DDotProduct(BsP0, vel_norm);		// Check (?)
			float dist1 = Vector2DDotProduct(BsP1, vel_norm);		// Check (?)

			float dist0_absVal = abs(dist0);
			float dist1_absVal = abs(dist1);

			if ((dist0_absVal > circle.m_radius) && (dist1_absVal > circle.m_radius))
			{
				return 0;
			}
			else if ((dist0_absVal <= circle.m_radius) && (dist1_absVal <= circle.m_radius))
			{

				float m0_absVal = abs(m0);
				float m1_absVal = abs(m1);

				if (m0_absVal < m1_absVal)
					P0Side = true;
				else
					P0Side = false;
			}
			else if (dist0_absVal <= circle.m_radius)
			{
				P0Side = true;
			}
			else	// if (dist1_absVal <= circle.m_radius)
			{
				P0Side = false;
			}

			if (P0Side)	//circle is close to P0
			{
				if (m0 < 0)
				{
					return 0;	// moving away
				}
				else
				{
					//Reaching here means the circle movement is going towards P0
					//The next line assumes the circle at collision time with P0
					float s = sqrt(circle.m_radius * circle.m_radius - dist0 * dist0);
					interTime = (m0 - s) / Vector2DLength(velocity);		// Check (?)
					interTime = 0.0f;
					if (interTime <= 1)
					{
						interPt = circle.m_center + velocity * interTime;
						//Normal of reflection is P0Bi normalized
						Vec2 P0Bi = interPt - lineSeg.m_pt0;
						Vector2DNormalize(normalAtCollision, P0Bi);		// Check (?)

						return 1;
					}
				}
			}
			else	// circle is closer to P1
			{
				if (m1 < 0)
				{
					return 0;	// moving away
				}
				else
				{
					//Reaching here means the circle movement is going towards P1
					//The next line assumes the circle at collision time with P1
					float s = sqrt(circle.m_radius * circle.m_radius - dist1 * dist1);
					interTime = (m1 - s) / Vector2DLength(velocity);		// Check (?)
					interTime = 0.0f;
					if (interTime <= 1)
					{
						interPt = circle.m_center + velocity * interTime;
						//Normal of reflection is P1Bi normalized
						Vec2 P1Bi = interPt - lineSeg.m_pt1;
						Vector2DNormalize(normalAtCollision, P1Bi);		// Check (?)

						return 1;
					}
				}
			}
		}


		return 0;//no collision
	}


	int CollisionIntersection_CircleCircle(const Circle& circleA,
		const Vec2& velA,
		const Circle& circleB,
		const Vec2& velB,
		Vec2& interPtA,
		Vec2& interPtB,
		float& interTime)
	{


		//Assuming circleA is animated, B is stationary
		Vec2 RV_a = velA - velB;

		// using new as static circle
		Circle new_c{ circleB.m_center, circleA.m_radius + circleB.m_radius };

		//float rv_mag = sqrt((RV_a.x * RV_a.x) + (RV_a.y * RV_a.y));
		//Compute RVn
		Vec2 RVn{};
		//CSD1130::Vector2DNormalize(RVn, RV_a);		// TODO : Need Math Lib

		// m = CaCb . RVn
		//float m = Vector2DDotProduct((circleB.m_center - circleA.m_center), RVn);		// TODO : Need Math Lib
		float m = 0.f;

		if (m <= 0.f)
			return 0;

		// test for no collision
		Vec2 CaCb = circleB.m_center - circleA.m_center;

		// CaCb^2											  
		//float n = Vector2DSquareLength(CaCb) - (m * m);		// TODO : Need Math lib
		float n = 0.f;

		if (n > (new_c.m_radius * new_c.m_radius))
		{
			return 0;
		}

		//compute s^2 = Rc^2 - n^2
		//float s = (new_c.m_radius * new_c.m_radius) - (n);
		//interTime = (m - sqrt(s)) / abs(Vector2DLength(RV_a));				// TODO : Need Math Lib
		interTime = 0.f;

		if (0.f < interTime && interTime < 1.f)
		{
			interPtA = circleA.m_center + velA * interTime;
			interPtB = circleB.m_center + velB * interTime;
			return 1;	// collision true
		}


		return 0;
	}




	void CollisionResponse_CircleLineSegment(const Vec2& ptInter,
		const Vec2& normal,
		Vec2& ptEnd,
		Vec2& reflected)
	{

		Vec2 penetration = ptEnd - ptInter;
		reflected = penetration - (2 * Vector2DDotProduct(penetration, normal) * normal);			// Check (?)
		ptEnd = ptInter + reflected;

		Vector2DNormalize(reflected, reflected);													// Check (?)
	}


	void CollisionResponse_CirclePillar(const Vec2& normal,
		const float& interTime,
		const Vec2& ptStart,
		const Vec2& ptInter,
		Vec2& ptEnd,
		Vec2& reflectedVectorNormalized)
	{


		Vec2 m = ptStart - ptInter;
		reflectedVectorNormalized = (2 * Vector2DDotProduct(m, normal)) * normal - m;				// Check (?)

		Vector2DNormalize(reflectedVectorNormalized, reflectedVectorNormalized);				// Check (?)
		ptEnd = ptInter + (Vector2DLength(m) * reflectedVectorNormalized * (1.f - interTime));	// Check (?)

	}

	void CollisionResponse_CircleCircle(Vec2& normal,
		const float interTime,
		Vec2& velA,
		const float& massA,
		Vec2& interPtA,
		Vec2& velB,
		const float& massB,
		Vec2& interPtB,
		Vec2& reflectedVectorA,
		Vec2& ptEndA,
		Vec2& reflectedVectorB,
		Vec2& ptEndB)
	{



		float len_A = Vector2DDotProduct(velA, normal);			// Check (?)
		float len_B = Vector2DDotProduct(velB, normal);			// Check (?)

		reflectedVectorA = velA - (2 * (len_A - len_B) / (massA + massB)) * massB * normal;
		reflectedVectorB = velB + (2 * (len_A - len_B) / (massA + massB)) * massA * normal;

		ptEndA = interPtA + (1 - interTime) * reflectedVectorA;
		ptEndB = interPtB + (1 - interTime) * reflectedVectorB;

	}


}