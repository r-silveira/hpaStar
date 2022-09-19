#include "AStar.h"

#include "Global.h"

#include <iostream>
#include <vector>

#ifndef NULL
	#define NULL 0
#endif

//-----------------------------------------------------------------------------
AStar::AStar()
{
	m_openList = new AStarOpenList();
	m_grid = NULL;

	processAllNeighborhood();
}



//-----------------------------------------------------------------------------
AStar::AStar(Grid* const grid)
{
	m_grid = grid;
	m_openList = new AStarOpenList();

	processAllNeighborhood();
}



//-----------------------------------------------------------------------------
void AStar::reset()
{
	m_grid->reset();
	m_openList->clear();
}


//-----------------------------------------------------------------------------
void AStar::debug_neighborhood()
{
	for (int i  = 0; i < m_grid->getNCellsHor() * m_grid->getNCellsVer(); ++i)
	{
		std::list<AStarCell*>& neighborhoodList = (*m_grid->getCell(i)->getNeighborhoodList());

		std::cout << "neighborhood list: (" <<  i << ")" << std::endl;
		for (std::list<AStarCell*>::const_iterator it = neighborhoodList.begin();
			it != neighborhoodList.end(); ++it)
		{
			std::cout << (*it)->getID() << ", ";
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
}


//-----------------------------------------------------------------------------
void AStar::debug_openList()
{
	std::cout << std::endl;
	m_openList->print();
	std::cout << std::endl;
}

//-----------------------------------------------------------------------------
AStar::~AStar()
{
	delete m_openList;
	m_openList = NULL;

	m_grid = NULL;
}


//-----------------------------------------------------------------------------
void AStar::update()
{
	updateIndex();
	processAllNeighborhood();
}


//-----------------------------------------------------------------------------
void AStar::updateIndex()
{
	for (int i  = 0; i < m_grid->getNCellsHor(); ++i)
	{
		for (int j  = 0; j < m_grid->getNCellsVer(); ++j)
		{
			m_grid->getCell(i,j)->setIndex(i,j);
		}
	}
}


//-----------------------------------------------------------------------------
bool AStar::checkNeighborExistence(int i, int j)
{
	return ((i >= 0) && (i < m_grid->getNCellsHor())
				&& (j >= 0) && (j < m_grid->getNCellsVer()));
}


//-----------------------------------------------------------------------------
void AStar::processAllNeighborhood()
{
	for (int i  = 0; i < m_grid->getNCellsHor(); ++i)
	{
		for (int j  = 0; j < m_grid->getNCellsVer(); ++j)
		{
			// check neighborhood of cell x(i,j)
			//  _____
			// |1|2|3|
			// |4|x|5|
			// |6|7|8|
			// 
			const int indx = j*m_grid->getNCellsHor() + i;

			// 1
			if (checkNeighborExistence(i-1, j+1))
			{
				m_grid->getCell(indx)->getNeighborhoodList()->push_back(m_grid->getCell(i-1, j+1));
			}

			// 2
			if (checkNeighborExistence(i, j+1))
			{
				m_grid->getCell(indx)->getNeighborhoodList()->push_back(m_grid->getCell(i, j+1));
			}

			// 3
			if (checkNeighborExistence(i+1, j+1))
			{
				m_grid->getCell(indx)->getNeighborhoodList()->push_back(m_grid->getCell(i+1, j+1));
			}

			// 4
			if (checkNeighborExistence(i-1, j))
			{
				m_grid->getCell(indx)->getNeighborhoodList()->push_back(m_grid->getCell(i-1, j));
			}

			// 5
			if (checkNeighborExistence(i+1, j))
			{
				m_grid->getCell(indx)->getNeighborhoodList()->push_back(m_grid->getCell(i+1, j));
			}

			// 6
			if (checkNeighborExistence(i-1, j-1))
			{
				m_grid->getCell(indx)->getNeighborhoodList()->push_back(m_grid->getCell(i-1, j-1));
			}
			
			// 7
			if (checkNeighborExistence(i, j-1))
			{
				m_grid->getCell(indx)->getNeighborhoodList()->push_back(m_grid->getCell(i, j-1));
			}

			// 8
			if (checkNeighborExistence(i+1, j-1))
			{
				m_grid->getCell(indx)->getNeighborhoodList()->push_back(m_grid->getCell(i+1, j-1));
			}
		}
	}
}


//-----------------------------------------------------------------------------
void AStar::setStartPosition(int i, int j)
{
	m_startPosition.first = i;
	m_startPosition.second = j;
}


//-----------------------------------------------------------------------------
void AStar::setStartPosition(const std::pair<int,int>& p)
{
	m_startPosition = p;
}


//-----------------------------------------------------------------------------
void AStar::setEndPosition(int i, int j)
{
	m_endPosition.first = i;
	m_endPosition.second = j;
}


//-----------------------------------------------------------------------------
void AStar::setEndPosition(const std::pair<int,int>& p)
{
	m_endPosition = p;
}


//-----------------------------------------------------------------------------
int AStar::goalDistEstimate(AStarCell* const cell)
{
	const std::pair<int,int>* cellIndex = cell->getIndex();

	return 10*(abs(m_endPosition.first - cellIndex->first) 
				+ abs(m_endPosition.second - cellIndex->second));
}


//-----------------------------------------------------------------------------
void AStar::draw(float offsetX, float offsetY)
{
	const float cellLengthX = m_grid->getCellLengthX();
	const float cellLengthY = m_grid->getCellLengthY();

	// start position
	float x = m_startPosition.first * cellLengthX;
	float y = m_startPosition.second * cellLengthY;

	glColor4f(0.0f , 1.0f, 0.0f, 1.0f);

	drawCircle(x+offsetX+cellLengthX*0.5f, y+offsetY+cellLengthY*0.5f, cellLengthX*0.5f);

	// goal position
	x = m_endPosition.first * cellLengthX;
	y = m_endPosition.second * cellLengthY;

	glColor4f(0.0f , 0.0f, 1.0f, 1.0f);

	drawCircle(x+offsetX+cellLengthX*0.5f, y+offsetY+cellLengthY*0.5f, cellLengthX*0.5f);
}


//-----------------------------------------------------------------------------
void AStar::drawPath(float offsetX, float offsetY)
{
	if (!m_path.empty())
	{
		const float cellLengthX = m_grid->getCellLengthX();
		const float cellLengthY = m_grid->getCellLengthY();

		glBegin(GL_QUADS);

		float blueColor = 0.0f;

		//glColor4f(1.0f , 1.0f, 1.0f, 0.5f);


		for (std::list<AStarCell*>::const_iterator it = m_path.begin(); 
				it != m_path.end(); ++it)
		{
			glColor4f(0.0f, 1.0f - blueColor, blueColor, 0.5f);

			blueColor += 1.0f/m_path.size();

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


//-----------------------------------------------------------------------------
void AStar::clearPath()
{
	m_path.clear();
}


//-----------------------------------------------------------------------------
AStarCell* AStar::getBestCellFromOpenList()
{
	return m_openList->pop();
}


//-----------------------------------------------------------------------------
bool AStar::isGoal(AStarCell* cell)
{
	return (cell->getID() == (m_endPosition.second * m_grid->getNCellsHor() + m_endPosition.first));
}


//-----------------------------------------------------------------------------
bool AStar::isValid(const AStarCell* cell)
{
	if ((cell->getType() == Cell::T_OBST) || (cell->getInListID() == AStarCell::IN_CLOSEDLIST))
	{
		return false;
	}
	return true;
}


//-----------------------------------------------------------------------------
int AStar::isDiagonal(AStarCell* const cell, AStarCell* const parent)
{
	// OTIMIZAR!
	// acho que pode tirar o abs, e deixar abs de tudo.
	if (abs(cell->getIndex()->first 
				- parent->getIndex()->first 
				+ cell->getIndex()->second 
				- parent->getIndex()->second) == 1)
	{
		return false;
	}
	return true;
}


//-----------------------------------------------------------------------------
int AStar::computeG(int parent, AStarCell* const cell)
{
	AStarCell* const parentCell = m_grid->getCell(parent);
	int g = 0;

	if (isDiagonal(cell, parentCell))
	{
		g = m_grid->getCell(parent)->getG() + 14;
	}
	else
	{
		g = m_grid->getCell(parent)->getG() + 10;
	}

	return g;

}


//-----------------------------------------------------------------------------
int AStar::computeGInGraph(int parent, std::pair<Node,int> const cell)
{
	AStarCell* const parentCell = m_grid->getCell(parent);
	int g = 0;

	g = m_grid->getCell(parent)->getG() + cell.second;

	return g;

}


//-----------------------------------------------------------------------------
bool AStar::run(int si, int sj, int ei, int ej)
{
	m_startPosition.first = si;
	m_startPosition.second = sj;

	m_endPosition.first = ei;
	m_endPosition.second = ej;

	std::pair<int, int> cellIndex(m_startPosition.first, m_startPosition.second);
	AStarCell* startCell = m_grid->getCell(cellIndex);

	startCell->setParent(AStarCell::NULLPARENT);
	startCell->setG(0);
	startCell->setH( goalDistEstimate(startCell) );
	startCell->setF(startCell->getG() + startCell->getH());
	startCell->setInListID(AStarCell::IN_OPENLIST);

	m_openList->insert(startCell);
		
	while (!m_openList->isEmpty())
	{
		AStarCell* currentCell = getBestCellFromOpenList();

		currentCell->setInListID(AStarCell::IN_CLOSEDLIST);

		if (isGoal(currentCell))
		{
			recoveryPath();
			return true;
		}
		else
		{
			for (std::list<AStarCell*>::iterator it = currentCell->getNeighborhoodList()->begin();
					it != currentCell->getNeighborhoodList()->end(); ++it)
			{
				if (isValid(*it))
				{
					const int itCellG = computeG(currentCell->getID(), (*it));

					if ((*it)->getInListID() != AStarCell::IN_OPENLIST)
					{
						(*it)->setParent(currentCell->getID());
						(*it)->setG( itCellG );
						(*it)->setH( goalDistEstimate((*it)) );
						(*it)->setF((*it)->getG() + (*it)->getH());
						(*it)->setInListID(AStarCell::IN_OPENLIST);
						m_openList->insert(*it);
					} 
					else if (itCellG < (*it)->getG())
					{
						(*it)->setParent(currentCell->getID());
						(*it)->setG( itCellG );
						(*it)->setF((*it)->getG() + (*it)->getH());
					}
				}
			}
		}
	}

	return false;
}


//-----------------------------------------------------------------------------
void AStar::recoveryPath()
{
	AStarCell* currentCell = m_grid->getCell(m_endPosition);

	m_path.clear();

	while ((*currentCell->getIndex()) != m_startPosition)
	{
		m_path.push_front(currentCell);
		currentCell = m_grid->getCell(currentCell->getParent());
	}

	m_path.push_front(currentCell);
}


//-----------------------------------------------------------------------------
std::list<AStarCell*> AStar::getPath()
{
	return m_path;
}


//-----------------------------------------------------------------------------
bool AStar::runInGraph(int nodeIDStart, int nodeIDEnd, Graph* graph)
{
	typedef std::list<std::pair<Node, int> > AdjacentList;

	int currentNodeID = nodeIDStart;
	std::vector<Node>* vecNodes = graph->getVecNodes();

	m_startPosition.first = (*vecNodes)[nodeIDStart]->getIndex()->first;
	m_startPosition.second = (*vecNodes)[nodeIDStart]->getIndex()->second;

	m_endPosition.first = (*vecNodes)[nodeIDEnd]->getIndex()->first;
	m_endPosition.second = (*vecNodes)[nodeIDEnd]->getIndex()->second;

	//std::pair<int, int> cellIndex(m_startPosition.first, m_startPosition.second);
	AStarCell* startCell = (*vecNodes)[nodeIDStart];

	startCell->setParent(AStarCell::NULLPARENT);
	startCell->setG(0);
	startCell->setH( goalDistEstimate(startCell) );
	startCell->setF(startCell->getG() + startCell->getH());
	startCell->setInListID(AStarCell::IN_OPENLIST);

	m_openList->insert(startCell);

	while (!m_openList->isEmpty())
	{
		AStarCell* currentCell = getBestCellFromOpenList();

		currentNodeID = graph->getNodeIDCorrespondent(currentCell);

		currentCell->setInListID(AStarCell::IN_CLOSEDLIST);

		if (isGoal(currentCell))
		{
			recoveryPath();
			return true;
		}
		else
		{
			for (AdjacentList::iterator it = (*graph->getGraph())[currentNodeID].begin();
				it != (*graph->getGraph())[currentNodeID].end(); 
				++it)
			{
				if (isValid(it->first))
				{
					const int itCellG = computeGInGraph(currentCell->getID(), (*it));

					if (it->first->getInListID() != AStarCell::IN_OPENLIST)
					{
						it->first->setParent(currentCell->getID());
						it->first->setG( itCellG );
						it->first->setH( goalDistEstimate(it->first) );
						it->first->setF(it->first->getG() + it->first->getH());
						it->first->setInListID(AStarCell::IN_OPENLIST);
						m_openList->insert(it->first);
					} 
					else if (itCellG < it->first->getG())
					{
						it->first->setParent(currentCell->getID());
						it->first->setG( itCellG );
						it->first->setF(it->first->getG() + it->first->getH());
					}
				}
			}
		}
	}

	return false;
}