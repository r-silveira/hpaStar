//----------------------------------------------------------------------------//
// main.cpp                                                                   //															  //
//----------------------------------------------------------------------------//


#if defined(_MSC_VER) && _MSC_VER <= 0x0600
#pragma warning(disable : 4786)
#endif

#include "Demo.h"
#include "Log.h"
#include <fstream>

//-----------------------------------------------------------------------------
// Glui interface
GLUI_Checkbox*		checkbox;
GLUI_Checkbox*		checkbox2;
GLUI_Checkbox*		checkbox3;
GLUI_Checkbox*		checkbox4;

GLUI_Spinner*		spinnerRes;
GLUI_Spinner*		spinnerCluster;
GLUI_Spinner*		spinnerSize;
GLUI_Spinner*		spinnerEpsilon;

GLUI_RadioGroup*	radio;
GLUI_Panel*			objPanel;
GLUI_Listbox*		listbox;
GLUI_RadioGroup*	ot_group;

//-----------------------------------------------------------------------------
int		nCluster	= 1;
int		size		= 10; 
int		res			= 1;
int		cellContour = 1;
int		drawCluster	= 1;
int		drawGraph	= 1;
int		showFunctions = 0;
int		borders		= 0;
int		epsilon		= 4;
int		mainWindow;


//----------------------------------------------------------------------------//
// GLUI callback functions                                                    //
//----------------------------------------------------------------------------//
void control_cb( int control )
{	
	if (control == 1)
	{
		theDemo.initGrid(	spinnerSize->get_int_val(),
							spinnerRes->get_int_val());
	}
}


//----------------------------------------------------------------------------//
void radioButton_cb(int control )
{
	theDemo.setMode(ot_group->get_int_val());
}


//----------------------------------------------------------------------------//
void cluster_cb( int control )
{	
	theDemo.setClusterQuantity(spinnerCluster->get_int_val());
}

//----------------------------------------------------------------------------//
void epsilon_cb( int control )
{	
	theDemo.setEpsilon(spinnerEpsilon->get_int_val());
}


//-----------------------------------------------------------------------------
void findPath()
{	
	theDemo.findPath();
}


//-----------------------------------------------------------------------------
void displayFunc()
{
	// render the scene
	theDemo.onRender();
}


//-----------------------------------------------------------------------------
void exitFunc()
{
	// shut the demo instance down
	theDemo.onShutdown();
}

//-----------------------------------------------------------------------------

void idleFunc()
{
	// redirect to the demo instance
	theDemo.onIdle();
}

//-----------------------------------------------------------------------------

void keyboardFunc(unsigned char key, int x, int y)
{
	// redirect the message to the demo instance
	theDemo.onKey(key, x, theDemo.getHeight() - y - 1);
}

//-----------------------------------------------------------------------------

void motionFunc(int x, int y)
{
	// redirect the message to the demo instance
	theDemo.onMouseMove(x, theDemo.getHeight() - y - 1);
}

//-----------------------------------------------------------------------------

void mouseFunc(int button, int state, int x, int y)
{
	// redirect the message to the demo instance
	if(state == GLUT_DOWN)
	{
		theDemo.onMouseButtonDown(button, x, theDemo.getHeight() - y - 1);
	}
	else if(state == GLUT_UP)
	{
		theDemo.onMouseButtonUp(button, x, theDemo.getHeight() - y - 1);
	}
}

//-----------------------------------------------------------------------------
void reset()
{
	theDemo.reset();
}


//-----------------------------------------------------------------------------
void save()
{
	std::string name = theDemo.getTextureName() + ".txt";

	theDemo.saveGridConfiguration(name);
}


//-----------------------------------------------------------------------------
void load()
{
	std::string name(".\\textures\\");
	
	name += theDemo.getVecTextureName(listbox->get_int_val());

	theDemo.setTextureName(name);
	theDemo.initTexture();
	theDemo.reset();

	std::string fileName = theDemo.getTextureName() + ".txt";

	theDemo.loadGridConfiguration(fileName);
	theDemo.setClusterQuantity(spinnerCluster->get_int_val());
}


//-----------------------------------------------------------------------------
void checkContour( int control )
{
	theDemo.setDrawCellContour(cellContour);
}


//-----------------------------------------------------------------------------
void checkCluster( int control )
{
	theDemo.setDrawCluster(drawCluster);
}


//-----------------------------------------------------------------------------
void checkShowFunctions(int control )
{
	theDemo.setShowFunctions(showFunctions);
}

//-----------------------------------------------------------------------------
void checkDrawGraph(int control )
{
	theDemo.setDrawGraph(drawGraph);
}

//-----------------------------------------------------------------------------
void reshapeFunc(int width, int height)
{
	// set the new width/height values
	theDemo.setDimension(width, height);

    GLUI_Master.auto_set_viewport();
}

