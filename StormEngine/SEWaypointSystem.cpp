/******************************************************************************//**
  * \file			SEWaypointSystem.cpp
  * \author 		Lee Yi Cong (100%)
  * \par    		yicong.lee@digipen.edu

  *
  * \brief			A-Star Pathfinding
  *
  * \date			January 2023

 Copyright (C) 2023 DigiPen Institute of Technology.
 Reproduction or disclosure of this file or its contents without the
 prior written consent of DigiPen Institute of Technology is prohibited.
 ******************************************************************************/
#include "pch.h"
#include "SEWaypointSystem.h"

std::map<std::string, PathFinding::Node> PathFinding::WayPointSystem::WayPoints;

std::map<std::string, PathFinding::Node> PathFinding::AStar::OpenList;
std::map<std::string, PathFinding::Node> PathFinding::AStar::ClosedList;
std::vector<PathFinding::Node> PathFinding::AStar::ClosedListVector;

namespace PathFinding
{
	// Node

	// Adds a neighbour node to to a vector in the node
	void Node::addNeighbourNode(std::string neighbourNode)
	{
		NeighbourNodes.push_back(neighbourNode);
	}

	// AStar

	// AStar path finding
	std::vector<Node> AStar::getPath(SEVec2 AIPosition, GameObject* target)
	{
		OpenList.clear();
		ClosedList.clear();
		ClosedListVector.clear();
		if (target)
		{
			Transform* targetPosition = (Transform*)(target->getComponent(COMPONENT_TYPE::CT_TRANSFORM));

			Node startNode(NULL);
			Node endNode(NULL);
			float startLength = 0;
			float endLength = 0;
			std::map<std::string, Node> waypoints = WayPointSystem::getNodes();
			if (targetPosition)
			{
				// For each node that exists in the waypoint system
				for (std::pair <const std::string, Node> node : waypoints)
				{
					Transform* nodePosition = (Transform*)(node.second.gameObject->getComponent(COMPONENT_TYPE::CT_TRANSFORM));
					if (nodePosition)
					{
						// Get closest node to the AI
						SEVec2 distVec = nodePosition->getTranslate() - AIPosition;
						float sqr_len = (distVec.x * distVec.x) + (distVec.y * distVec.y);
						if (startNode.gameObject == NULL)
						{
							startNode = node.second;
							startLength = sqr_len;
						}
						else if (sqr_len < startLength)
						{
							startNode = node.second;
							startLength = sqr_len;
						}

						// Get the closest node to the Target
						distVec = nodePosition->getTranslate() - targetPosition->getTranslate();
						sqr_len = (distVec.x * distVec.x) + (distVec.y * distVec.y);
						if (endNode.gameObject == NULL)
						{
							endNode = node.second;
							endLength = sqr_len;
						}
						else if (sqr_len < endLength)
						{
							endNode = node.second;
							endLength = sqr_len;
						}
					}
				}
			}

			// If all the nodes are deleted
			if (endNode.gameObject == NULL)
			{
				return ClosedListVector;
			}

			Transform* endPosition = (Transform*)(endNode.gameObject->getComponent(COMPONENT_TYPE::CT_TRANSFORM));
			Transform* startPosition = (Transform*)(startNode.gameObject->getComponent(COMPONENT_TYPE::CT_TRANSFORM));
			SEVec2 distVec = endPosition->getTranslate() - startPosition->getTranslate();
			float sqr_len = (distVec.x * distVec.x) + (distVec.y * distVec.y);

			// Total cost of first node
			startNode.totalCost = sqr_len;
			OpenList.emplace(startNode.gameObject->getName(), startNode);

			// While there are still neighbouring nodes not already in closed list
			while (!OpenList.empty())
			{
				Node currentNode(NULL);
				float cheapestCost = 0;
				std::string cheapestName;

				// Pop the cheapest cost node from the open list into the closed list
				for (std::pair<const std::string, Node>& node : OpenList)
				{
					if (cheapestCost == 0)
					{
						cheapestCost = node.second.totalCost;
						currentNode = node.second;
						cheapestName = node.second.gameObject->getName();
					}
					else if (node.second.totalCost < cheapestCost)
					{
						cheapestCost = node.second.totalCost;
						currentNode = node.second;
						cheapestName = node.second.gameObject->getName();
					}
				}
				//OpenList.erase(cheapestName);
				OpenList.clear(); // no going back
				// To find a specific node in a map
				ClosedList.emplace(currentNode.gameObject->getName(), currentNode);
				// To return to the AI a vector that has the nodes in insertion order
				ClosedListVector.push_back(currentNode);

				// Reached target node!
				if (currentNode.gameObject->getName() == endNode.gameObject->getName())
				{
					return ClosedListVector;
				}

				// For all the neighbouring nodes of the current node
				for (std::string neighbourName : currentNode.NeighbourNodes)
				{
					// To get the neighbour node from the map of nodes in the WayPoint System
					// Since putting the neighbouring nodes in a vector in the Node class won't
					// ensure that the neighbour nodes have neighbour nodes assigned to them
					Node neighbour = WayPointSystem::getNodes()[neighbourName];
					// If neighbour node is not in openlist and not in closed list
					// Calculate Total Cost and add neighbour node to open list
					if (OpenList.find(neighbour.gameObject->getName()) == OpenList.end() && ClosedList.find(neighbour.gameObject->getName()) == ClosedList.end())
					{
						Transform* neighbourPosition = (Transform*)(neighbour.gameObject->getComponent(COMPONENT_TYPE::CT_TRANSFORM));
						Transform* currentPosition = (Transform*)(currentNode.gameObject->getComponent(COMPONENT_TYPE::CT_TRANSFORM));
						distVec = neighbourPosition->getTranslate() - currentPosition->getTranslate();
						sqr_len = (distVec.x * distVec.x) + (distVec.y * distVec.y);
						neighbour.actualCost = currentNode.actualCost + sqr_len;

						distVec = endPosition->getTranslate() - neighbourPosition->getTranslate();
						sqr_len = (distVec.x * distVec.x) + (distVec.y * distVec.y);
						neighbour.totalCost = neighbour.actualCost + sqr_len;

						OpenList.emplace(neighbour.gameObject->getName(), neighbour);
					}
					// If neighbour node already exists in open list but has higher cost, 
					// replace it with new neighbour node
					else if (OpenList.find(neighbour.gameObject->getName()) != OpenList.end())
					{
						Transform* neighbourPosition = (Transform*)(neighbour.gameObject->getComponent(COMPONENT_TYPE::CT_TRANSFORM));
						Transform* currentPosition = (Transform*)(currentNode.gameObject->getComponent(COMPONENT_TYPE::CT_TRANSFORM));
						distVec = neighbourPosition->getTranslate() - currentPosition->getTranslate();
						sqr_len = (distVec.x * distVec.x) + (distVec.y * distVec.y);
						neighbour.actualCost = currentNode.actualCost + sqr_len;

						distVec = endPosition->getTranslate() - neighbourPosition->getTranslate();
						sqr_len = (distVec.x * distVec.x) + (distVec.y * distVec.y);
						neighbour.totalCost = neighbour.actualCost + sqr_len;

						if (OpenList[neighbour.gameObject->getName()].totalCost > neighbour.totalCost)
							OpenList[neighbour.gameObject->getName()] = neighbour;
					}
				}
			}
			/*std::vector<Node> nopath;
			return nopath;*/
		}
		// If no paths found, return empty vector
		std::vector<Node> nopath;
		return nopath;
	}

