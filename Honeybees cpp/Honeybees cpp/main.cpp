// DONE Block movement when colliding with the border / lines.
// DONE Draw more hexagons within the borders.
// DONE Destroy hexagons on collision with the player.
// DONE Add an enemy sprite which moves across the screen and destroy player on collision.
// DONE Increase playersize by every destroyed hexagon.
// DONE Add a key to quit the program.
//Draw text to display playerScore.
//Reshape the player and enemy sprite.
// OPTIONAL Add bitmap sprite as player (bee).

#include <math.h>
#include <cstdlib>
#include <Windows.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>

#define PLAYERSIZE (0.2f + playerScore / 164.0f)
#define PLAYFIELDSIZE 4.0f
#define CAMERAMOVESPEED 0.1f
#define HEXAGONCOUNT 21
#define HEXAGONSIZE 0.2f

int windowID;
float playerScore = 0.0f;
float currentHexagonPos[HEXAGONCOUNT][2];
float currentEnemyPos[1][2];
float cameraPosition[2] = { 0,0 };

float randomFloat(float a, float b) {
   	float random = ((float)rand() / (float)RAND_MAX);
   	float diff = b - a;
   	float r = random * diff;
   	return a + r;
}

void drawHexagon(float x, float y, float size) {
   	//Set drawing color (RGBA)
   	glColor3d(0.957, 0.643, 0.376);
   	//Tell OpenGL your intentions
   	glBegin(GL_POLYGON);
   	//Set the vertices glVertex2d(x, y);
   	glVertex2d(-0.5 * size + x, 0.5 * size + y);
   	glVertex2d(-0.5 * size + x, -0.5 * size + y);
   	glVertex2d(0 * size + x, -0.8 * size + y);
   	glVertex2d(0.5 * size + x, -0.5 * size + y);
   	glVertex2d(0.5 * size + x, 0.5 * size + y);
   	glVertex2d(0 * size + x, 0.8 * size + y);
	glEnd();
   	//Done making polygon
}

void drawEnemy(float x, float y, float size) {
	//Set drawing color (RGBA)
	glColor3d(0.957, 0.643, 0.376);
	//Tell OpenGL your intentions
	glBegin(GL_POLYGON);
	//Set the vertices glVertex2d(x, y);
	glVertex2d(-0.5 * size + x, 0.5 * size + y);
	glVertex2d(-0.5 * size + x, -0.5 * size + y);
	glVertex2d(0 * size + x, -0.8 * size + y);
	glVertex2d(0.5 * size + x, -0.5 * size + y);
	glVertex2d(0.5 * size + x, 0.5 * size + y);
	glVertex2d(0 * size + x, 0.8 * size + y);
	glEnd();
	//Done making polygon
}

void drawBorders() {
	glColor3d(0.957, 0.643, 0.376);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-PLAYFIELDSIZE, PLAYFIELDSIZE);
	glVertex2f(PLAYFIELDSIZE, PLAYFIELDSIZE);
	glVertex2f(PLAYFIELDSIZE, -PLAYFIELDSIZE);
	glVertex2f(-PLAYFIELDSIZE, -PLAYFIELDSIZE);
	glEnd();
}
   
void setupHexagons() {
   	for (int hexagonCount = 0; hexagonCount < HEXAGONCOUNT; hexagonCount++)
   	{
   		currentHexagonPos[hexagonCount][0] = randomFloat(-PLAYFIELDSIZE + HEXAGONSIZE, PLAYFIELDSIZE - HEXAGONSIZE);
   		currentHexagonPos[hexagonCount][1] = randomFloat(-PLAYFIELDSIZE + HEXAGONSIZE, PLAYFIELDSIZE - HEXAGONSIZE);
   	}
}

void setupEnemy() {
	currentEnemyPos[0][0] = randomFloat(-PLAYFIELDSIZE + 0.3f, PLAYFIELDSIZE - 0.3f);
	currentEnemyPos[0][1] = randomFloat(-PLAYFIELDSIZE + 0.3f, PLAYFIELDSIZE - 0.3f);
}

bool doesCollide(float item1[], float item2[], float size) {
   	float diffX = item1[0] - item2[0];
   	float diffY = item1[1] - item2[1];
   	float distance = sqrtf((diffX * diffX) + (diffY * diffY));
   	return distance < size;
}

void updateHexagons() {
   	for (int hexagonOuter = 0; hexagonOuter < HEXAGONCOUNT - 1; hexagonOuter++) {
   		for (int hexagonInner = hexagonOuter + 1; hexagonInner < HEXAGONCOUNT; hexagonInner++) {
   			if (doesCollide(currentHexagonPos[hexagonOuter], currentHexagonPos[hexagonInner], HEXAGONSIZE)) {
				currentHexagonPos[hexagonInner][0] = randomFloat(-PLAYFIELDSIZE + HEXAGONSIZE, PLAYFIELDSIZE - HEXAGONSIZE);
				currentHexagonPos[hexagonInner][1] = randomFloat(-PLAYFIELDSIZE + HEXAGONSIZE, PLAYFIELDSIZE - HEXAGONSIZE);
   			}
   		}
   	}
}

