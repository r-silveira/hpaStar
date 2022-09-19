#include "PickHandler.h"

#include "global.h"

//-----------------------------------------------------------------------------

PickHandler::PickHandler()
{

}

//-----------------------------------------------------------------------------

PickHandler::~PickHandler()
{

}
	
//-----------------------------------------------------------------------------

void PickHandler::init(Grid* g)
{
	grid = g;
}

//-----------------------------------------------------------------------------

// Draw only the depth, not draw the colors
void PickHandler::drawPlaneDepthOnly(float xi, float xf, float yi, float yf)
{
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);	
	glEnable(GL_DEPTH_TEST);
	
	glBegin(GL_QUADS);

		glVertex3i(xi, yi, 0.09f);
		glVertex3i(xf, yi, 0.09f);
		glVertex3i(xf, yf, 0.09f);
		glVertex3i(xi, yf, 0.09f);
	
	glEnd();

	glDisable(GL_DEPTH_TEST);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

//-----------------------------------------------------------------------------

void PickHandler::applyCamTransform()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluOrtho2D(grid->getXMin(), grid->getXMax()+1, grid->getYMin(), grid->getYMax()+1);

	// set the model transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//-----------------------------------------------------------------------------

Vec3f PickHandler::getWorldSpace(int x, int y)
{
	double modelMatrix[16];
	double projMatrix[16];
	double xObjectCoord;
	double yObjectCoord;
	double zObjectCoord;

	int windowWidth;
	int windowHeight;
	int viewport[4];
	int commandSize;
	int sceneryWidth;
	int sceneryHeight;
	int i;
	
	float zWindowCoord;

	glClear(GL_DEPTH_BUFFER_BIT);

	applyCamTransform();

	drawPlaneDepthOnly(grid->getXMin(), grid->getXMax()+1, grid->getYMin(), grid->getYMax()+1);

	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);

	windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	glGetIntegerv(GL_VIEWPORT, viewport);

	sceneryWidth	= (grid->getXMax()+1) - grid->getXMin();
	sceneryHeight	= (grid->getYMax()+1) - grid->getYMin();


	glReadPixels(x, (windowHeight - y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zWindowCoord);

	// Test point depth to assures that it is on the scenario
	if (zWindowCoord > (1.0f - 0.00001f))
	{
	}

	if (gluUnProject(x, (GLdouble)(windowHeight - y), (GLdouble)zWindowCoord, modelMatrix, projMatrix, viewport, &xObjectCoord, &yObjectCoord, &zObjectCoord) == GL_FALSE)
	{
	}

	return Vec3f(xObjectCoord-1, (grid->getYMax()+1)-yObjectCoord, 0.0f);
}