#if defined(_MSC_VER) && _MSC_VER <= 0x0600
#pragma warning(disable : 4786)
#endif

#include "Demo.h"
#include <cmath>
#include "Log.h"
#include "Tick.h"

#include <string>
#include <fstream>

const GLfloat Demo::lightPosition[]			= { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat Demo::lightColorAmbient[]		= { 0.4f, 0.4f, 0.4f, 1.0f };
const GLfloat Demo::lightColorDiffuse[]		= { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat Demo::lightColorSpecular[]	= { 1.0f, 1.0f, 1.0f, 0.5f };
const GLfloat Demo::textColor[]				= { 0.9f, 0.9f, 0.9f};
const char*	  Demo::textureNames[]			= {"citySnow.jpg", "labyrinth.jpg", "city.jpg", "null.jpg"};

//----------------------------------------------------------------------------//
// The one and only Demo instance                                             //
//----------------------------------------------------------------------------//

Demo theDemo;

//----------------------------------------------------------------------------//
// Constructors                                                               //
//----------------------------------------------------------------------------//

Demo::Demo()
{
	m_width		= 640;
	m_height	= 480;

	m_mouseX		= 0;
	m_mouseY		= 0;
	m_bFullscreen	= false;
	m_bPaused		= false;
	m_light			= true;
	m_cellContour	= true;
	m_drawCluster	= true;
	m_drawGraph		= true;
	m_mode			= 0; // 1 == classical A*, 2 == Hierarchical
	m_renderScale	= 0.1f;

	m_bLeftMouseButtonDown		= false;
	m_bMiddleMouseButtonDown	= false;
	m_bRightMouseButtonDown		= false;

	oImage = NULL;

	m_goal.first = 1;
	m_goal.second = 1;

	aStar = new AStar(&grid);
	clusterHandler = new ClusterHandler(&grid);
	hierarchicalHandler = new HierarchicalHandler(&grid, clusterHandler, aStar);

	aStar->setStartPosition(0, 0);
	aStar->setEndPosition(0, 0);

	textureName = ".\\textures\\citySnow.jpg";
}


//----------------------------------------------------------------------------//
// Destructor                                                                 //
//----------------------------------------------------------------------------//
Demo::~Demo()
{
	delete aStar;
}


//----------------------------------------------------------------------------//
void Demo::setClusterQuantity(int n)
{ 
	clusterHandler->setQuantity(n); 
	clusterHandler->update();
	hierarchicalHandler->update();
}


//----------------------------------------------------------------------------//
void Demo::setEpsilon(int n)
{
	clusterHandler->setEpsilon(n);
}


//----------------------------------------------------------------------------//
// Render String                                                            //
//----------------------------------------------------------------------------//
void Demo::renderString(char *string, const float color[], void *font)
{
    char *c;
    int viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, viewport[2], viewport[3]);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    {
        {
            glColor3fv(color);
            glRasterPos3f(0,0,0);
            gluOrtho2D(0,viewport[2], 0,viewport[3]);
            for (c=string; (*c)!='\0'; c++)
                glutBitmapCharacter(font,(*c));
        }
    }
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


//----------------------------------------------------------------------------//
// Get the demo caption                                                       //
//----------------------------------------------------------------------------//

std::string Demo::getCaption()
{
	return "- Viewer - Near Optimal Hierarchical Path-Finding -";
}


//----------------------------------------------------------------------------//
// Get the fullscreen flag                                                    //
//----------------------------------------------------------------------------//
bool Demo::getFullscreen()
{
	return m_bFullscreen;
}


// Get the window height                                                      //
//----------------------------------------------------------------------------//
int Demo::getHeight()
{
	return m_height;
}


//----------------------------------------------------------------------------//
// Get the window width                                                       //
//----------------------------------------------------------------------------//
int Demo::getWidth()
{
	return m_width;
}


//----------------------------------------------------------------------------//
// Map into the grid the (x,y) cell as obstacle                               //
//----------------------------------------------------------------------------//
void Demo::mapAsObstacleCell(float x, float y)
{
	grid.mapAsObstacleCell((int)(x), (int)(y));	
}

//----------------------------------------------------------------------------//
// Map into the grid the (x,y) cell as goal                                   //
//----------------------------------------------------------------------------//

void Demo::mapAsFreeCell(float x, float y)
{
	grid.mapAsFreeCell((int)(x), (int)(y));	
}

//----------------------------------------------------------------------------//
// Map into the grid the (x,y) cell as obstacle                               //
//----------------------------------------------------------------------------//

void Demo::reset()
{
	//grid.resetGrid();
	aStar->reset();

	glutPostRedisplay();
}

//----------------------------------------------------------------------------//
// Draw axis                                                                  //
//----------------------------------------------------------------------------//

void Demo::drawAxis()
{
	//drawAxes
	glBegin(GL_LINES);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);

		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 10.0f, 0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 10.0f);

	glEnd();

	// reference system
	glColor3f(1.0f, 1.0f, 1.0f);

	glutSolidCube(0.3f);

	glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(9.0f, 0.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		glutSolidCone(0.3, 1, 10, 10);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 9.0f, 0.0f);
		glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
		glutSolidCone(0.3, 1, 10, 10);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(0.0f, 0.0f, 9.0f);
		glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
		glutSolidCone(0.3, 1, 10, 10);
	glPopMatrix();
}

