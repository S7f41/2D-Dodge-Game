#include <windows.h>
#include <GL/glut.h>
#include <math.h>

#define pi (2 * acos(0.0))

struct Point3D
{
    double x, y, z;
};

Point3D eye = {100, 100, 100};
Point3D pos = {1, 15, 1};
Point3D ballPos = {1, 20, 1};
Point3D goalPostPos = {0, 100, 0};

double time = 0.0;
double u = 30;
double A = 60 * pi / 180;
double g = 9.8;
double B = 20 * pi / 180;
double T = 2 * u * sin(A * pi / 180) / g;
double R = u * u * sin(2 * A) / g;

double angle = 0.0;

bool ballThrown = false;

void drawAxes()
{
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(1000, 0, 0);
        glVertex3f(-1000, 0, 0);

        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0, -1000, 0);
        glVertex3f(0, 1000, 0);

        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0, 0, 1000);
        glVertex3f(0, 0, -1000);
    }
    glEnd();
}

void drawGrid()
{
    int i;

    glColor3f(0.6, 0.6, 0.6); // grey
    glBegin(GL_LINES);
    {
        for (i = -8; i <= 8; i++)
        {

            if (i == 0)
                continue;

            glVertex3f(i * 10, -90, 0);
            glVertex3f(i * 10, 90, 0);

            glVertex3f(-90, i * 10, 0);
            glVertex3f(90, i * 10, 0);
        }
    }
    glEnd();
}

void drawCylinder(double radius, double height, int slices)
{
    int i;
    double angle, angle2;

    glColor3f(0.8, 0.2, 0.2); // Red
    glBegin(GL_POLYGON);
    {
        for (i = 0; i < slices; i++)
        {
            angle = (2 * pi) * ((double)i / slices);
            glVertex3d(radius * cos(angle), radius * sin(angle), 0);
        }
    }
    glEnd();

    glColor3f(0.2, 0.8, 0.2); // Green
    glBegin(GL_POLYGON);
    {
        for (i = 0; i < slices; i++)
        {
            angle = (2 * pi) * ((double)i / slices);
            glVertex3d(radius * cos(angle), radius * sin(angle), height);
        }
    }
    glEnd();

    glBegin(GL_QUADS);
    {
        for (i = 0; i < slices; i++)
        {
            if (i % 2 == 0)
            {
                glColor3f(0, 0.8, 0.8); // Cyan
            }
            else
            {
                glColor3f(0.8, 0.8, 0); // Yellow
            }
            angle = (2 * pi) / ((double)i / slices);
            angle2 = (2 * pi) / ((double)(i + 1) / slices);

            glVertex3d(radius * cos(angle), radius * sin(angle), 0);
            glVertex3d(radius * cos(angle2), radius * sin(angle2), 0);
            glVertex3d(radius * cos(angle2), radius * sin(angle2), height);
            glVertex3d(radius * cos(angle), radius * sin(angle), height);
        }
    }
    glEnd();
}

void drawCone(double radius, double height, int slices)
{
    int i;
    double angle, angle2;

    glColor3f(0.8, 0.2, 0.8); // Magenta
    glBegin(GL_POLYGON);
    {
        for (i = 0; i < slices; i++)
        {
            angle = (2 * pi) * ((double)i / slices);
            glVertex3d(radius * cos(angle), radius * sin(angle), 0);
        }
    }
    glEnd();

    glColor3f(0.2, 1.0, 0); // Green
    glBegin(GL_TRIANGLES);
    {
        for (i = 0; i < slices; i++)
        {
            if (i % 2 == 0)
            {
                glColor3f(0, 0.2, 1); // Blue
            }
            else
            {
                glColor3f(0.2, 1, 0); // Lime
            }
            angle = (2 * pi) / ((double)i / slices);
            angle2 = (2 * pi) / ((double)(i + 1) / slices);

            glVertex3d(radius * cos(angle), radius * sin(angle), 0);
            glVertex3d(radius * cos(angle2), radius * sin(angle2), 0);
            glVertex3d(0, 0, height);
        }
    }
    glEnd();
}

