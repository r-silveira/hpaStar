#include "Grid.h"

#include "global.h"
#include "Log.h"

#include <cassert>
#include <cmath>
//-----------------------------------------------------------------------------

#ifndef M_PI
	#define M_PI 3.141592
#endif 


//-----------------------------------------------------------------------------
Grid::Grid()
{
	nCellsHor	= 10;
	nCellsVer	= 10;
	res			= 1.0f;
	cellLengthX = 1.0f;
	cellLengthY = 1.0f;
	xMinWorld	= 1.0f;
	xMaxWorld	= 10.0f;
	yMinWorld	= 1.0f;
	yMaxWorld	= 10.0f;
	distX		= 10.0f;
	distY		= 10.0f;
	show_functions = false;

	init(xMinWorld, xMaxWorld, yMinWorld, yMaxWorld, res);
}


//-----------------------------------------------------------------------------
void Grid::resize(int _size, float _res)
{	

}


//-----------------------------------------------------------------------------
void Grid::init(float _xMin, float _xMax, float _yMin, float _yMax, float r)
{
	res		= r;

	xMinWorld = _xMin;
	xMaxWorld = _xMax;
	yMinWorld = _yMin;
	yMaxWorld = _yMax;

	distX = fabs((xMaxWorld - xMinWorld)+1);
	distY = fabs((yMaxWorld - yMinWorld)+1);

	nCellsHor = (int)(distX*res);
	nCellsVer = (int)(distY*res);

	grid.resize(nCellsHor * nCellsVer);

	cellLengthX = (distX) / nCellsHor;
	cellLengthY = (distY) / nCellsVer;

	makeEmptyGrid();
}


//-----------------------------------------------------------------------------
void Grid::makeEmptyGrid()
{
	int ind = 0;

	// All cells are free type
	for(int j = 0; j < nCellsHor; ++j)
    {
		for(int i = 0; i < nCellsVer; ++i)
		{
			grid[ind].setType( Cell::T_FREE );
			grid[ind].setID( ind );
			grid[ind].getNeighborhoodList()->clear();
			grid[ind].setIndex(i, j);
			ind++;
		}
	}
}


//-----------------------------------------------------------------------------
void Grid::reset()
{
	// All cells are free type
	for(int ind = 0; ind < (nCellsHor * nCellsVer); ++ind)
    {
		//if ((grid[ind].getType() != Cell::T_OBST) 
		//	&& (grid[ind].getType() != Cell::T_GOAL))
		{
			//grid[ind].setType( Cell::T_FREE );
			grid[ind].reset();
		}
    }
}


//-----------------------------------------------------------------------------
void Grid::fullReset()
{
	// All cells are free type
	for(int ind = 0; ind < (nCellsHor * nCellsVer); ++ind)
    {
		//if ((grid[ind].getType() != Cell::T_OBST) 
		//	&& (grid[ind].getType() != Cell::T_GOAL))
		{
			grid[ind].setType( Cell::T_FREE );
			grid[ind].reset();
		}
    }
}


//-----------------------------------------------------------------------------
void Grid::mapAsObstacleCell(int i, int j)
{
	grid[j*nCellsHor + i].setType(Cell::T_OBST);
}

//-----------------------------------------------------------------------------

void Grid::mapAsGoalCell(int i, int j)
{
	grid[j*nCellsHor + i].setType(Cell::T_GOAL);
}


//-----------------------------------------------------------------------------
void Grid::mapAsFreeCell(int i, int j)
{
	grid[j*nCellsHor + i].setType(Cell::T_FREE);
}


//-----------------------------------------------------------------------------
//
//    Esta funcao renderString2 coloca o string junto ao objeto anteriormente desenhado.
//    Ou seja, a posicao onde o string é colocado é controlada pela matriz ModelView.
//
void Grid::renderString(char *string, const float color[], void *font)
{
    char *c;
    int viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, viewport[2], viewport[3]);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glColor3fv(color);
        glRasterPos3f(0,0,0);
        gluOrtho2D(0,viewport[2], 0,viewport[3]);
        for (c=string; (*c)!='\0'; c++)
            glutBitmapCharacter(font,(*c));
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


