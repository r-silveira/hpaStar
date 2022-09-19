#include "Graph.h"

#include "Global.h"

#ifndef NULL
	#define NULL 0
#endif


//-----------------------------------------------------------------------------
Graph::Graph()
{
	m_grid = NULL;

	m_graph.reserve(500);
	m_nodes.reserve(500);
}

//-----------------------------------------------------------------------------
Graph::Graph(Grid* const grid)
{
	m_grid = grid;

	m_graph.reserve(500);
	m_nodes.reserve(500);
}


//-----------------------------------------------------------------------------
Graph::~Graph()
{
	
}


//-----------------------------------------------------------------------------
int Graph::howManyNodesEquals(AStarCell* const c)
{
	int result = 0;

	for (int i = 0; i < m_nodes.size(); ++i)
	{
		if (m_nodes[i]->getID() == c->getID())
		{
			++result;
		}
	}

	return result;
}


//-----------------------------------------------------------------------------
void Graph::removeNodesEquals()
{
	for (std::vector<Node>::iterator it = m_nodes.begin(); it != m_nodes.end(); ++it)
	{
		if ((howManyNodesEquals((*it)) > 2) && (it != m_nodes.begin()))
		{
			std::vector<Node>::iterator prev = it;
			--prev;

			m_nodes.erase(it);
			
			it = prev;
		}
	}
}


//-----------------------------------------------------------------------------
void Graph::addNodeList(const std::list<std::pair<AStarCell*,AStarCell*> >& entrances)
{
	m_nodes.resize(entrances.size());
	//m_nodes.clear();

	int ind = 0;

	for (std::list<std::pair<AStarCell*,AStarCell*> >::const_iterator it = entrances.begin();
			it != entrances.end(); ++it)
	{
		m_nodes[ind] = it->first;
		++ind;
	}

	removeNodesEquals();

	m_graph.resize(m_nodes.size());

	for (int i = 0; i < m_graph.size(); ++i)
	{
		m_graph[i].clear();
	}
}


//-----------------------------------------------------------------------------
void Graph::updateNodeList(const std::list<std::pair<AStarCell*,AStarCell*> >& entrances)
{
	int ind = 0;

	for (std::list<std::pair<AStarCell*,AStarCell*> >::const_iterator it = entrances.begin();
			it != entrances.end(); ++it)
	{
		m_nodes.push_back(it->first);
	}

	removeNodesEquals();

	m_graph.resize(m_nodes.size());

	for (int i = 0; i < m_graph.size(); ++i)
	{
		m_graph[i].clear();
	}
}


//-----------------------------------------------------------------------------
void Graph::addEdge(const std::pair<AStarCell*,AStarCell*>& edge, int cost)
{
	// search for where to insert this edge
	for (int i = 0; i < m_nodes.size(); ++i)
	{
		// no cicles
		if (edge.first != edge.second)
		{
			// node found!
			if (edge.first->getID() == m_nodes[i]->getID())
			{
				const std::pair<Node, int> graphEdge(edge.second, cost);
				m_graph[i].push_back(graphEdge);
			}
		}
	}
}


//-----------------------------------------------------------------------------
int Graph::getNodeIDCorrespondent(AStarCell* const cell)
{
	int nodeID = -1;

	for (int i = 0; i < m_nodes.size(); ++i)
	{
		if (m_nodes[i]->getID() == cell->getID())
		{
			nodeID = i;
		}
	}

	return nodeID;
}


//-----------------------------------------------------------------------------
void Graph::draw(float offsetX, float offsetY, bool drawGraph)
{
	if (drawGraph)
	{
		const float& cellLengthX = m_grid->getCellLengthX();
		const float& cellLengthY = m_grid->getCellLengthY();

		glLineWidth(2);

		glColor4f(1.0f , 1.0f, 0.6f, 0.5f);

		for (int i = 0; i < m_nodes.size(); ++i)
		{
			const float nx = m_nodes[i]->getIndex()->first*cellLengthX;
			const float ny = m_nodes[i]->getIndex()->second*cellLengthY;

			drawCircle(nx+offsetX+cellLengthX*0.5f, ny+offsetY+cellLengthY*0.5f, cellLengthX*0.5f);
			

			if (!m_graph[i].empty())
			{
				for (std::list< std::pair<Node, int> >::const_iterator it = m_graph[i].begin(); it != m_graph[i].end(); ++it)
				{
					const float x = (*it).first->getIndex()->first*cellLengthX;
					const float y = (*it).first->getIndex()->second*cellLengthY;

					glBegin(GL_LINES);
						
						glVertex3f(nx+offsetX+cellLengthX*0.5f,	ny+offsetY+cellLengthX*0.5f, 0.2f);
						glVertex3f(x+offsetX+cellLengthX*0.5f,	y+offsetY+cellLengthX*0.5f,	 0.2f);

					glEnd();
				}
			}
		}
		glLineWidth(1);
	}
}


//-----------------------------------------------------------------------------
void Graph::drawEmphatize(	float offsetX, 
							float offsetY, 
							bool drawGraph, 
							const std::list<Node>& pathInGraph)
{
	if (drawGraph)
	{
		const float& cellLengthX = m_grid->getCellLengthX();
		const float& cellLengthY = m_grid->getCellLengthY();

		glLineWidth(2);

		glColor3f(1.0f , 0.6f, 0.2f);

		for (	std::list<Node>::const_iterator it = pathInGraph.begin();
				it != pathInGraph.end();
				++it )
		{
			const float nx = (*it)->getIndex()->first*cellLengthX;
			const float ny = (*it)->getIndex()->second*cellLengthY;

			drawCircle(nx+offsetX+cellLengthX*0.5f, ny+offsetY+cellLengthY*0.5f, cellLengthX*0.5f);
		}
		glLineWidth(1);
	}
}


//-----------------------------------------------------------------------------
void Graph::print() const
{
	std::cout << "====================================" << std::endl;
	for (int i = 0; i < m_nodes.size(); ++i)
	{
		std::cout << "Node[" << i << "]: (" << m_nodes[i]->getIndex()->first << ", " 
											<<  m_nodes[i]->getIndex()->second << ")" << std::endl;

		std::cout << "Neighbors: " << std::endl;
		if (!m_graph[i].empty())
		{
			for (std::list< std::pair<Node, int> >::const_iterator it = m_graph[i].begin(); it != m_graph[i].end(); ++it)
			{
				const float x = (*it).first->getIndex()->first;
				const float y = (*it).first->getIndex()->second;

				std::cout <<"(" << x << ", "  <<  y << ")" 
					<< " - wight: " << (*it).second << std::endl;

			}
		}
	}
}

