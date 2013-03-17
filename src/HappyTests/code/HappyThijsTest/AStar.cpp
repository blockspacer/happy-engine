//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author: Thijs Morlion
//Created: 16/03/2013
#include "HappyThijsTestPCH.h" 

#include "AStar.h"

namespace ht {

AStar::AStar()
	:m_Grid(nullptr)
{
}


AStar::~AStar()
{
}

void AStar::init()
{
	m_Grid = NEW ht::Grid(10, 10);
	m_Grid->init();

	SetStart(1,1);
	SetGoal(9,5);

	calculate();
}

void AStar::draw2D( he::gui::Canvas2D* canvas )
{
	m_Grid->draw2D(canvas);
}

void AStar::calculate()
{
	processNode(m_StartIndex);

	while (!m_OpenNodes.contains(m_GoalIndex))
	{
		int totalScore = INT_MAX;
		int nextNodeIndex;
		m_OpenNodes.forEach([&](int nodeIndex)
		{
			Node& node = m_Grid->getNodeByIndex(nodeIndex);
			if (node.GetTotalScore() <= totalScore)
			{
				nextNodeIndex = node.GetIndex();
				totalScore = node.GetTotalScore();
			}
		});

		Node& nextNode = m_Grid->getNodeByIndex(nextNodeIndex);
		m_OpenNodes.remove(nextNode.GetIndex());
		nextNode.SetState(Node::NODE_PATH);
		m_ClosedNodes.add(nextNode.GetIndex());

		processNode(nextNode.GetIndex());
	}
}

void AStar::processNode( const int nodeIndex )
{
	Node& nextNode = m_Grid->getNodeByIndex(nodeIndex);
	for (int i = 0; i < ht::Grid::NeighBour_Max; ++i)
	{
		int neighBourIndex;
		if(m_Grid->getNeigbour(nextNode, static_cast<ht::Grid::NeighBour>(i), neighBourIndex))
		{
			Node& neighBour = m_Grid->getNodeByIndex(neighBourIndex);

			if (!m_ClosedNodes.contains(neighBour.GetIndex()))
			{
				ht::Node::NodeState nodeState = neighBour.GetState();
				if (nodeState != Node::NODE_BLOCKED)
				{
					neighBour.SetParentIndex(nextNode.GetIndex());
					neighBour.SetMovementCost(10);

					if (nodeState != Node::NODE_GOAL)
						neighBour.SetState(Node::NODE_VISITED);

					m_OpenNodes.add(neighBour.GetIndex());
				}
			}
		}
	}

	m_OpenNodes.remove(nodeIndex);
	m_ClosedNodes.add(nodeIndex);
	if (nextNode.GetState() != Node::NODE_START)
		nextNode.SetState(Node::NODE_PATH);

	//m_OpenNodes.sort(AStar::nodeSorter);
}

void AStar::SetGoal( const int column, const int row )
{
	m_Grid->getNode(column, row).SetState(ht::Node::NODE_GOAL);
	m_GoalIndex = m_Grid->getNode(column, row).GetIndex();
	m_Grid->ResetNodes(m_GoalIndex);
}

void AStar::SetStart( const int column, const int row )
{
	m_Grid->getNode(column,row).SetState(ht::Node::NODE_START);
	m_StartIndex = m_Grid->getNode(column,row).GetIndex();
	m_OpenNodes.add(m_StartIndex);
}

int AStar::nodeSorter( const int a, const int b )
{
	Node& nodeA = m_Grid->getNodeByIndex(a);
	Node& nodeB = m_Grid->getNodeByIndex(b);

	if (nodeA.GetTotalScore() < nodeB.GetTotalScore())
		return -1;
	else
		return 1;
}

} //end namespace