/******************************************************************************//**
 * \file		SECollision.h
 * \author 		Loke Kheng Ray
 * \co-author
 * \par    		email: l.khengray@digipen.edu
 *
 * \brief		Contains function declarations to facilitate calculations
 *				between two given objects
 * \date   		January 2023

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
******************************************************************************/

#pragma once

#include "pch.h"
#include <Vector2D.h>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace StormEngine
{
	enum class CollisionDir : short {


		CollideTop,
		CollideBottom,
		CollideLeft,
		CollideRight,
		CollideNull
	};

	struct AABB
	{
		//AEVec2	c; // center
		//float  r[2]; // holds half width and half height

		StormEngine::Vec2	min;
		StormEngine::Vec2	max;

		StormEngine::Vec2	scale;
	};

	struct AABBPos
	{
		glm::vec2	pos;
	};

	enum COLLISION_TYPE
	{
		SQUARE,		// aabb collisions
		CIRCLE,		// oob collisions
	};

	//void UpdateCollision();
	AABB GenerateBoundingBox(glm::vec2 points);

	bool Collision_Rect(const AABB& aabb1, const AABB& aabb2);

	CollisionDir CollisionRectDir(const AABB& aabb1, const AABB& aabb2);

	bool Collision_Rect_wVelocity(const AABB& aabb1, const Vec2& vel1,
		const AABB& aabb2, const Vec2& vel2);


	struct LineSegment
	{
		Vec2	m_pt0;
		Vec2	m_pt1;
		Vec2	m_normal;
	};

	struct Circle
	{
		Vec2  m_center;
		float	m_radius;

		// Extra credits
		float   m_mass{ 1.0f };
	};

	struct Ray
	{
		Vec2	m_pt0;
		Vec2	m_dir;
	};


	// INTERSECTION FUNCTIONS
	int CollisionIntersection_CircleLineSegment(const Circle& circle,			//Circle data - input
		const Vec2& ptEnd,													//End circle position - input
		const LineSegment& lineSeg,												//Line segment - input
		Vec2& interPt,														//Intersection point - output
		Vec2& normalAtCollision,												//Normal vector at collision time - output
		float& interTime,														//Intersection time ti - output
		bool& checkLineEdges);													//The last parameter is new - for Extra Credits: true = check collision with line segment edges

	// Extra credits
	int CheckMovingCircleToLineEdge(bool withinBothLines,						//Flag stating that the circle is starting from between 2 imaginary line segments distant +/- Radius respectively - input
		const Circle& circle,													//Circle data - input
		const Vec2& ptEnd,													//End circle position - input
		const LineSegment& lineSeg,												//Line segment - input
		Vec2& interPt,														//Intersection point - output
		Vec2& normalAtCollision,												//Normal vector at collision time - output
		float& interTime);														//Intersection time ti - output


	// circle-circle - same usage for: dynamic circle vs static pillar, and dynamic circle vs dynamic circle
	// In the case of "dynamic circle vs static pillar", velB will be 0.0f
	int CollisionIntersection_CircleCircle(const Circle& circleA,				//CircleA data - input
		const Vec2& velA,														//CircleA velocity - input
		const Circle& circleB,													//CircleB data - input
		const Vec2& velB,														//CircleA velocity - input
		Vec2& interPtA,														//Intersection point of CircleA at collision time - output
		Vec2& interPtB,														//Intersection point of CircleB at collision time - output
		float& interTime);														//intersection time - output




	// RESPONSE FUNCTIONS
	void CollisionResponse_CircleLineSegment(const Vec2& ptInter,				//Intersection position of the circle - input
		const Vec2& normal,													//Normal vector of reflection on collision time - input
		Vec2& ptEnd,															//Final position of the circle after reflection - output
		Vec2& reflected);														//Normalized reflection vector direction - output

	void CollisionResponse_CirclePillar(const Vec2& normal,					//Normal vector of reflection on collision time - input
		const float& interTime,													//Intersection time - input
		const Vec2& ptStart,													//Starting position of the circle - input
		const Vec2& ptInter,													//Intersection position of the circle - input
		Vec2& ptEnd,															//Final position of the circle after reflection - output
		Vec2& reflectedVectorNormalized);										//Normalized reflection vector - output

	// Extra credits
	void CollisionResponse_CircleCircle(Vec2& normal,							//Normal vector of reflection on collision time - input
		const float interTime,													//Intersection time - input
		Vec2& velA,															//Velocity of CircleA - input
		const float& massA,														//Mass of CircleA - input
		Vec2& interPtA,														//Intersection position of circle A at collision time - input
		Vec2& velB,															//Velocity of CircleB - input
		const float& massB,														//Mass of CircleB - input
		Vec2& interPtB,														//Intersection position of circle B at collision time - input
		Vec2& reflectedVectorA,												//Non-Normalized reflected vector of Circle A - output
		Vec2& ptEndA,															//Final position of the circle A after reflection - output
		Vec2& reflectedVectorB,												//Non-Normalized reflected vector of Circle B - output
		Vec2& ptEndB);														//Final position of the circle B after reflection - output
}