//-----------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	std::ofstream outfile("Log.txt");

	Log::setLogger(new LogStream(&outfile));


	// initialize the GLUT system
	glutInit(&argc, argv);

	// create our demo instance
	if(!theDemo.onCreate(argc, argv))
	{
		std::cerr << "Creation of the viewer failed." << std::endl;
		return -1;
	}

	// register our own exit callback
	atexit(exitFunc);

	// set all GLUT modes
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(theDemo.getWidth(), theDemo.getHeight());
	
	mainWindow = glutCreateWindow(theDemo.getCaption().c_str());
	
	if(theDemo.getFullscreen()) 
	{
		glutFullScreen();
	}
	
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutPassiveMotionFunc(motionFunc);
	glutDisplayFunc(displayFunc);
	glutKeyboardFunc(keyboardFunc);

	// GLUI code                          
	// Create the side subwindow
	GLUI* glui = GLUI_Master.create_glui_subwindow( mainWindow, GLUI_SUBWINDOW_RIGHT );

	new GLUI_StaticText( glui, "Viewer Options" );
	new GLUI_Separator( glui );
	
	listbox = glui->add_listbox("Map: ");
	listbox->add_item(0,"citySnow.jpg");
	listbox->add_item(1,"labyrinth.jpg");
	listbox->add_item(2,"city.jpg");
	listbox->add_item(3,"null.jpg");

	GLUI_Panel *obj_panel3 = new GLUI_Panel( glui, "" );
	new GLUI_Button( obj_panel3, "Reset", 0,(GLUI_Update_CB)reset );

	GLUI_Panel *obj_panel5 = new GLUI_Panel( glui, "" );
	new GLUI_Button( obj_panel5, "Save Grid", 0,(GLUI_Update_CB)save );
	new GLUI_Button( obj_panel5, "Load Grid", 0,(GLUI_Update_CB)load );


	GLUI_Panel *obj_panel = new GLUI_Panel( glui, "Grid Configurations" );

	spinnerSize = new GLUI_Spinner( obj_panel, "Size:", &size, 1, control_cb);
	spinnerSize->set_int_limits( 2, 5000 );
	spinnerSize->set_speed(0.0f);

	spinnerRes = new GLUI_Spinner( obj_panel, "Resolution:", &res, 1, control_cb);
	spinnerRes->set_int_limits( 1, 5000 );
	spinnerRes->set_speed(0.0f);

	new GLUI_Separator( obj_panel );
	checkbox = new GLUI_Checkbox( obj_panel, "Draw grid", &cellContour, 0, checkContour);
	checkbox3 = new GLUI_Checkbox( obj_panel, "Draw clusters", &drawCluster, 0, checkCluster);
	checkbox4 = new GLUI_Checkbox( obj_panel, "Draw graph", &drawGraph, 0, checkDrawGraph);
	checkbox2 = new GLUI_Checkbox( obj_panel, "Show functions", &showFunctions, 0, checkShowFunctions);


	//-------------------------------------------
	new GLUI_Separator( glui );
	GLUI_Panel *obj_panel6 = new GLUI_Panel( glui, "A* Configurations" );

	//  Id of the selected radio button
	int radiogroup_item_id = 0;

	//  Create radio button group
	ot_group = glui->add_radiogroup_to_panel (obj_panel6,
    &radiogroup_item_id, 1, radioButton_cb);

	//  Add the radio buttons to the radio group
	glui->add_radiobutton_to_group( ot_group, "A*" );
	glui->add_radiobutton_to_group( ot_group, "HPA*" );


	spinnerEpsilon = new GLUI_Spinner( obj_panel6, "epsilon:", &epsilon, 1, epsilon_cb);
	spinnerEpsilon->set_int_limits( 1, 15 );
	spinnerEpsilon->set_speed(0.0f);

	spinnerCluster = new GLUI_Spinner( obj_panel6, "Cluster:", &nCluster, 2, cluster_cb);
	spinnerCluster->set_int_limits( 1, 10 );
	spinnerCluster->set_speed(0.0f);

	//GLUI_Panel *obj_panel4 = new GLUI_Panel( glui, "" );
	//new GLUI_Button( obj_panel4, "Find Path", 0,(GLUI_Update_CB)findPath );

	new GLUI_Separator( glui );

	new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );
	
	glui->set_main_gfx_window( mainWindow );

	// We register the idle callback with GLUI, *not* with GLUT
	GLUI_Master.set_glutIdleFunc( idleFunc );
	GLUI_Master.set_glutReshapeFunc( reshapeFunc );

	// initialize our demo instance
	if(!theDemo.onInit())
	{
		std::cerr << "Initialization of the demo failed." << std::endl;
		return -1;
	}

	// run the GLUT loop
	glutMainLoop();

	return 0;
}
