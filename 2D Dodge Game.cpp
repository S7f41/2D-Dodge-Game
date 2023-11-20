#include<windows.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#define PI 3.1416

struct Point
{
    float x, y;
};


int boardLength = 160;
int boardLeft = 20;
int boardRight = 180;
int boardBottom = 5;
int boardTop = 165;

// Agent 1
struct Point centerA1 = {40, 150};
float radiusA1 = 3;

// Agent 2
struct Point centerA2 = {30, 150};
float radiusA2 = 3;

// Enemy 1
struct Point centerE1 = {100, 162};
float radiusE1 = 5;

// Enemy 2
struct Point centerE2 = {100, 162};
float radiusE2 = 5;

float velocity = 0.02;

bool isAgentDestroyed = false;
bool isGameFinished = false;
bool isScreenFlash = false;

int score = 0;
int enemy1State = 0;
int enemy2State = 0;

float agentColorR = 0.0;
float agentColorG = 1.0;
float agentColorB = 0.0;

void drawCircleWithPolygon(float cx, float cy, float radius, int slices, float r, float g, float b)
{
    int i;
    float angle, x, y;
    glColor3f(r, g, b);
    glBegin(GL_POLYGON);
    {
        for (i = 0; i < slices; i++)
        {
            angle = ((2 * PI) / slices) * i;
            x = cx + radius * cos(angle);
            y = cy + radius * sin(angle);
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);


    if (isScreenFlash)
    {
        glClearColor(1.0, 0.0, 0.0, 1.0);
        isScreenFlash = false;
    }
    else if (isAgentDestroyed || isGameFinished)
    {
        glClearColor(1.0, 0.0, 0.0, 1.0);
    }
    else
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
    }

    glBegin(GL_LINES);
    {
        glColor3f(1, 1, 1);
        glVertex2d(-25, 0);
        glVertex2d(25, 0);

        glVertex2d(0, -25);
        glVertex2d(0, 25);
    }
    glEnd();


    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    {
        glVertex2d(boardLeft, boardTop);
        glVertex2d(boardRight, boardTop);
        glVertex2d(boardRight, boardBottom);
        glVertex2d(boardLeft, boardBottom);
    }
    glEnd();


    drawCircleWithPolygon(centerE1.x, centerE1.y, radiusE1, 40, 1, 0, 0);
    drawCircleWithPolygon(centerE2.x, centerE2.y, radiusE2, 40, 1, 1, 0);


    if (!isAgentDestroyed)
    {
        drawCircleWithPolygon(centerA1.x, centerA1.y, radiusA1, 40, agentColorR, agentColorG, agentColorB);
    }
    if (!isAgentDestroyed)
    {
        drawCircleWithPolygon(centerA2.x, centerA2.y, radiusA2, 40, agentColorR, agentColorG, agentColorB);
    }

    glFlush();
}

