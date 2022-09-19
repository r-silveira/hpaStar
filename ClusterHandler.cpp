#include "Global.h"

#include "ClusterHandler.h"



#define UP_ENTRANCE 0 
#define DOWN_ENTRANCE 1
#define LEFT_ENTRANCE 2
#define RIGHT_ENTRANCE 3

//-----------------------------------------------------------------------------
ClusterHandler::ClusterHandler()
{
	m_grid = NULL;
	m_quantity	= 1;
	m_epsilon = 4;

	m_vecClusters.resize(m_quantity*m_quantity);
}


//-----------------------------------------------------------------------------
ClusterHandler::ClusterHandler(Grid* const grid)
{
	m_grid		= grid;
	m_quantity	= 1;
	m_epsilon	= 4;

	m_vecClusters.resize(m_quantity*m_quantity);

	for (int i = 0; i < m_quantity*m_quantity; ++i)
	{
		m_vecClusters[i].setGrid(grid);
	}
}


//-----------------------------------------------------------------------------
ClusterHandler::~ClusterHandler()
{
}


//-----------------------------------------------------------------------------
void ClusterHandler::update()
{
	createClusters();
	//std::cout << m_entrances.size() << std::endl;
}


//-----------------------------------------------------------------------------
void ClusterHandler::debugCluster(int id)
{
	m_vecClusters[id].setDrawCluster(true);

	//std::cout << "===============================" << std::endl;
	//isValidSymmUp(5,5);
	//isValidSymmDown(5,5);
	//isValidSymmLeft(5,5);
	//isValidSymmRight(5,5);
	//std::cout << std::endl;

}


//-----------------------------------------------------------------------------
void ClusterHandler::debugClusterNeighbors()
{
	std::cout << "============================" << std::endl;
	std::cout << "Cluster Neighbors: "			<< std::endl;
	std::cout << "============================" << std::endl;

	for (int i = 0; i < m_quantity*m_quantity; ++i)
	{
		m_vecClusters[i].printNeighbors();
	}
}


//-----------------------------------------------------------------------------
void ClusterHandler::resetClusterInfo()
{
	for(int ind = 0; ind < (m_grid->getNCellsHor() * m_grid->getNCellsVer()); ++ind)
    {
		m_grid->getCell(ind)->setCluster( -2 );
		m_grid->getCell(ind)->setIsEntrance(false);
		m_entrances.clear();
    }
}


//-----------------------------------------------------------------------------
void ClusterHandler::createClusters()
{
	m_vecClusters.resize(m_quantity*m_quantity);

	resetClusterInfo();

	for (int i = 0; i < m_quantity*m_quantity; ++i)
	{
		m_vecClusters[i].setGrid(m_grid);
	}

	int size = m_grid->getNCellsHor()/m_quantity;

	int numberOfClusters = 0;

	for (int j = 0; j < m_quantity; ++j)
	{
		for (int i = 0; i < m_quantity; ++i)
		{
			m_vecClusters[j*m_quantity+i].createCluster(j*m_quantity+i, std::pair<int,int>(i*size, j*size), size);
		}
	}

	associateClusterNeighborhood();
	createBordersEntrances(m_epsilon);

	//debugClusterNeighbors();
	//debugCluster(0);
}


//-----------------------------------------------------------------------------
void ClusterHandler::draw(float offsetX, float offsetY, bool drawCluster)
{
	if (drawCluster)
	{
		for (int i = 0; i < (int)m_vecClusters.size(); ++i)
		{
			m_vecClusters[i].draw(offsetX, offsetY);
		}
	}
}


//-----------------------------------------------------------------------------
void ClusterHandler::associateClusterNeighborhood()
{
	for (int j = 0; j < m_quantity; ++j)
	{
		for (int i = 0; i < m_quantity; ++i)
		{
			// up cluster
			if (j+1 < m_quantity)
			{
				m_vecClusters[j*m_quantity+i].setUpCluster((j+1)*m_quantity+i);

			}
			else
			{
				m_vecClusters[j*m_quantity+i].setUpCluster(-1);
			}


			// down cluster
			if (j-1 >= 0)
			{
				m_vecClusters[j*m_quantity+i].setDownCluster((j-1)*m_quantity+i);

			}
			else
			{
				m_vecClusters[j*m_quantity+i].setDownCluster(-1);
			}


			// left cluster
			if (i-1 >= 0)
			{
				m_vecClusters[j*m_quantity+i].setLeftCluster(j*m_quantity+(i-1));

			}
			else
			{
				m_vecClusters[j*m_quantity+i].setLeftCluster(-1);
			}


			// right cluster
			if (i+1 < m_quantity)
			{
				m_vecClusters[j*m_quantity+i].setRightCluster(j*m_quantity+(i+1));

			}
			else
			{
				m_vecClusters[j*m_quantity+i].setRightCluster(-1);
			}
		}
	}

}