//----------------------------------------------------------------------------//

void Demo::initGrid(float _xMin, float _xMax, float _yMin, float _yMax, float r)
{
	aStar->clearPath();
	aStar->reset();

	grid.init(_xMin, _xMax, _yMin, _yMax, r);
	
	aStar->update();
	clusterHandler->update();
	hierarchicalHandler->update();
}

//----------------------------------------------------------------------------//

void Demo::initGrid(float _size, float r)
{
	aStar->clearPath();
	aStar->reset();

	grid.init(1, _size, 1, _size, r);

	aStar->update();
	clusterHandler->update();
	hierarchicalHandler->update();
}


//----------------------------------------------------------------------------//
void Demo::saveGridConfiguration(const std::string& fileName)
{

	ofstream fileOut(fileName.c_str());

	fileOut << grid.getNCellsHor() << " " << grid.getNCellsVer() << std::endl;

	for (int i = 0; i < grid.getNCellsHor() * grid.getNCellsVer(); ++i)
	{
		fileOut << grid.getCell(i)->getType() << " ";
	}

	fileOut.close();
}


//----------------------------------------------------------------------------//
void Demo::loadGridConfiguration(const std::string& fileName)
{
	ifstream fileIn(fileName.c_str());
	int nHor;
	
	aStar->clearPath();
	aStar->reset();

	aStar->setStartPosition(0,0);
	aStar->setEndPosition(0,0);
	m_goal.first = 0;
	m_goal.second = 0;

	fileIn >> nHor;
	fileIn >> nHor;

	initGrid(nHor, 1);

	grid.fullReset();

	char type;

	for (int i = 0; i < grid.getNCellsHor() * grid.getNCellsVer(); ++i)
	{
		fileIn >> type;
		grid.getCell(i)->setType(type);
	}

	fileIn.close();

	glutPostRedisplay();
}


//----------------------------------------------------------------------------//
// Create the demo                                                            //
//----------------------------------------------------------------------------//
bool Demo::onCreate(int argc, char *argv[])
{
	// show some information
	std::cout << "o----------------------------------------------------------------o" << std::endl;
	std::cout << "|                              Viewer                            |" << std::endl;
	std::cout << "|                    (C) 2007 Renato Silveira                    |" << std::endl;
	std::cout << "|----------------------------------------------------------------|" << std::endl;
	std::cout << "| This program is free software; you can redistribute it and/or  |" << std::endl;
	std::cout << "| modify it under the terms of the GNU General Public License as |" << std::endl;
	std::cout << "| published by the Free Software Foundation; either version 2 of |" << std::endl;
	std::cout << "| the License, or (at your option) any later version.            |" << std::endl;
	std::cout << "o----------------------------------------------------------------o" << std::endl;
	std::cout << std::endl;

	// parse the command line arguments
	int arg;

	for (arg = 1; arg < argc; ++arg)
	{
		// check for fullscreen flag
		if (strcmp(argv[arg], "--fullscreen") == 0) 
		{
			m_bFullscreen = true;
		}
		// check for window flag
		else if(strcmp(argv[arg], "--window") == 0) 
		{
			m_bFullscreen = false;
		}
		// check for dimension flag
		else if((strcmp(argv[arg], "--dimension") == 0) && (argc - arg > 2))
		{
			m_width		= atoi(argv[++arg]);
			m_height	= atoi(argv[++arg]);
		  
			if ((m_width <= 0) || (m_height <= 0))
			{
				std::cerr << "Invalid dimension!" << std::endl;
				return false;
			}
		}
		else
		{
			std::cerr	<< "Usage: " << argv[0] 
						<< " [--fullscreen] [--window] [--dimension width height]" 
						<< std::endl;
			return false;
		}
	}

	return true;
}

//----------------------------------------------------------------------------//
// Handle an idle event                                                       //
//----------------------------------------------------------------------------//