void checkPlayerCollision() {
	for (int hexagonIndex = 0; hexagonIndex < HEXAGONCOUNT; hexagonIndex++) {
		if (doesCollide(cameraPosition, currentHexagonPos[hexagonIndex], (HEXAGONSIZE + PLAYERSIZE) / 2)) {
			currentHexagonPos[hexagonIndex][0] = randomFloat(-PLAYFIELDSIZE + HEXAGONSIZE, PLAYFIELDSIZE - HEXAGONSIZE);
			currentHexagonPos[hexagonIndex][1] = randomFloat(-PLAYFIELDSIZE + HEXAGONSIZE, PLAYFIELDSIZE - HEXAGONSIZE);
			playerScore++;
		}
	}
	if (doesCollide(cameraPosition, currentEnemyPos[0], (0.3f + PLAYERSIZE) / 2)) {
		currentEnemyPos[0][0] = randomFloat(-PLAYFIELDSIZE + 0.3f, PLAYFIELDSIZE - 0.3f);
		currentEnemyPos[0][1] = randomFloat(-PLAYFIELDSIZE + 0.3f, PLAYFIELDSIZE - 0.3f);
		playerScore++;
	}
}
  
void drawPlayer(float x, float y) {
	 glBegin(GL_POLYGON);
	 glVertex2d(x - (PLAYERSIZE / 2), y + (PLAYERSIZE / 2));
	 glVertex2d(x + (PLAYERSIZE / 2), y + (PLAYERSIZE / 2));
	 glVertex2d(x + (PLAYERSIZE / 2), y - (PLAYERSIZE / 2));
	 glVertex2d(x - (PLAYERSIZE / 2), y - (PLAYERSIZE / 2));
	 glEnd();
}

void moveEnemy() {
	//move enemy on x and y axis between 1 and -1 (needs to be increased)
	currentEnemyPos[0][0] = (currentEnemyPos[0][0] > 1) ? -1 : currentEnemyPos[0][0] + 0.1;
	currentEnemyPos[0][1] = (currentEnemyPos[0][1] > 1) ? -1 : currentEnemyPos[0][1] + 0.05;
}

void display(void) {
	glLoadIdentity();
	gluLookAt(cameraPosition[0], cameraPosition[1], -1, cameraPosition[0], cameraPosition[1], 0, 0.0, 1.0, 0.0);
   	updateHexagons();
	checkPlayerCollision();
	//Set background color (RGBA)
   	glClearColor(0.565, 0.933, 0.565, 0);
   	glClear(GL_COLOR_BUFFER_BIT);
   	//draw the honey (x,y,size)
   	for (int hexagonCount = 0; hexagonCount < HEXAGONCOUNT; hexagonCount++) {
   		drawHexagon(currentHexagonPos[hexagonCount][0], currentHexagonPos[hexagonCount][1], HEXAGONSIZE);
   	}
	drawEnemy(currentEnemyPos[0][0], currentEnemyPos[0][1], 0.3f);
	drawBorders();
	drawPlayer(cameraPosition[0], cameraPosition[1]);
	//Clear screen and draw
   	glutSwapBuffers();
	glutPostRedisplay();
}
   
void keyboardFunc(unsigned char key, int x, int y) {
	 switch (key)
	 {
		case 'a':
			 cameraPosition[0] += CAMERAMOVESPEED;
			 moveEnemy();
			 break;
		case 'd':
			 cameraPosition[0] -= CAMERAMOVESPEED;
			 moveEnemy();
			 break;
		case 'w':
			 cameraPosition[1] += CAMERAMOVESPEED;
			 moveEnemy();
			 break;
		case 's':
			 cameraPosition[1] -= CAMERAMOVESPEED;
			 moveEnemy();
			 break;
		case 27:
			// ESC pressed, exit the application
			glutDestroyWindow(windowID);
			exit(0);
			break;
		default:
			 break;
	 }
	 // if (cameraPositon[0] > PLAYFIELDSIZE) { cameraPosition[0] = PLAYFIELDSIZE; }
	 //Keep the player inside the drawBorders / boundary.
	 float halfPlayerSize = PLAYERSIZE / 2.0f;
	 cameraPosition[0] = (cameraPosition[0] > PLAYFIELDSIZE - halfPlayerSize) ? PLAYFIELDSIZE - halfPlayerSize : cameraPosition[0];
	 cameraPosition[0] = (cameraPosition[0] < -PLAYFIELDSIZE + halfPlayerSize) ? -PLAYFIELDSIZE + halfPlayerSize : cameraPosition[0];
	 cameraPosition[1] = (cameraPosition[1] > PLAYFIELDSIZE - halfPlayerSize) ? PLAYFIELDSIZE - halfPlayerSize : cameraPosition[1];
	 cameraPosition[1] = (cameraPosition[1] < -PLAYFIELDSIZE + halfPlayerSize) ? -PLAYFIELDSIZE + halfPlayerSize : cameraPosition[1];
}

int main(int argc, char **argv) {
   	//pre-configure  game layout
   	setupHexagons();
	setupEnemy();
   	//Initialize GLUT
   	glutInit(&argc, argv);
   	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
   	//Window Creation
   	glutInitWindowSize(1280, 720);
   	glutInitWindowPosition(0, 0);
   	char s[4096] = "Hello world";
	windowID = glutCreateWindow(s);
   	//When the display needs redrawing
   	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardFunc);
   	//Continue until user quits
   	glutMainLoop();
   	return 0;
}