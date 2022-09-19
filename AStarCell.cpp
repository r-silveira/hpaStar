#include "AStarCell.h"

#include <iostream> 

const int AStarCell::NULLPARENT = -1;

//-----------------------------------------------------------------------------
AStarCell::Compare::~Compare()
{
}


// less than operator
//-----------------------------------------------------------------------------
bool AStarCell::Compare::operator()(const AStarCell* cell1,
                                    const AStarCell* cell2)
{
    int aux1 = cell1->getF();
    int aux2 = cell2->getF();
    
	{
        return (aux1 < aux2);
	}
}


//-----------------------------------------------------------------------------
AStarCell::AStarCell() : Cell()
{
	m_inListID = -1; // out of lists
	m_id = 0;
	m_f = 0;
	m_g = 0;
	m_h = 0;
	m_cluster = -2;
	m_entrance = false;
	m_parent = NULLPARENT;
}


//-----------------------------------------------------------------------------
AStarCell::AStarCell(int id) : Cell()
{
	m_inListID = -1; // out of lists
	m_id = id;
	m_f = 0;
	m_g = 0;
	m_h = 0;
	m_cluster = -2;
	m_entrance = false;
	m_parent = NULLPARENT;
}


//-----------------------------------------------------------------------------
void AStarCell::reset()
{
	m_inListID = -1; // out of lists
	m_f = 0;
	m_g = 0;
	m_h = 0;
	m_parent = NULLPARENT;
}


//-----------------------------------------------------------------------------
AStarCell::~AStarCell()
{
}


//-----------------------------------------------------------------------------
void AStarCell::setIndex(int i, int j)
{ 
	m_index.first = i; 
	m_index.second = j; 
};


//-----------------------------------------------------------------------------
void AStarCell::setIndex(const std::pair<int,int>& index)
{ 
	m_index = index; 
};


//-----------------------------------------------------------------------------
void AStarCell::print() const
{
	std::cout	//<< "InListID: " << m_inListID << std::endl
				<< "m_f: " << m_f << std::endl
				//<< "m_g: " << m_g << std::endl
				<< "m_h: " << m_h << std::endl
				//<< "m_parent: " << m_parent << std::endl
				<< "(i,j): (" << m_index.first << ", " << m_index.second << ")" << std::endl;
}