//-----------------------------------------------------------------------------
bool ClusterHandler::isValidSymmUp(int i, int j)
{
	const int& upID = m_vecClusters[m_grid->getCell(i,j)->getCluster()].getUpCluster();
	bool result = false;

	// exsist p neghbor
	if (upID != -1)
	{
		if ((m_grid->getCell(i,j)->getType() == Cell::T_FREE) 
			&& (m_grid->getCell(i,j+1)->getType() == Cell::T_FREE)
			&& (upID == m_grid->getCell(i,j+1)->getCluster()))
		{
			result = true;
		}
	}

	//std::cout << "SymmUp" << std::endl;
	//std::cout << "SymmUp of (" << i << "," << j << ") is (" << i << "," << j+1 << ")" << std::endl;
	//std::cout << "Valid: " << result << std::endl;

	return result;
}

//-----------------------------------------------------------------------------
bool ClusterHandler::isValidSymmDown(int i, int j)
{
	const int& downID = m_vecClusters[m_grid->getCell(i,j)->getCluster()].getDownCluster();
	bool result = false;

	// exsist p neghbor
	if (downID != -1)
	{
		if ((m_grid->getCell(i,j)->getType() == Cell::T_FREE) 
			&& (m_grid->getCell(i,j-1)->getType() == Cell::T_FREE)
			&& (downID == m_grid->getCell(i,j-1)->getCluster()))
		{
			result = true;
		}
	}

	//std::cout << "SymmDown" << std::endl;
	//std::cout << "SymmDown of (" << i << "," << j << ") is (" << i << "," << j-1 << ")" << std::endl;
	//std::cout << "Valid: " << result << std::endl;

	return result;
}
		

//-----------------------------------------------------------------------------
bool ClusterHandler::isValidSymmLeft(int i, int j)
{
	const int& leftID = m_vecClusters[m_grid->getCell(i,j)->getCluster()].getLeftCluster();
	bool result = false;

	// exsist p neghbor
	if (leftID != -1)
	{
		if ((m_grid->getCell(i,j)->getType() == Cell::T_FREE) 
			&& (m_grid->getCell(i-1,j)->getType() == Cell::T_FREE)
			&& (leftID == m_grid->getCell(i-1,j)->getCluster()))
		{
			result = true;
		}
	}

	//std::cout << "SymmLeft" << std::endl;
	//std::cout << "SymmLeft of (" << i << "," << j << ") is (" << i-1 << "," << j << ")" << std::endl;
	//std::cout << "Valid: " << result << std::endl;

	return result;
}


//-----------------------------------------------------------------------------
bool ClusterHandler::isValidSymmRight(int i, int j)
{
	const int& rightID = m_vecClusters[m_grid->getCell(i,j)->getCluster()].getRightCluster();;
	bool result = false;

	// exsist p neghbor
	if (rightID != -1)
	{
		if ((m_grid->getCell(i,j)->getType() == Cell::T_FREE) 
			&& (m_grid->getCell(i+1,j)->getType() == Cell::T_FREE)
			&& (rightID == m_grid->getCell(i+1,j)->getCluster()))
		{
			result = true;
		}
	}

	//std::cout << "SymmRight" << std::endl;
	//std::cout << "SymmRight of (" << i << "," << j << ") is (" << i+1 << "," << j << ")" << std::endl;
	//std::cout << "Valid: " << result << std::endl;

	return result;
}


