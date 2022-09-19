#include "Cluster.h"

#include "Global.h"

//-----------------------------------------------------------------------------
Cluster::Cluster()
{
	m_grid = NULL;
	m_id = 0;
	m_start.first = 0;
	m_start.second = 0;
	m_size = 0;
	m_drawCluster = false;

	m_upClusterID	= -1;
	m_downClusterID	= -1;
	m_leftClusterID	= -1;
	m_rightClusterID= -1;

}


//-----------------------------------------------------------------------------
Cluster::Cluster(Grid* const grid)
{
	m_grid = grid;
	m_id = 0;
	m_start.first = 0;
	m_start.second = 0;
	m_drawCluster = false;
	m_size = m_grid->getNCellsHor();
	
	m_upClusterID	= -1;
	m_downClusterID	= -1;
	m_leftClusterID	= -1;
	m_rightClusterID= -1;
}


//-----------------------------------------------------------------------------
Cluster::~Cluster()
{

}


//-----------------------------------------------------------------------------
void Cluster::update()
{
	
}


//-----------------------------------------------------------------------------
void Cluster::createCluster(int id, std::pair<int,int> start, int size)
{
	m_id = id;
	m_start = start;
	m_size = size;

	associateCellsToClusters();

	//std::cout << "Id: " << m_id << std::endl;
	//std::cout << "Start: (" << m_start.first << ", " << m_start.second << ")" << std::endl;
	//std::cout << "Size: " << size << std::endl;
}


//-----------------------------------------------------------------------------
void Cluster::associateCellsToClusters()
{
	for (int j = m_start.second; j < (m_start.second + m_size); ++j)
	{
		for (int i = m_start.first; i < (m_start.first + m_size); ++i)
		{
			m_grid->getCell(i,j)->setCluster(m_id); 
		}
	}
}


//-----------------------------------------------------------------------------
void Cluster::printNeighbors() const
{
	std::cout << "ClusterID: " << m_id << std::endl;
	std::cout << "Up: " << m_upClusterID << std::endl;
	std::cout << "Down: " << m_downClusterID << std::endl;
	std::cout << "Left: " << m_leftClusterID << std::endl;
	std::cout << "Right: " << m_rightClusterID << std::endl;
	std::cout << std::endl;
}


//-----------------------------------------------------------------------------
void Cluster::draw(float offsetX, float offsetY)
{
	glLineWidth(2);
	
	glColor3f(1.0f, 1.0f, 0.0f);

	glBegin(GL_LINE_STRIP);

		glVertex3f( m_start.first+offsetX, m_start.second+offsetY,			0.2f);
		glVertex3f( m_start.first+offsetX, m_start.second+m_size+offsetY,	0.2f);

		glVertex3f( m_start.first+m_size+offsetX, m_start.second+m_size+offsetY,	0.2f);
		glVertex3f( m_start.first+m_size+offsetX, m_start.second+offsetY,			0.2f);

	glEnd();


	const float& cellLengthX = m_grid->getCellLengthX();
	const float& cellLengthY = m_grid->getCellLengthY();

	glColor4f(0.0f , 1.0f, 0.0f, 0.5f);

	for (int j = m_start.second; j < (m_start.second + m_size); ++j)
	{
		for (int i = m_start.first; i < (m_start.first + m_size); ++i)
		{
			const float x = i*cellLengthX;
			const float y = j*cellLengthY;

			if (m_drawCluster)
			{	
				glColor4f(1.0f , 1.0f, 0.0f, 0.2f);
		        
				glBegin(GL_QUADS);
					
					glVertex3f(x+offsetX,				y+offsetY,				0.2f);
					glVertex3f(x+cellLengthX+offsetX,	y+offsetY,				0.2f);
					glVertex3f(x+cellLengthX+offsetX,	y+cellLengthY+offsetY,	0.2f);
					glVertex3f(x+offsetX,				y+cellLengthY+offsetY,	0.2f);

				glEnd();
			}

			if (m_grid->getCell(i,j)->isEntrance())
			{	
				glBegin(GL_QUADS);
					
					glVertex3f(x+offsetX,				y+offsetY,				0.2f);
					glVertex3f(x+cellLengthX+offsetX,	y+offsetY,				0.2f);
					glVertex3f(x+cellLengthX+offsetX,	y+cellLengthY+offsetY,	0.2f);
					glVertex3f(x+offsetX,				y+cellLengthY+offsetY,	0.2f);

				glEnd();
			}

			//m_grid->getCell(i,j)->setCluster(m_id); 
		}
	}

	glLineWidth(1);
}