void drawSphere(double radius, int stacks, int slices)
{
    int i, j;
    double angle1, angle2;

    Point3D points[stacks + 1][slices + 1];

    for (i = 0; i <= stacks; i++)
    {
        angle1 = (pi / 2) * ((double)i / stacks);
        for (j = 0; j <= slices; j++)
        {
            angle2 = (2 * pi) * ((double)j / slices);
            points[i][j].x = radius * cos(angle1) * cos(angle2);
            points[i][j].y = radius * cos(angle1) * sin(angle2);
            points[i][j].z = radius * sin(angle1);
        }
    }

    glBegin(GL_QUADS);
    {
        for (i = 0; i < stacks; i++)
        {
            for (j = 0; j < slices; j++)
            {
                if (j % 2 == 0)
                {
                    glColor3f(0, 0, 1); // Blue
                }
                else
                {
                    glColor3f(0, 1, 1); // Cyan
                }

                glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3d(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3d(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);

                glVertex3d(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3d(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                glVertex3d(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                glVertex3d(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
            }
        }
    }
    glEnd();
}

void drawPlayer()
{
    glPushMatrix();
    {

        glPushMatrix();
        {
            glTranslatef(0, 0, 58);
            drawSphere(7, 20, 20);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0, 0, 46);
            drawCone(5, 7, 20);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(0, 0, 15);
            drawCylinder(10, 30, 20);
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-16, 0, 30);
            glRotatef(55, 0, 1, 1); // Rotate Left Arm
            drawCylinder(2, 15, 20); // Left Arm
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(16, 0, 30);
            glRotatef(-55, 0, 1, 1); // Rotate Right Arm
            drawCylinder(2, 15, 20); // Right Arm
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-5, 0, 0);
            drawCylinder(2, 15, 20); // Left Leg
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(5, 0, 0);
            drawCylinder(2, 15, 20); // Right Leg
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(-17, 50, 0);
            drawCylinder(2, 15, 20); // Left bar
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(17, 50, 0);
            drawCylinder(2, 15, 20); // Right bar
        }
        glPopMatrix();

        glPushMatrix();
        {
            glTranslatef(18, 50, 15);
            glRotatef(90, 0, 1, 0);
            glRotatef(180, 0, 1, 0); // Rotate the upbar
            drawCylinder(2, 36, 20);
        }
        glPopMatrix();

    }
    glPopMatrix();
}

void drawBall()
{
    glPushMatrix();
    {
        glTranslated(ballPos.x, ballPos.y, ballPos.z);
        drawSphere(7, 50, 50);
    }
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, 0, 0, 0, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    drawAxes();
    drawGrid();

    drawPlayer();

    drawBall();

    glutSwapBuffers();
}

void animate()
{
    angle += 0.1;

    if (ballThrown)
    {
        time += 0.005;
        double t = time;

        // New trajectory calculation
        double distanceToGoal = sqrt(pow(goalPostPos.x - ballPos.x, 2) + pow(goalPostPos.y - ballPos.y, 2));
        double angleToGoal = atan2(goalPostPos.y - ballPos.y, goalPostPos.x - ballPos.x);

        double x = u * cos(A) * cos(angleToGoal) * t;
        double y = u * sin(A) * t - 0.5 * g * t * t;
        double z = u * cos(A) * sin(angleToGoal) * t;

        // Check if ball has reached near the goalpost
        if (sqrt(pow(x - goalPostPos.x, 2) + pow(z - goalPostPos.y, 2)) < 5) // 5 is the threshold distance
        {
            ballThrown = false;
            time = 0;
        }

        ballPos.x = x;
        ballPos.y = y;
        ballPos.z = z;
    }

    glutPostRedisplay();
}

void throwBall()
{
    if (!ballThrown)
    {
        ballThrown = true;
        time = 0;
    }
}

void init()
{
    glClearColor(0, 0, 0, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80, 1, 1, 5000.0);
}

void keyboard_action(unsigned char key, int x, int y)
{
    if (key == '1')
    {
        eye.x += 0.5;
    }
    else if (key == '2')
    {
        eye.x -= 0.5;
    }
    else if (key == '3')
    {
        eye.y += 0.5;
    }
    else if (key == '4')
    {
        eye.y -= 0.5;
    }
    else if (key == '5')
    {
        eye.z += 0.5;
    }
    else if (key == '6')
    {
        eye.z -= 0.5;
    }
    else if (key == ' ')
    {
        throwBall();
    }

    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow("Football_Player");

    init();

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutIdleFunc(animate);
    glutKeyboardFunc(keyboard_action);

    glutMainLoop();

    return 0;
}

