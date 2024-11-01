#include <GL/glut.h>

float rotationAngle = 45.0f;  // �ngulo de rotaci�n para la cara
bool rotating = false;       // Bandera para indicar si estamos rotando
float depthOffset = -12.0f;  // Distancia inicial de los cubos para que sean visibles

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void drawCube(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z + depthOffset); // Mueve en el eje Z usando depthOffset

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

    float spacing = 2.3f; // Espaciado entre los cubos

    
    if (rotating) {
        glPushMatrix();
        glTranslatef(0.0f, spacing, 0.0f); 
        glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f); 
        glTranslatef(0.0f, -spacing, 0.0f); 
        for (int x = -1; x <= 1; ++x) {
            for (int z = -1; z <= 1; ++z) {
                drawCube(x * spacing, spacing, z * spacing); 
            }
        }
        glPopMatrix();
    }

  
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            for (int z = -1; z <= 1; ++z) {
                if (!(y == 1 && rotating)) { 
                    drawCube(x * spacing, y * spacing, z * spacing);
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
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'r') { 
        rotating = true;
    }
    if (key == 'd') { 
        depthOffset -= 0.1f;
    }
    if (key == 'f') { 
        depthOffset += 0.1f;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Rubik's Cube with Depth Offset");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    initGL();
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}