void animate()
{
    if (!isGameFinished)
    {
        score++;

        float factor;

        if (enemy1State == 0)
        {
            factor = 1;
            centerE1.y -= velocity;

            if (centerE1.x < boardBottom - radiusE1 || centerE1.y < (boardBottom + radiusE1))
            {
                enemy1State = 1;
            }
        }
        else if (enemy1State == 1)
        {
            factor = 1;
            centerE1.x -= velocity;
            centerE1.y += factor * velocity;

            if (centerE1.x < (boardLeft + radiusE1) || centerE1.y > (boardBottom + boardTop) / 2)
            {
                enemy1State = 2;
            }
        }
        else if (enemy1State == 2)
        {
            factor = 1;
            centerE1.x += velocity;
            centerE1.y -= 0.5 * velocity;

            if (centerE1.x > (boardRight - radiusE1))
            {
                enemy1State = 3;
            }
        }
        else if (enemy1State == 3)
        {
            factor = 1;
            centerE1.x -= velocity;
            centerE1.y += 2 * velocity;

            if (centerE1.y > boardTop - radiusE1)
            {
                enemy1State = 0;
            }
        }


        if (enemy2State == 0)
        {
            factor = 1;
            centerE2.x += velocity;
            centerE2.y -= factor * velocity;

            if (centerE2.x > boardRight - radiusE2 || centerE2.y < (boardBottom + boardTop) / 2)
            {
                enemy2State = 1;
            }
        }
        else if (enemy2State == 1)
        {
            factor = 1;
            centerE2.x -= velocity;

            if (centerE2.x < (boardLeft + boardRight) / 2)
            {
                enemy2State = 2;
            }
        }
        else if (enemy2State == 2)
        {
            factor = 1;
            centerE2.x -= velocity;
            centerE2.y -= factor * velocity;

            if (centerE2.x < (boardLeft - radiusE2) || centerE2.y < (boardBottom + radiusE2))
            {
                enemy2State = 3;
            }
        }
        else if (enemy2State == 3)
        {
            factor = 1;
            centerE2.x += velocity;
            centerE2.y += 2 * velocity;

            if (centerE2.y > boardTop - radiusE2)
            {
                enemy2State = 0;
            }
        }


        double distA1E1 = sqrt(pow((centerE1.x - centerA1.x), 2) + pow((centerE1.y - centerA1.y), 2));
        double distA1E2 = sqrt(pow((centerE2.x - centerA1.x), 2) + pow((centerE2.y - centerA1.y), 2));
        double distA2E1 = sqrt(pow((centerE1.x - centerA2.x), 2) + pow((centerE1.y - centerA2.y), 2));
        double distA2E2 = sqrt(pow((centerE2.x - centerA2.x), 2) + pow((centerE2.y - centerA2.y), 2));

        if (distA1E1 <= radiusA1 + radiusE1 || distA1E2 <= radiusA1 + radiusE2 || distA2E1 <= radiusA2 + radiusE1 || distA2E2 <= radiusA2 + radiusE2)
        {
            isAgentDestroyed = true;
            isGameFinished = true;
            isScreenFlash = true;
        }
    }
    else
    {
        printf("Score : %d\n", score);
        exit(0);
    }

    glutPostRedisplay();
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glOrtho(-2.0, 200.0, -2.0, 200.0, -25.0, 25.0);
}

void keyboard_action(unsigned char key, int x, int y)
{
    if (!isGameFinished)
    {

        if (key == 'l' && centerA2.x > boardLeft + radiusA2)
        {
            centerA2.x -= 1;
        }
        else if (key == 'r' && centerA2.x < boardRight - radiusA2)
        {
            centerA2.x += 1;
        }
        else if (key == 'u' && centerA2.y < boardTop - radiusA2)
        {
            centerA2.y += 1;
        }
        else if (key == 'd' && centerA2.y > boardBottom + radiusA2)
        {
            centerA2.y -= 1;
        }
    }

    glutPostRedisplay();
}

void special_action(int key, int x, int y)
{
    if (!isGameFinished)
    {

        if (key == GLUT_KEY_LEFT && centerA1.x > boardLeft + radiusA1)
        {
            centerA1.x -= 1;
        }
        else if (key == GLUT_KEY_RIGHT && centerA1.x < boardRight - radiusA1)
        {
            centerA1.x += 1;
        }
        else if (key == GLUT_KEY_UP && centerA1.y < boardTop - radiusA1)
        {
            centerA1.y += 1;
        }
        else if (key == GLUT_KEY_DOWN && centerA1.y > boardBottom + radiusA1)
        {
            centerA1.y -= 1;
        }
    }

    glutPostRedisplay();
}

void menu(int value)
{
    switch (value)
    {
    case 1:
        agentColorR = 1.0;
        agentColorG = 0.0;
        agentColorB = 0.0;
        break;
    case 2:
        agentColorR = 0.0;
        agentColorG = 1.0;
        agentColorB = 0.0;
        break;
    case 3:
        agentColorR = 0.0;
        agentColorG = 0.0;
        agentColorB = 1.0;
        break;
    }
    glutPostRedisplay();
}

void createMenu()
{
    int colorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);

    glutCreateMenu(menu);
    glutAddSubMenu("Agent Color", colorMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Objects");
    init();
    createMenu();
    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboard_action);
    glutSpecialFunc(special_action);
    glutMainLoop();
    return 0;
}
