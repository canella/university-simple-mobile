/* OpenGL includes */
/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* OpenGL includes */
#include <GL/glew.h>
#include <GL/freeglut.h>

/* local includes */
#include "GlHelper.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "render/CubeRenderingObject.h"
#include "render/TriangleRenderingObject.h"
#include "render/GridRenderingObject.h"
#include "render/LineRenderingObject.h"
#include "render/OBJRenderingObject.h"

#ifndef __GLUT_PROGRAM_H__
#define __GLUT_PROGRAM_H__
class GlutProgram {
	private:
		static GlutProgram *instance;
		
		int width;
		int height;
		int posX;
		int posY;
		const char *title;
		ShaderProgram shaderProgram;
		Camera camera;
		LineRenderingObject line[7]; // lines for connecting all other elements
		GridRenderingObject grid[3]; // background grids
		// scene rendering objects =)
		CubeRenderingObject cube[2];
		TriangleRenderingObject triangle[2];
		OBJRenderingObject obj[1];
		

		// GLUT needs those static functions...
		static void onDisplayWrapper(void){
			instance->onDisplay();
		}
		static void onIdleWrapper(void) {
			instance->onIdle();
		}
		static void onResizeWrapper(int width, int height) {
			instance->onResize(width, height);
		}
		static void onKeyboardInputWrapper(unsigned char key, int x, int y) {
			instance->onKeyboardInput(key, x, y);
		}
		static void onSpecialInputWrapper(int key, int x, int y) {
			instance->onSpecialInput(key, x, y);
		}
		static void onMouseInputWrapper(int button, int state, int x, int y) {
			instance->onMouseInput(button, state, x, y);
		}
		
		void onDisplay(void);
		void onIdle(void);
		void onResize(int width, int height);
		void onKeyboardInput(unsigned char key, int x, int y);
		void onSpecialInput(int key, int x, int y);
		void onMouseInput(int button, int state, int x, int y);
		
		void initScene(void);
		
	public:
		static void setInstance(GlutProgram * glutProgram);
		
		void setDimensions(int width, int height);
		void setPosition(int x, int y);
		void setTitle(const char *title);
		void init(int *argc, char **argv);
		void run(void);
};
#endif // __SHADER_PROGRAM_H__