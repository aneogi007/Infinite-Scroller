#include <iostream>
#include <memory>
#include <filesystem>
#include <algorithm>
#include "glstate.hpp"
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>

namespace fs = std::filesystem;

// START SCREEN  - DONE
// END SCREEN    - DONE
// HIGH SCORE    - DONE
// TODO: CHANGE COLOR OF THE CAR (customization)

// Menu identifiers
const int MENU_EXIT = 1;					// Exit application
std::vector<std::string> meshFilenames;		// Paths to .obj files to load

// OpenGL state
std::unique_ptr<GLState> glState;

// Initialization functions
void initGLUT(int* argc, char** argv);
void initMenu();

// Callback functions
void display();
void reshape(GLint width, GLint height);
void keyPress(unsigned char key, int x, int y);
void keyRelease(unsigned char key, int x, int y);
void idle();
void menu(int cmd);
void cleanup();

// Program entry point
int main(int argc, char** argv) {
	try {
		// Create the window and menu
		initGLUT(&argc, argv);
		initMenu();
		// Initialize OpenGL (buffers, shaders, etc.)
		glState = std::unique_ptr<GLState>(new GLState());
		glState->initializeGL();
		glState->parseLights();

	} catch (const std::exception& e) {
		// Handle any errors
		std::cerr << "Fatal error: " << e.what() << std::endl;
		cleanup();
		return -1;
	}

	// Execute main loop
	glutMainLoop();

	return 0;
}

// Setup window and callbacks
void initGLUT(int* argc, char** argv) {
	// Set window and context settings
	//int width = 1920; int height = 1080;
	int width = 800; int height = 800;
	glutInit(argc, argv);
	glutInitWindowSize(width, height);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	// Create the window
	glutCreateWindow("Hyperspace");

	// Create a menu

	// GLUT callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutIdleFunc(idle);
	glutCloseFunc(cleanup);
}

void initMenu() {
	// Create the main menu, adding the objects menu as a submenu
	glutCreateMenu(menu);
	glutAddMenuEntry("Exit", MENU_EXIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Called whenever a screen redraw is requested
void display() {
	// Tell the GLState to render the scene
	glState->paintGL();

	// Scene is rendered to the back buffer, so swap the buffers to display it
	glutSwapBuffers();
}

// Called when the window is resized
void reshape(GLint width, GLint height) {
	// Tell OpenGL the new window size
	glState->resizeGL(width, height);
}

void keyPress(unsigned char key, int x, int y) {	// TODO
	auto ship = glState->scene->getShips()[glState-> getCurrentSpaceShip()];
	switch (key) {
	case 's':  // switch camera	
		//glState->switchCam();
		//glutPostRedisplay();	// Request redraw
		if(glState->getGameState() == glState->MENU) {
			glState->setGameState(glState->GAME);
			glState->initGameState();
		}
		else if (glState->getGameState() == glState->WELCOME) {
			glState->setGameState(glState->MENU);
		}
		break;
	case 13:
		if (glState->getGameState() == glState->MENU) {
			glState->setGameState(glState->GAME);
			glState->initGameState();
		}
		else if (glState->getGameState() == glState->WELCOME) {
			glState->setGameState(glState->MENU);
		}
		else if (glState->getGameState() == glState->OVER) {
			glState->setGameState(glState->GAME);
			glState->initGameState();

		}
		break;
	// case 'a':  // turn left
	// 	glState->getCamera(glState->getCamType()).turnLeft();
	// 	glutPostRedisplay();	// Request redraw
	// 	break;
	// case 'd':  // turn right
	// 	glState->getCamera(glState->getCamType()).turnRight();
	// 	glutPostRedisplay();	// Request redraw
	// 	break;
	// case 'w':  // move forward
	// 	glState->getCamera(glState->getCamType()).moveForward();
	// 	glutPostRedisplay();
	// 	break;
	// case 'x':  // move backward
	// 	glState->getCamera(glState->getCamType()).moveBackward();
	// 	glutPostRedisplay();
	// 	break;
	case 'c': {
		
		break;
	}

	case 'm': {
		glState->initMenuState();
		glState->setGameState(glState->MENU);
		break;
	}

	case 'a': {
		if (glState->getGameState() == glState->GAME) {
			ship->setLeftMove(true);
		} 
		break;
	}
	case 'd': {

		if (glState->getGameState() == glState->GAME) {
			ship->setRightMove(true);
			
		} else if (glState->getGameState() == glState->MENU) {
			if (glState->getCurrentSpaceShip() == 0) {
				glState->setCurrentSpaceShip(1);
			} else {
				glState->setCurrentSpaceShip(0);
			}
		}
		break;
	}
	default:
		break;
	}
}



// Called when a key is released
void keyRelease(unsigned char key, int x, int y) {
	auto ship = glState->scene->getShips()[glState-> getCurrentSpaceShip()];

	switch (key) {
	case 27:	// Escape key
		menu(MENU_EXIT);
		break;
	case 97:
	
		ship->setLeftMove(false);
		ship->setdriftMode(true);

		glState->setKeyPress(false);
		break;
	case 100:
		
		ship->setRightMove(false);
		ship->setdriftMode(true);

		glState->setKeyPress(false);
		break;
	}
}

// Called when there are no events to process
void idle() {
	// anything that happens every frame (e.g. movement) should be done here
	// Be sure to call glutPostRedisplay() if the screen needs to update as well
	glutPostRedisplay();
}

// Called when a menu button is pressed
void menu(int cmd) {
	switch (cmd) {
	// End the program
	case MENU_EXIT:
		glutLeaveMainLoop();
		break;
	}
}

// Called when the window is closed or the event loop is otherwise exited
void cleanup() {
	// Delete the GLState object, calling its destructor,
	// which releases the OpenGL objects
	glState.reset(nullptr);
}
