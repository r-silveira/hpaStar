/* -* c++ *- PathPlanning - 2007 Renato Silveira
 *	
 *	Demo.h
 *
 *
 *				created 27/06/2008
 */

#ifndef DEMO_H
#define DEMO_H

//----------------------------------------------------------------------------//
// Includes                                                                   //
//----------------------------------------------------------------------------//

#include "Global.h"
#include "Vec2f.h"
#include "Grid.h"
#include "PickHandler.h"
#include "AStar.h"
#include "ClusterHandler.h"
#include "Image.h"
#include "HierarchicalHandler.h"

class Demo
{
	protected:

		// member variables
		int		m_width;
		int		m_height;
		int		m_prevWidth;
		int		m_prevHeight;
		int		m_mouseX;
		int		m_mouseY;
		float	m_renderScale;
		bool	m_bFullscreen;
		bool	m_bLeftMouseButtonDown;
		bool	m_bRightMouseButtonDown;
		bool	m_bMiddleMouseButtonDown;
		bool	m_bPaused;
		bool	m_light;
		bool	m_cellContour;
		bool	m_drawCluster;
		bool	m_drawGraph;
		float	m_fpsDuration;
		int		m_fpsFrames;
		int		m_fps;
		int		m_gridSize;
		unsigned int m_lastTick;
		int		m_mode;
		
		std::string textureName;

		Vec3f	mouseCoordWorld;
		Grid		grid;
		PickHandler	pick;
		AStar*		aStar;
		ClusterHandler*	clusterHandler;
		HierarchicalHandler* hierarchicalHandler;

		std::pair<int,int> m_goal;
		std::pair<int,int> m_start;

		void mapAsObstacle(float x, float y);
		void drawAxis();

	public:
		
		// set the light position and attributes
		static const GLfloat lightPosition[];
		static const GLfloat lightColorAmbient[];
		static const GLfloat lightColorDiffuse[];
		static const GLfloat lightColorSpecular[];
		static const GLfloat textColor[];
		static const char*	 textureNames[];
		Image* oImage;

		// constructors/destructor
		Demo();
		virtual ~Demo();

		std::string getCaption();
		bool		getFullscreen();
		int			getHeight();
		int			getWidth();

		inline const char* getVecTextureName(int i) { return textureNames[i]; };
		inline std::string getTextureName() const { return textureName; };
		inline const bool drawCellContour() const { return m_cellContour; };
		inline const bool drawGraph() const { return m_drawGraph; };
		inline const int getMode() { return m_mode; };
	
		inline void setGridSize(int n) { m_gridSize = n; }; 
		inline void setMode(int m) { m_mode = m; }; 
		inline void setDrawCellContour(bool value) { m_cellContour = value; };
		inline void setDrawGraph(bool value) { m_drawGraph = value; };
		inline void setDrawCluster(bool value) { m_drawCluster = value; };
		inline void setShowFunctions(bool value) { grid.setShowFunctions(value); };
		inline void setTextureName(const std::string& name ) { textureName = name; };
		void setClusterQuantity(int n);
		void setEpsilon(int n);

		void initGrid(float _xMin, float _xMax, float _yMin, float _yMax, float r);
		void initGrid(float _size, float r);
		void mapAsObstacleCell(float x, float y);
		void mapAsFreeCell(float x, float y);
		void reset();
		void runAStar(int si, int sj, int ei, int ej);
		void findPath();
		void saveGridConfiguration(const std::string& fileName);
		void loadGridConfiguration(const std::string& fileName);

		void renderFPS();
		void initTexture();
		void putBackgroundTexture();
		void renderString(char *string, const float color[], void *font);
		bool onCreate(int argc, char *argv[]);
		void onIdle();
		bool onInit();
		void onKey(unsigned char key, int x, int y);
		void onMouseButtonDown(int button, int x, int y);
		void onMouseButtonUp(int button, int x, int y);
		void onMouseMove(int x, int y);
		void onRender();
		void onShutdown();
		
		void setDimension(int width, int height);
};

extern Demo theDemo;

#endif

//----------------------------------------------------------------------------//
