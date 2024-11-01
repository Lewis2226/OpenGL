#include <GL/glut.h>

float rotationAngle = .0f;  // �ngulo de rotaci�n para la cara
bool rotatingx = false;       // Bandera para indicar si estamos rotando
bool rotatingy = false;       // Bandera para indicar si estamos rotando
float depthOffset = -12.0f;  // Distancia inicial de los cubos para que sean visibles
int rotatingColumn = -1;
int rotatingRow = -1;

void initGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void drawCube(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z + depthOffset); 

    glBegin(GL_QUADS);
    // Top face (y = 1.0f)
    glColor3f(0.0f, 1.0f, 0.0f); 
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Resto de las caras
    glColor3f(1.0f, 0.5f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glColor3f(1.0f, 0.0f, 0.0f); 
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glColor3f(1.0f, 1.0f, 0.0f); 
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glColor3f(0.0f, 0.0f, 1.0f); 
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glColor3f(1.0f, 0.0f, 1.0f); 
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float spacing = 2.3f;

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                if (rotatingx && rotatingColumn == x) {
                    // Si la columna es la que est� rotando
                    glPushMatrix();
                    glTranslatef(x * spacing, 0.0f, 0.0f);
                    glRotatef(rotationAngle, 1.0f, 0.0f, 0.0f);
                    glTranslatef(-x * spacing, 0.0f, 0.0f);
                    drawCube(x * spacing, y * spacing, z * spacing);
                    glPopMatrix();
                }

                else if (rotatingy && rotatingRow == y) {
                    // Si la columna es la que est� rotando
                    glPushMatrix();
                    glTranslatef(0.0f, y * spacing, 0.0f);
                    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
                    glTranslatef(0.0f, -y * spacing, 0.0f);
                    drawCube(x * spacing, y * spacing, z * spacing);
                    glPopMatrix();
                }
                else {
                    drawCube(x * spacing, y * spacing, z * spacing);
                }
            }
        }
    }
    glutSwapBuffers();
}
void timer(int value) {
    if (rotatingx || rotatingy) {
        rotationAngle += 2.5f;
        if (rotationAngle >= 180.0f) {
            rotationAngle = 0.0f;
            rotatingx = false;
            rotatingy = false;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(30, timer, 0);
}

void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case '1': rotatingColumn = -1; rotatingx = true; break; // Columna izquierda
    case '2': rotatingColumn = 0; rotatingx = true; break;  // Columna central
    case '3': rotatingColumn = 1; rotatingx = true; break;  // Columna derecha
    case '4': rotatingRow = -1; rotatingy = true; break; // Columna izquierda
    case '5': rotatingRow = 0; rotatingy = true; break;  // Columna central
    case '6': rotatingRow = 1; rotatingy = true; break;  // Columna derecha
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
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