//-----------------------------------------------------------------------------
void Grid::draw(float offsetX, float offsetY, bool cellContour)
{
	if (!grid.empty())
	{
		if (cellContour)
		{
			float x;
			float y;

			//glLineWidth(2);
			const float textColor[] = {0.0f, 1.0f, 0.0f};	

			for (int j = 0; j < nCellsVer; ++j)
			{
				for (int i = 0; i < nCellsHor; ++i)
				{
					x = i*cellLengthX;
					y = j*cellLengthY;

					if (grid[j*nCellsHor+i].getType() == Cell::T_OBST)
					{	
						glColor4f(1.0f , 0.0f, 0.0f, 0.2f);
		                
						glBegin(GL_QUADS);
							
							glVertex3f(x+offsetX,				y+offsetY,				0.2f);
							glVertex3f(x+cellLengthX+offsetX,	y+offsetY,				0.2f);
							glVertex3f(x+cellLengthX+offsetX,	y+cellLengthY+offsetY,	0.2f);
							glVertex3f(x+offsetX,				y+cellLengthY+offsetY,	0.2f);

						glEnd();
					}

					
					/*
					if (grid[j*nCellsHor+i].isEntrance())
					{	
						glColor4f(0.0f , 1.0f, 0.0f, 0.5f);
		                
						glBegin(GL_QUADS);
							
							glVertex3f(x+offsetX,				y+offsetY,				0.2f);
							glVertex3f(x+cellLengthX+offsetX,	y+offsetY,				0.2f);
							glVertex3f(x+cellLengthX+offsetX,	y+cellLengthY+offsetY,	0.2f);
							glVertex3f(x+offsetX,				y+cellLengthY+offsetY,	0.2f);

						glEnd();
					}*/

					glLineStipple(1, 0xaaaa);
					glEnable(GL_LINE_STIPPLE);

					glColor3f(1.0f, 1.0f, 1.0f);

					glBegin(GL_LINE_STRIP);

						glVertex3f( x+offsetX, y+offsetY, 0.2f);
						glVertex3f( x+offsetX, y+cellLengthY+offsetY, 0.2f);

						glVertex3f( (x+cellLengthX)+offsetX, y+cellLengthY+offsetY, 0.2f);
						glVertex3f( (x+cellLengthX)+offsetX, y+offsetY, 0.2f);

					glEnd();

					glDisable(GL_LINE_STIPPLE);

					// write to debug
					if (show_functions)
					{
						glPushMatrix();

						glTranslatef(x+offsetX+0.1f, y+offsetY+0.1f, 0.25f);
						std::ostringstream buffer;
						float value = grid[j*nCellsHor+i].getG();
						buffer.str("");
						buffer << value;
						std::string str = "G: " + buffer.str();
						renderString(const_cast<char*>(str.c_str()), textColor, GLUT_BITMAP_HELVETICA_10);

						glTranslatef(0.0f, 0.3f, 0.0f);
						value = grid[j*nCellsHor+i].getH();
						buffer.str("");
						buffer << value;
						str = "H: " + buffer.str();
						renderString(const_cast<char*>(str.c_str()), textColor, GLUT_BITMAP_HELVETICA_10);
						
						glTranslatef(0.0f, 0.3f, 0.0f);
						value = grid[j*nCellsHor+i].getF();
						buffer.str("");
						buffer << value;
						str = "F: " + buffer.str();
						renderString(const_cast<char*>(str.c_str()), textColor, GLUT_BITMAP_HELVETICA_10);

						renderString(const_cast<char*>(str.c_str()), textColor, GLUT_BITMAP_HELVETICA_10);

						glPopMatrix();
					}
				}

				/*
				if (grid[j*nCellsHor+i].getInListID() == AStarCell::IN_OPENLIST)
				{
					x = i*cellLengthX;
					y = j*cellLengthY;

					glColor4f(1.0f, 1.0f, 0.0f, 0.1f);
	                
					glBegin(GL_QUADS);

						glVertex3f(x+offsetX,				y+offsetY,				0.2f);
						glVertex3f(x+cellLengthX+offsetX,	y+offsetY,				0.2f);
						glVertex3f(x+cellLengthX+offsetX,	y+cellLengthY+offsetY,	0.2f);
						glVertex3f(x+offsetX,				y+cellLengthY+offsetY,	0.2f);

					glEnd();
				}

				if (grid[j*nCellsHor+i].getInListID() == AStarCell::IN_CLOSEDLIST)
				{
					x = i*cellLengthX;
					y = j*cellLengthY;

					glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
	                
					glBegin(GL_QUADS);

						glVertex3f(x+offsetX,				y+offsetY,				0.2f);
						glVertex3f(x+cellLengthX+offsetX,	y+offsetY,				0.2f);
						glVertex3f(x+cellLengthX+offsetX,	y+cellLengthY+offsetY,	0.2f);
						glVertex3f(x+offsetX,				y+cellLengthY+offsetY,	0.2f);

					glEnd();
				}*/

			}
		}
	}
}

//-----------------------------------------------------------------------------

Grid::~Grid()
{
}

//-----------------------------------------------------------------------------

