#include <GL/glut.h>
#include <iostream>
#include <cmath>

float rotationAngle = 0.0f;
bool rotating = false;
int rotatingAxis = -1; // 0: X-axis, 1: Y-axis, 2: Z-axis
int rotatingLayer = 0;

struct CubeRotation {
    float x, y, z;
};

CubeRotation cubeRotations[3][3][3];

void initGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 3; ++z)
                cubeRotations[x][y][z] = { 0.0f, 0.0f, 0.0f };
}

void drawCube(float x, float y, float z, CubeRotation rotation) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);
    float s = 0.5f;

    glColor3f(0.0f, 1.0f, 0.0f); // Green face
    glVertex3f(s, s, -s); glVertex3f(-s, s, -s); glVertex3f(-s, s, s); glVertex3f(s, s, s);

    glColor3f(1.0f, 0.5f, 0.0f); // Orange face
    glVertex3f(s, -s, s); glVertex3f(-s, -s, s); glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s);

    glColor3f(1.0f, 0.0f, 0.0f); // Red face
    glVertex3f(s, s, s); glVertex3f(-s, s, s); glVertex3f(-s, -s, s); glVertex3f(s, -s, s);

    glColor3f(1.0f, 1.0f, 0.0f); // Yellow face
    glVertex3f(s, -s, -s); glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s); glVertex3f(s, s, -s);

    glColor3f(0.0f, 0.0f, 1.0f); // Blue face
    glVertex3f(-s, s, s); glVertex3f(-s, s, -s); glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s);

    glColor3f(0.1f, 0.0f, 0.1f); // Purple face
    glVertex3f(s, s, -s); glVertex3f(s, s, s); glVertex3f(s, -s, s); glVertex3f(s, -s, -s);
    glEnd();

    glPopMatrix();
}

void updateCubeOrientation(int axis, int layer) {
    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                if ((axis == 0 && x == layer) ||
                    (axis == 1 && y == layer) ||
                    (axis == 2 && z == layer)) {

                    CubeRotation& rotation = cubeRotations[x][y][z];

                    switch (axis) {
                    case 0: rotation.x += 90.0f; break;
                    case 1: rotation.y += 90.0f; break;
                    case 2: rotation.z += 90.0f; break;
                    }
                }
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3.0, 3.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    float spacing = 1.1f;

    for (int x = 0; x < 3; ++x) {
        for (int y = 0; y < 3; ++y) {
            for (int z = 0; z < 3; ++z) {
                CubeRotation rotation = cubeRotations[x][y][z];

                if (rotating && ((rotatingAxis == 0 && x == rotatingLayer) ||
                    (rotatingAxis == 1 && y == rotatingLayer) ||
                    (rotatingAxis == 2 && z == rotatingLayer))) {

                    glPushMatrix();
                    if (rotatingAxis == 0) glRotatef(rotationAngle, 1.0f, 0.0f, 0.0f);
                    if (rotatingAxis == 1) glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
                    if (rotatingAxis == 2) glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);

                    glTranslatef((x - 1) * spacing, (y - 1) * spacing, (z - 1) * spacing);
                    drawCube(0.0f, 0.0f, 0.0f, rotation);
                    glPopMatrix();
                }
                else {
                    glPushMatrix();
                    glTranslatef((x - 1) * spacing, (y - 1) * spacing, (z - 1) * spacing);
                    drawCube(0.0f, 0.0f, 0.0f, rotation);
                    glPopMatrix();
                }
            }
        }
    }
    glutSwapBuffers();
}

void timer(int value) {
    if (rotating) {
        rotationAngle += 5.0f;
        if (rotationAngle >= 90.0f) {
            rotationAngle = 0.0f;
            rotating = false;
            updateCubeOrientation(rotatingAxis, rotatingLayer);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, .1f, 100.0f);
}

void keyboard(unsigned char key, int x, int y) {
    if (!rotating) {
        switch (key) {
        case '1': rotatingAxis = 0; rotatingLayer = 0; rotating = true; break; // X-axis left
        case '2': rotatingAxis = 0; rotatingLayer = 1; rotating = true; break; // X-axis middle
        case '3': rotatingAxis = 0; rotatingLayer = 2; rotating = true; break; // X-axis right
        case '4': rotatingAxis = 1; rotatingLayer = 0; rotating = true; break; // Y-axis bottom
        case '5': rotatingAxis = 1; rotatingLayer = 1; rotating = true; break; // Y-axis middle
        case '6': rotatingAxis = 1; rotatingLayer = 2; rotating = true; break; // Y-axis top
        case '7': rotatingAxis = 2; rotatingLayer = 0; rotating = true; break; // Z-axis front
        case '8': rotatingAxis = 2; rotatingLayer = 1; rotating = true; break; // Z-axis middle
        case '9': rotatingAxis = 2; rotatingLayer = 2; rotating = true; break; // Z-axis back
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Rubik's Cube");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    initGL();
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}