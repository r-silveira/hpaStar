#include "HierarchicalHandler.h"

#include "Global.h"

#include <list>
#include <iostream>
#include <limits>

//-----------------------------------------------------------------------------
HierarchicalHandler::HierarchicalHandler()
{
	m_graph = NULL;
	m_clusterHandler = NULL;
	m_grid = NULL;
	m_aStar = NULL;
}


//-----------------------------------------------------------------------------
HierarchicalHandler::HierarchicalHandler(	Grid* const grid, 
											ClusterHandler* const clusterHandler,  
											AStar* const aStar )
{
	m_grid = grid;
	m_clusterHandler = clusterHandler;
	m_aStar = aStar;

	m_graph = new Graph(m_grid);
}


//-----------------------------------------------------------------------------
HierarchicalHandler::~HierarchicalHandler()
{
	if (m_graph != NULL)
	{
		delete m_graph;
	}
}


//-----------------------------------------------------------------------------
void HierarchicalHandler::draw(float offsetX, float offsetY, bool drawGraph)
{
	if (m_graph != NULL)
	{
		m_graph->draw(offsetX, offsetY, drawGraph);
		m_graph->drawEmphatize(offsetX, offsetY, drawGraph, m_pathInGraph);
	}
}


//-----------------------------------------------------------------------------
void HierarchicalHandler::createInterEdges()
{
	std::list< std::pair<AStarCell*,AStarCell*> >& entrances = *m_clusterHandler->getEntrances();

	for (std::list< std::pair<AStarCell*,AStarCell*> >::const_iterator it = entrances.begin(); 
			it != entrances.end();
			++it)
	{
		// add edges with cost 10.
		m_graph->addEdge(*it, 10);
	}
}


//-----------------------------------------------------------------------------
void HierarchicalHandler::createIntraEdges()
{
	std::pair<int,int> prevStart	= m_aStar->getStartPosition();
	std::pair<int,int> prevEnd		= m_aStar->getEndPosition();

	// for each cluster
	for (int i = 0; i < m_nodesInCluster.size(); ++i)
	{
		// for each node inside the cluster
		for (std::list<Node*>::iterator itNode = m_nodesInCluster[i].begin();
				itNode != m_nodesInCluster[i].end();
				++itNode)
		{
			//std::cout << (**itNode)->getIndex()->first << ", " << (**itNode)->getIndex()->second << std::endl;


			//++itNode;
			// connect with the others
			for (std::list<Node*>::iterator itNeighbor = itNode;
					itNeighbor != m_nodesInCluster[i].end();
					++itNeighbor)
			{
				m_aStar->clearPath();
				m_aStar->reset();

				m_aStar->run(	(*(*itNode))->getIndex()->first, 
								(*(*itNode))->getIndex()->second, 
								(*(*itNeighbor))->getIndex()->first, 
								(*(*itNeighbor))->getIndex()->second );

				//std::cout << "From: (" << (*(*itNode))->getIndex()->first << "," << (*(*itNode))->getIndex()->second
				//			<< ") to (" <<(*(*itNeighbor))->getIndex()->first << "," << (*(*itNeighbor))->getIndex()->second << ")" << std::endl;
				//std::cout << "Cost: " << m_aStar->getPathCost() << std::endl;

				m_graph->addEdge(std::make_pair(**itNode, **itNeighbor), m_aStar->getPathCost());
				m_graph->addEdge(std::make_pair(**itNeighbor, **itNode), m_aStar->getPathCost());
			}
			//--itNode;
		}
	}

	m_aStar->setStartPosition(prevStart);
	m_aStar->setEndPosition(prevEnd);
}


//-----------------------------------------------------------------------------
void HierarchicalHandler::createGraphFromClustersEntrances()
{
	std::list< std::pair<AStarCell*,AStarCell*> >& entrances = *m_clusterHandler->getEntrances();


	m_graph->addNodeList(entrances);
}


