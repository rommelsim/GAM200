/*****************************************************************//**
 * \file   PathFinding.h
 * \brief  WayPoint System which manages all the AStar Pathfinding
 *
 * \author Yi Cong
 * \date   November 2022
 * Copyright (C) 2022 DigiPen Institute of Technology.
 * Reproduction or disclosure of this file or its contents without the
 * prior written consent of DigiPen Institute of Technology is prohibited.
 *********************************************************************/
#pragma once
 //#include <Systems/ObjectManager.h>
#include "SEFactory.h"
#include <map>
#include <vector>

namespace PathFinding
{
	// Node Class
	class Node
	{
	public:
		Node() = default;
		Node(GameObject* _gameObject) : gameObject(_gameObject), actualCost(0), totalCost(0) {}
		GameObject* gameObject;
		std::vector<std::string> NeighbourNodes;
		void addNeighbourNode(std::string neighbourNode);
		float actualCost;
		float totalCost;
	};

	class WayPointSystem;

	// AStar path finding class
	class AStar
	{
	public:
		static std::vector<Node> getPath(SEVec2 AI, GameObject* target);
		static GameObject* gameObject;

	private:
		static std::map<std::string, Node> OpenList;
		static std::map<std::string, Node> ClosedList;
		static std::vector<Node> ClosedListVector;
	};

	// One WayPointSystem, many nodes and pathfinders
	class WayPointSystem
	{
	public:
		static void Init();
		static std::map<std::string, Node>& getNodes();
		static void addWayPoint(GameObject* waypoint);	// add new waypoint
		static void addNeighbourNode(std::string node, std::string neighbour);
		static std::vector<std::string> getNeighbourNodes(std::string node);
		static std::vector<Node> getPath(SEVec2 AIPosition, GameObject* target);
		static void removeWayPoint(std::string waypointName);

	private:
		static std::map<std::string, Node> WayPoints;
	};
}