//-----------------------------------------------------------------------------
void ClusterHandler::addEntranceToList(AStarCell* const cell, int entranceType)
{
	std::pair<int, int>& index = (*cell->getIndex());
	
	switch(entranceType)
	{
		case UP_ENTRANCE:

			m_entrances.push_back(std::make_pair(cell, m_grid->getCell(index.first, index.second+1)));
			break;

		case DOWN_ENTRANCE:

			m_entrances.push_back(std::make_pair(cell, m_grid->getCell(index.first, index.second-1)));
			break;

		case LEFT_ENTRANCE:

			m_entrances.push_back(std::make_pair(cell, m_grid->getCell(index.first-1, index.second)));
			break;

		case RIGHT_ENTRANCE:

			m_entrances.push_back(std::make_pair(cell, m_grid->getCell(index.first+1, index.second)));
			break;

		default:
			break;
	}
}


//-----------------------------------------------------------------------------
void ClusterHandler::computeWhereIsEntrance(std::vector< std::pair< std::pair<int,int>,int> >& possibleItems, int epsilon)
{
	if (!possibleItems.empty())
	{
		if (possibleItems.size() < epsilon)
		{
			int index = floor(possibleItems.size()/2.0f);
			m_grid->getCell(possibleItems[index].first.first, 
							possibleItems[index].first.second)->setIsEntrance(true); 
			
			addEntranceToList(m_grid->getCell(	possibleItems[index].first.first, 
												possibleItems[index].first.second ),
								possibleItems[index].second);
		}
		else
		{	
			m_grid->getCell(possibleItems[0].first.first, 
							possibleItems[0].first.second)->setIsEntrance(true);

			addEntranceToList(m_grid->getCell(	possibleItems[0].first.first, 
												possibleItems[0].first.second ),
								possibleItems[0].second);

			m_grid->getCell(possibleItems[possibleItems.size()-1].first.first, 
							possibleItems[possibleItems.size()-1].first.second)->setIsEntrance(true);

			addEntranceToList(m_grid->getCell(	possibleItems[possibleItems.size()-1].first.first, 
												possibleItems[possibleItems.size()-1].first.second ),
								possibleItems[possibleItems.size()-1].second);
		}
		possibleItems.clear();
	}
}


//-----------------------------------------------------------------------------
void ClusterHandler::createBordersEntrances(int epsilon)
{
	// to each cluster
	for (int ind = 0; ind < (int)m_vecClusters.size(); ++ind)
	{
		// checking the borders

		std::pair<int,int>& startIndex = m_vecClusters[ind].getStartIndex();
		
		const int& size		= m_vecClusters[ind].getSize();
		const int horLimit	= startIndex.first + m_vecClusters[ind].getSize() - 1;
		const int verLimit	= startIndex.second + m_vecClusters[ind].getSize() - 1;


		std::vector< std::pair< std::pair<int,int>, int> > possibleItems;

		possibleItems.reserve(size);

		// up
		for (int i = startIndex.first; i < (startIndex.first + size); ++i)
		{
			if (isValidSymmUp(i, verLimit))
			{
				possibleItems.push_back(std::make_pair(std::make_pair<int,int>(i, verLimit), UP_ENTRANCE)); 
			}
			else
			{
				computeWhereIsEntrance(possibleItems, epsilon);
			}
		}
		computeWhereIsEntrance(possibleItems, epsilon);


		// down
		for (int i = startIndex.first; i < (startIndex.first + size); ++i)
		{
			if (isValidSymmDown(i, startIndex.second))
			{
				possibleItems.push_back(std::make_pair(std::make_pair<int,int>(i, startIndex.second), DOWN_ENTRANCE)); 
			}
			else
			{
				computeWhereIsEntrance(possibleItems, epsilon);
			}
		}
		computeWhereIsEntrance(possibleItems, epsilon);

		// left
		for (int j = startIndex.second; j < (startIndex.second + size); ++j)
		{
			if (isValidSymmLeft(startIndex.first, j))
			{
				possibleItems.push_back(std::make_pair(std::make_pair<int,int>(startIndex.first, j), LEFT_ENTRANCE)); 
			}
			else
			{
				computeWhereIsEntrance(possibleItems, epsilon);
			}
		}
		computeWhereIsEntrance(possibleItems, epsilon);

		// right
		for (int j = startIndex.second; j < (startIndex.second + size); ++j)
		{
			if (isValidSymmRight(horLimit, j))
			{
				possibleItems.push_back(std::make_pair(std::make_pair<int,int>(horLimit, j), RIGHT_ENTRANCE)); 
		
			}
			else
			{
				computeWhereIsEntrance(possibleItems, epsilon);
			}
		}
		computeWhereIsEntrance(possibleItems, epsilon);
	}
}