//-----------------------------------------------------------------------------
void HierarchicalHandler::createGraphFromClustersEntrances(const std::list< std::pair<AStarCell*,AStarCell*> >& entrances)
{
	m_graph->addNodeList(entrances);
}


//-----------------------------------------------------------------------------
void HierarchicalHandler::groupNodesPerClusters()
{
	for (int i = 0; i < m_nodesInCluster.size(); ++i)
	{
		m_nodesInCluster[i].clear();
	}

	std::vector<Node>* const vecNodes = m_graph->getVecNodes();

	m_nodesInCluster.resize(m_clusterHandler->getVeClusters()->size());

	for (int i = 0; i < vecNodes->size(); ++i)
	{
		m_nodesInCluster[(*vecNodes)[i]->getCluster()].push_back(&((*vecNodes)[i]));
	}

	/*
	// Debug only
	for (int i = 0; i < m_nodesInCluster.size(); ++i)
	{
		if (!m_nodesInCluster[i].empty())
		{
			for (std::list< Node* >::iterator it = m_nodesInCluster[i].begin(); 
					it != m_nodesInCluster[i].end();
					++it)
			{
				std::cout	<< "Nodes in cluster[" << i << "] = (" 
						<< (*(*it))->getIndex()->first << "," << (*(*it))->getIndex()->second << ")" << std::endl;
				//std::cout << "weight: " <<  *(*it))->getIndex()<< std::endl;
			}
			//std::cout	<< "Nodes in cluster[" << i << "] = " 
			//			<< m_nodesInCluster[i].size() << std::endl;
		}
	}
	std::cout << std::endl;
	//*/
	
}


//-----------------------------------------------------------------------------
void HierarchicalHandler::update()
{
	m_pathInGraph.clear();

	createGraphFromClustersEntrances();
	groupNodesPerClusters();

	createInterEdges();
	createIntraEdges();

	//m_graph->print();
}


//-----------------------------------------------------------------------------
void HierarchicalHandler::updateChangingStartAndEnd()
{
	m_entrances.clear();
	m_entrances =  *m_clusterHandler->getEntrances();

	createGraphFromClustersEntrances();
	groupNodesPerClusters();

	addStartAndEndNode(&m_entrances);

	createGraphFromClustersEntrances(m_entrances);
	groupNodesPerClusters();

	createInterEdges();
	createIntraEdges();

	//m_graph->print();
}


//----------------------------------------------------------------------------
void HierarchicalHandler::addStartAndEndNode(std::list< std::pair<AStarCell*,AStarCell*> >* entrances)
{
	// find the cluster that start point is located 
	int clusterIndex = m_grid->getCell(m_aStar->getStartPosition())->getCluster();	

	// forh each entrance
	for (std::list< Node* >::iterator it = m_nodesInCluster[clusterIndex].begin(); 
					it != m_nodesInCluster[clusterIndex].end();
					++it)
	{
		entrances->push_back(std::make_pair<AStarCell*,AStarCell*>(m_grid->getCell(m_aStar->getStartPosition()), (*(*it))));
	}

	clusterIndex = m_grid->getCell(m_aStar->getEndPosition())->getCluster();	

	// forh each entrance
	for (std::list< Node* >::iterator it = m_nodesInCluster[clusterIndex].begin(); 
					it != m_nodesInCluster[clusterIndex].end();
					++it)
	{
		entrances->push_back(std::make_pair<AStarCell*,AStarCell*>(m_grid->getCell(m_aStar->getEndPosition()), (*(*it))));
	}

	//m_graph->updateNodeList(entrances);
	//groupNodesPerClusters();
	//createInterEdges();
	//createIntraEdges();

}