void Demo::onIdle()
{
	unsigned int	tick;
	float			elapsedSeconds;

	// get the current tick value	
	tick = Tick::getTick();

	// calculate the amount of elapsed seconds
	elapsedSeconds = (float)(tick - m_lastTick) / 1000.0f;

	// adjust fps counter
	m_fpsDuration += elapsedSeconds;

	if(m_fpsDuration >= 0.1f)
	{
		m_fps			= (int)((float)m_fpsFrames / m_fpsDuration);
		m_fpsDuration	= 0.0f;
		m_fpsFrames		= 0;
	}

	// Here we update the current models
	if (!m_bPaused)
	{
		//====== insert your code here ======

	}

	// current tick will be last tick next round
	m_lastTick = tick;

	// update the screen
	glutPostRedisplay();
}

//----------------------------------------------------------------------------//
// Initialize the demo                                                        //
//----------------------------------------------------------------------------//

bool Demo::onInit()
{
	// light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightColorAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColorDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColorSpecular);

	GLfloat mat_diffuse[]	= { 1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat mat_specular[]	= { 0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat mat_shininess[]	= { 50.0f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE,	mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS,mat_shininess);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);

	// cull face
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	// anti-aliasing and blend
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);

	// we're done
	std::cout << "Initialization done." << std::endl;
	std::cout << std::endl;
	std::cout << "Quit the viewer by pressing 'q' or ESC" << std::endl;
	std::cout << std::endl;

	initTexture();

	pick.init(&grid);

	return true;
}


void Demo::initTexture()
{
	GLuint idTexture;

	if (oImage != NULL)
		delete oImage;

	oImage = new Image();

	//textureName = ".\\textures\\map.jpg";
	
	oImage->LoadFromFilename(textureName);
	
	const int nWidth	= oImage->GetWidth();
	const int nHeight	= oImage->GetHeight();
	const int nDepth	= oImage->GetPixelDepth();

	const GLenum format = (nDepth == 4) ? GL_RGBA : GL_RGB;
	
	const unsigned char* pixels = oImage->GetData();

	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	
	glTexImage2D(	GL_TEXTURE_2D, 
					0,
					nDepth,
					nWidth, 
					nHeight, 
					0, 
					format, 
					GL_UNSIGNED_BYTE, 
					pixels);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_DECAL);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}


//----------------------------------------------------------------------------//
// Handle a key event                                                         //
//----------------------------------------------------------------------------//

void Demo::onKey(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
		case 'q':
		case 'Q':
			exit(0);
			break;

		case ' ':

			break;

		case 'l':
		case 'L':
			m_light = !m_light;
		
			if (m_light)
			{
				glEnable(GL_LIGHTING);
			}
			else
			{
				glDisable(GL_LIGHTING);
			}
			break;

		case 'f':
		case 'F':
			m_bFullscreen = !m_bFullscreen;

			if (m_bFullscreen)
			{
				glutFullScreen();
			}
			else
			{
				glutReshapeWindow(m_prevWidth, m_prevHeight);
			}
			break;
	}
}

//----------------------------------------------------------------------------//
// Handle a mouse button down event                                           //
//----------------------------------------------------------------------------//

void Demo::onMouseButtonDown(int button, int x, int y)
{
	// update mouse button states
	mouseCoordWorld = pick.getWorldSpace(x, y);

	mouseCoordWorld[0] *= grid.getResolution();
	mouseCoordWorld[1] *= grid.getResolution();

	if(button == GLUT_LEFT_BUTTON)
	{
		m_bLeftMouseButtonDown = true;
		mapAsObstacleCell(mouseCoordWorld[0], mouseCoordWorld[1]);
	}

	if (button == GLUT_MIDDLE_BUTTON)
	{
		m_bMiddleMouseButtonDown = true;
		mapAsFreeCell(mouseCoordWorld[0], mouseCoordWorld[1]);
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		m_bRightMouseButtonDown = true;

		aStar->clearPath();
		aStar->setStartPosition(m_goal);
		aStar->reset();

		m_goal.first = mouseCoordWorld[0];
		m_goal.second = mouseCoordWorld[1];

		aStar->setEndPosition(m_goal.first, m_goal.second);

		if ((m_mode == 0) || (clusterHandler->getQuantity() == 1))
		{
			findPath();
		} 
		else if (m_mode == 1)
		{
			hierarchicalHandler->updateChangingStartAndEnd();

			// must be called in this order!
			//hierarchicalHandler->firstStep();
			//hierarchicalHandler->thirdStep();
			hierarchicalHandler->processAStarInGraph();
		}


		//std::cout << "Path cost (walk): " << aStar->getPathCost() << std::endl;
	}

	// update internal mouse position
	m_mouseX = x;
	m_mouseY = y;
}

//----------------------------------------------------------------------------//
// Handle a mouse button up event                                             //
//----------------------------------------------------------------------------//