	//WayPointSystem

	// Init Waypoints
	void WayPointSystem::Init()
	{
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint0"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint1"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint2"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint3"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint4"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint5"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint6"));

		PathFinding::WayPointSystem::addNeighbourNode("Waypoint0", "Waypoint1");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint1", "Waypoint0");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint1", "Waypoint2");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint2", "Waypoint1");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint2", "Waypoint3");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint3", "Waypoint2");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint3", "Waypoint4");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint3", "Waypoint6");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint4", "Waypoint3");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint4", "Waypoint5");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint5", "Waypoint4");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint5", "Waypoint6");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint6", "Waypoint3");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint6", "Waypoint5");

		// Moose Waypoints
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint7"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint8"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint9"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint10"));

		PathFinding::WayPointSystem::addNeighbourNode("Waypoint7", "Waypoint10");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint7", "Waypoint8");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint8", "Waypoint7");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint8", "Waypoint9");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint9", "Waypoint8");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint9", "Waypoint10");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint10", "Waypoint7");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint10", "Waypoint9");

		// Lala Waypoints
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint11"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint12"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint13"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint14"));
		PathFinding::WayPointSystem::addWayPoint(SEFindObjectByName("Waypoint15"));

		PathFinding::WayPointSystem::addNeighbourNode("Waypoint11", "Waypoint12");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint12", "Waypoint11");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint13", "Waypoint14");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint14", "Waypoint13");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint14", "Waypoint15");
		PathFinding::WayPointSystem::addNeighbourNode("Waypoint15", "Waypoint14");


#if _DEBUG
		for (auto waypoint : WayPoints)
		{
			Texture* tex = (Texture*)waypoint.second.gameObject->getComponent(CT_TEXTURE);
			tex->isRendering = true;
		}
#endif
	}

	// Get nodes existing in the WayPoint System
	std::map<std::string, Node>& WayPointSystem::getNodes()
	{
		return WayPoints;
	}

	// Add new WayPoint
	void WayPointSystem::addWayPoint(GameObject* gameObject)
	{
		Node newWayPoint(gameObject);
		WayPoints.emplace(gameObject->getName(), newWayPoint);
	}

	// Adds a neighbouring node to an existing node
	void WayPointSystem::addNeighbourNode(std::string node, std::string neighbour)
	{
		WayPoints[node].NeighbourNodes.push_back(neighbour);
	}

	// Get neighbouring nodes for serialisation
	std::vector<std::string> WayPointSystem::getNeighbourNodes(std::string node)
	{
		return WayPoints[node].NeighbourNodes;
	}

	// Calculates shortest path to target node
	std::vector<Node> WayPointSystem::getPath(SEVec2 AIPosition, GameObject* target)
	{
		return AStar::getPath(AIPosition, target);
		//return PathFinders.at(AIName).getPath(target);
	}

	// When a game object gets deleted
	void WayPointSystem::removeWayPoint(std::string waypointName)
	{
		// Check if waypoint exists
		if (WayPoints.find(waypointName) != WayPoints.end())
		{
			// Remove neighbouring node from all existing nodes
			for (std::pair<const std::string, Node>& waypoint : WayPoints)
			{
				for (int i = 0; i < waypoint.second.NeighbourNodes.size(); i++)
				{
					if (waypoint.second.NeighbourNodes[i] == waypointName)
						waypoint.second.NeighbourNodes.erase(waypoint.second.NeighbourNodes.begin() + i);
				}
			}
			// Remove waypoint from WayPoint System
			WayPoints.erase(waypointName);
		}
	}
}