//-----------------------------------------------------------------------------
void HierarchicalHandler::computeLowLevelPath()
{
	m_finalPath.clear();

	if (!m_pathInGraph.empty())
	{
		for (std::list< AStarCell* >::const_iterator it = m_pathInGraph.begin();
				it != m_pathInGraph.end();
				++it)
		{
			std::list< AStarCell* >::const_iterator from = it;
			++it;

			if (it != m_pathInGraph.end())
			{
				std::list< AStarCell* >::const_iterator to = it;

				m_aStar->clearPath();
				m_aStar->reset();
				m_aStar->run(	(*from)->getIndex()->first, 
								(*from)->getIndex()->second, 
								(*to)->getIndex()->first,
								(*to)->getIndex()->second);

				std::list<AStarCell*>& path = m_aStar->getPath();

				for (std::list<AStarCell*>::const_iterator itFinalPath = path.begin();
						itFinalPath != path.end(); ++itFinalPath)
				{
					m_finalPath.push_back((*itFinalPath));
				}
			}
			--it;
			//else
			//{
			//	--it;
			//}
		}
	}
	else
	{
		m_aStar->clearPath();
		m_aStar->reset();
		m_aStar->run(	m_aStar->getStartPosition().first, 
						m_aStar->getStartPosition().second, 
						m_aStar->getEndPosition().first,
						m_aStar->getEndPosition().second);

		std::list<AStarCell*>& path = m_aStar->getPath();

		for (std::list<AStarCell*>::const_iterator itFinalPath = path.begin();
				itFinalPath != path.end(); ++itFinalPath)
		{
			m_finalPath.push_back((*itFinalPath));
		}
	}
}


//-----------------------------------------------------------------------------
// define a path from the entrance to the goal point
void HierarchicalHandler::processAStarInGraph()
{
	if (!m_nodesInCluster.empty())
	{
		std::pair<int,int> prevStart	= m_aStar->getStartPosition();
		std::pair<int,int> prevEnd		= m_aStar->getEndPosition();

		m_aStar->clearPath();
		m_aStar->reset();

		m_pathInGraph.clear();

		const int& initNodeID	= m_graph->getNodeIDCorrespondent(m_grid->getCell(m_aStar->getStartPosition()));
		const int& endNodeID	= m_graph->getNodeIDCorrespondent(m_grid->getCell(m_aStar->getEndPosition()));

		if ((initNodeID != -1) && (endNodeID != -1))
		{
			m_aStar->runInGraph(initNodeID, endNodeID, m_graph);
		}

		std::list< AStarCell* >& aux = m_aStar->getPath();

		for (std::list< AStarCell* >::const_iterator it = aux.begin();
				it != aux.end(); ++it)
		{
			m_pathInGraph.push_back((*it));
		}

		m_aStar->clearPath();
		m_aStar->reset();

		computeLowLevelPath();

		m_aStar->setStartPosition(prevStart);
		m_aStar->setEndPosition(prevEnd);
	}
}

//-----------------------------------------------------------------------------
void HierarchicalHandler::drawFinalPath(float offsetX, float offsetY)
{
	const float cellLengthX = m_grid->getCellLengthX();
	const float cellLengthY = m_grid->getCellLengthY();

	float blueColor = 0.0f;

	if (!m_finalPath.empty())
	{
		glBegin(GL_QUADS);

		for (std::list<AStarCell*>::const_iterator it = m_finalPath.begin(); 
				it != m_finalPath.end(); ++it)
		{
			glColor4f(0.0f, 1.0f - blueColor, blueColor, 0.5f);

			blueColor += 1.0f/(m_finalPath.size());

			const float x = (*it)->getIndex()->first * cellLengthX;
			const float y = (*it)->getIndex()->second * cellLengthY;

			glVertex3f(x+offsetX,				y+offsetY,				0.2f);
			glVertex3f(x+cellLengthX+offsetX,	y+offsetY,				0.2f);
			glVertex3f(x+cellLengthX+offsetX,	y+cellLengthY+offsetY,	0.2f);
			glVertex3f(x+offsetX,				y+cellLengthY+offsetY,	0.2f);
		}

		glEnd();
	}
}