void Demo::onMouseButtonUp(int button, int x, int y)
{
	// update mouse button states
	if (button == GLUT_LEFT_BUTTON)
	{
		m_bLeftMouseButtonDown = false;
	}

	if (button == GLUT_MIDDLE_BUTTON)
	{
		m_bMiddleMouseButtonDown = false;
	}

	if(button == GLUT_RIGHT_BUTTON)
	{
		m_bRightMouseButtonDown = false;
	}

	// update internal mouse position
	m_mouseX = x;
	m_mouseY = y;
}

//----------------------------------------------------------------------------//
// Handle a mouse move event                                                  //
//----------------------------------------------------------------------------//

void Demo::onMouseMove(int x, int y)
{	
	mouseCoordWorld = pick.getWorldSpace(x, y);

	mouseCoordWorld[0] *= grid.getResolution();
	mouseCoordWorld[1] *= grid.getResolution();

	// update twist/tilt angles
	if (m_bLeftMouseButtonDown)
	{
		mapAsObstacleCell(mouseCoordWorld[0], mouseCoordWorld[1]);
	}

	// update distance
	if (m_bRightMouseButtonDown)
	{
	}

	if (m_bMiddleMouseButtonDown)
	{
		mapAsFreeCell(mouseCoordWorld[0], mouseCoordWorld[1]);
	}

	// update internal mouse position
	m_mouseX = x;
	m_mouseY = y;
}

//----------------------------------------------------------------------------//
// Put the background texture											      //
//----------------------------------------------------------------------------//
void Demo::putBackgroundTexture()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (GLdouble)m_width, 0, (GLdouble)m_height, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Setting the texture
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(0.0f, 0.0f);

			glTexCoord2f(1.0f,1.0f);
			glVertex2f((GLdouble)m_width, 0.0f);
			
			glTexCoord2f(1.0f,0.0f);
			glVertex2f((GLdouble)m_width, (GLdouble)m_height);
			
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(0.0f, (GLdouble)m_height);
		glEnd();

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

//----------------------------------------------------------------------------//
// Render the current FPS                                                   //
//----------------------------------------------------------------------------//
void Demo::renderFPS()
{
	// set the projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(grid.getXMin(), grid.getXMax()+1, grid.getYMin(), grid.getYMax()+1);

	// set the model transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// render the fps counter
	glTranslatef(1.0f, 1.0f, 0.0f);
	std::ostringstream buffer;
	buffer << "fps: " << m_fps;
	std::string str = buffer.str();

	renderString(const_cast<char*>(str.c_str()), textColor, GLUT_BITMAP_HELVETICA_18);

	// increase frame counter
	m_fpsFrames++;
}


//----------------------------------------------------------------------------//
// Render the current scene                                                   //
//----------------------------------------------------------------------------//
void Demo::onRender()
{
	// clear all the buffers
	glClearColor(0.0f, 0.0f, 0.2f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	putBackgroundTexture();

	// set the projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(grid.getXMin(), grid.getXMax()+1, grid.getYMin(), grid.getYMax()+1);

	// set the model transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	grid.draw(grid.getXMin(), grid.getYMin(), m_cellContour);
	clusterHandler->draw(grid.getXMin(), grid.getYMin(), m_drawCluster);
	hierarchicalHandler->draw(grid.getXMin(), grid.getYMin(), m_drawGraph);
	aStar->draw(grid.getXMin(), grid.getYMin());
	
	if ((m_mode == 0) || (clusterHandler->getQuantity() == 1))
	{
		aStar->drawPath(grid.getXMin(), grid.getYMin());
	} 
	else if (m_mode == 1)
	{
		hierarchicalHandler->drawFinalPath(grid.getXMin(), grid.getYMin());
	}
	

	renderFPS();

	glutSwapBuffers();
}

//----------------------------------------------------------------------------//
// Shut the demo down                                                         //
//----------------------------------------------------------------------------//

void Demo::onShutdown()
{
}


//----------------------------------------------------------------------------//
// Set the dimension                                                          //
//----------------------------------------------------------------------------//
void Demo::setDimension(int width, int height)
{
	// store new width and height values
	m_prevWidth		= m_width;
	m_prevHeight	= m_height;

	m_width		= width;
	m_height	= height; 

	// set new viewport dimension
	glViewport(3, 4, m_width, m_height);
}


//----------------------------------------------------------------------------//
void Demo::runAStar(int si, int sj, int ei, int ej)
{
	aStar->run(	si, 
				sj, 
				ei, 
				ej);	
	
	//std::cout << "PathCost: " << aStar->getPathCost() << std::endl;


	//aStar->setStartPosition(m_goal);
	//aStar->reset();

	glutPostRedisplay();
}


//----------------------------------------------------------------------------//
void Demo::findPath()
{	
	runAStar(	aStar->getStartPosition().first, 
				aStar->getStartPosition().second, 
				aStar->getEndPosition().first, 
				aStar->getEndPosition().second);
}