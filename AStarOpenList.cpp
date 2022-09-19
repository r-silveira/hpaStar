#include "AStarOpenList.h"

#include <iostream>

//-----------------------------------------------------------------------------
AStarOpenList::AStarOpenList()
{
	init();
}


//-----------------------------------------------------------------------------
AStarOpenList::~AStarOpenList()
{
}


//-----------------------------------------------------------------------------
void AStarOpenList::init()
{
    openList.clear();
}


//-----------------------------------------------------------------------------
void AStarOpenList::clear()
{
    openList.clear();
}


//-----------------------------------------------------------------------------
void AStarOpenList::insert(AStarCell* cell)
{
	openList.push_back(cell);
}


//-----------------------------------------------------------------------------
AStarCell* AStarOpenList::pop()
{
	openList.sort(AStarCell::Compare());
	// now, back position has always the element with greater F value
	AStarCell* result = openList.front();
	result->setInListID(-1);
	openList.pop_front();

	return result;
}


//-----------------------------------------------------------------------------
void AStarOpenList::print() const
{

	std::cout << "AStar Open List (Default)" << std::endl;
	std::cout << "---------------------------" << std::endl;

	for (std::list<AStarCell*>::const_iterator it = openList.begin(); it != openList.end(); ++it)
	{
		(*it)->print();
		std::cout << std::endl;
	}
}
