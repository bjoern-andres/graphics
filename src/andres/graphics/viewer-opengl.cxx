#include <cstddef>

#include <andres/graphics/graphics-hdf5.hxx>

#include <GL/glew.h>
#include <GL/freeglut.h>

typedef std::size_t size_type;
typedef andres::graphics::Graphics<float, size_type> Graphics;
typedef Graphics::PointType Point;
typedef Graphics::PointPropertyType PointProperty;
typedef Graphics::LineType Line;
typedef Graphics::LinePropertyType LineProperty;

Graphics graphics;

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // draw axes
    {
        const float lengthAxis = 1.0f;
        glBegin(GL_LINES);
            glColor3ub(255, 0, 0);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(lengthAxis, 0.0f, 0.0f);

            glColor3ub(0, 255, 0);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, lengthAxis, 0.0f);

            glColor3ub(0, 0, 255);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, lengthAxis);
        glEnd();
    }

    glBegin(GL_POINTS);
        for(size_type j = 0; j < graphics.numberOfPoints(); ++j) {
            const Point& point = graphics.point(j);
            const PointProperty& pointProperty = graphics.pointProperty(point.propertyIndex());
            if(pointProperty.visibility()) {
                glColor3ub(pointProperty.color(0), pointProperty.color(1), pointProperty.color(2));
                glVertex3f(point[0], point[1], point[2]);
            }
        }
    glEnd();

    glBegin(GL_LINES);
        for(size_type j = 0; j < graphics.numberOfLines(); ++j) {
            const Line& line = graphics.line(j);
            const LineProperty& lineProperty = graphics.lineProperty(line.propertyIndex());
            if(lineProperty.visibility()) {
                glColor3ub(lineProperty.color(0), lineProperty.color(1), lineProperty.color(2));
                for(size_type k = 0; k < 2; ++k) {
                    const Point& point = graphics.point(line.pointIndex(k));
                    glVertex3f(point[0], point[1], point[2]);
                }
            }
        }
    glEnd();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    const float angleStep = 3.0f;
    const float scaleUpStep = 1.1f;
    const float scaleDownStep = 1.0f / scaleUpStep;

    switch(key) {
    case '2': // x left
        glRotatef(-angleStep, 1.0f, 0.0f, 0.0f);
        glutPostRedisplay();
        break;
    case '8': // x right
        glRotatef(angleStep, 1.0f, 0.0f, 0.0f);
        glutPostRedisplay();
        break;
    case '4': // y left
        glRotatef(-angleStep, 0.0f, 1.0f, 0.0f);
        glutPostRedisplay();
        break;
    case '6': // y right
        glRotatef(angleStep, 0.0f, 1.0f, 0.0f);
        glutPostRedisplay();
        break;
    case '1': // z left
        glRotatef(-angleStep, 0.0f, 0.0f, 1.0f);
        glutPostRedisplay();
        break;
    case '3': // z right
        glRotatef(angleStep, 0.0f, 0.0f, 1.0f);
        glutPostRedisplay();
        break;
    case '+': // scale up
        glScalef(scaleUpStep, scaleUpStep, scaleUpStep);
        glutPostRedisplay();
        break;
    case '-': // scale up
        glScalef(scaleDownStep, scaleDownStep, scaleDownStep);
        glutPostRedisplay();
        break;
    case 27: // escape
        exit(0);
        break;
    default:
        break;
    }
}

int main(int argc, char** argv) {
    // parse command line input
    if(argc != 2) {
        std::cerr << "parameters: <graphics.h5>" << std::endl;
        return 1;
    }
    const std::string fileName = argv[1];

    // load graphics from file
    {
        hid_t file = andres::hdf5::openFile(fileName);
        andres::hdf5::load(file, graphics);
        andres::hdf5::closeFile(file);
    }

    graphics.center();
    // graphics.normalize();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("andres::graphics");
    init();
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
