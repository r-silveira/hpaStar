#include "AStarClosedListDefault.h"

#include <iostream>

//-----------------------------------------------------------------------------
AStarClosedListDefault::~AStarClosedListDefault()
{
}


//-----------------------------------------------------------------------------
void AStarClosedListDefault::init(int maxNodeId)
{
    m_list.clear();
}

//-----------------------------------------------------------------------------
void AStarClosedListDefault::add(AStarCell* ptCell)
{
	ptCell->setInListID(AStarCell::IN_CLOSEDLIST);
    m_list.push_back(ptCell);
}


//-----------------------------------------------------------------------------
void AStarClosedListDefault::remove(int nodeId)
{
	for (std::list<AStarCell*>::iterator it = m_list.begin(); it != m_list.end(); ++it)
	{
        if ((*it)->getID() == nodeId)
        {
            m_list.erase(it);
            return;
        }
	}
}


//-----------------------------------------------------------------------------
void AStarClosedListDefault::print() const
{
	std::cout << "AStar Closed List (Default)" << std::endl;
	std::cout << "---------------------------" << std::endl;

	for (std::list<AStarCell*>::const_iterator itList = m_list.begin(); itList != m_list.end(); ++itList)
	{
		(*itList)->print();		
		std::cout << std::endl;
	}
	std::cout << std::endl;
}


//-----------------------------------------------------------------------------

