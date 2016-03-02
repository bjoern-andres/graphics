#include <cstddef>
#include <fstream>

#include <andres/graphics/graphics-hdf5.hxx>
#include <andres/graphics/svg.hxx>

#include <GL/glew.h>
#include <GL/freeglut.h>

typedef std::size_t size_type;
typedef andres::graphics::Graphics<float, size_type> Graphics;
typedef Graphics::PointType Point;
typedef Graphics::PointPropertyType PointProperty;
typedef Graphics::LineType Line;
typedef Graphics::LinePropertyType LineProperty;

Graphics graphics;

float pointSize = 5.0f;
float lineWidth = 1.0f;
unsigned char colorHorizon[] = {200, 200, 200};
bool showPoints = true;
bool showLines = true;
bool showAxes = true;
bool showHorizon = true;

void init() {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLineWidth(1.0f);

    if(showHorizon) {
        glColor3ub(colorHorizon[0], colorHorizon[1], colorHorizon[2]);
        glBegin(GL_LINES);
            for(float r = -1.0f; r < 1.1f; r += 0.1f) {
                glVertex3f(r, -1.0f, 0.0f);
                glVertex3f(r, 1.0f, 0.0f);

                glVertex3f(-1.0f, r, 0.0f);
                glVertex3f(1.0f, r, 0.0f);
            }
        glEnd();
    }

    if(showAxes) {
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

    if(showPoints) {
        glPointSize(pointSize);
        glBegin(GL_POINTS);
            for(size_type j = 0; j < graphics.numberOfPoints(); ++j) {
                const Point& point = graphics.point(j);
                const PointProperty& pointProperty = graphics.pointProperty(point.propertyIndex());
                if(pointProperty.visibility()) {
                    glColor4ub(pointProperty.color(0), pointProperty.color(1), pointProperty.color(2), pointProperty.alpha());
                    glVertex3f(point[0], point[1], point[2]);
                }
            }
        glEnd();
    }

    if(showLines) {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
            for(size_type j = 0; j < graphics.numberOfLines(); ++j) {
                const Line& line = graphics.line(j);
                const LineProperty& lineProperty = graphics.lineProperty(line.propertyIndex());
                if(lineProperty.visibility()) {
                    glColor4ub(lineProperty.color(0), lineProperty.color(1), lineProperty.color(2), lineProperty.alpha());
                    for(size_type k = 0; k < 2; ++k) {
                        const Point& point = graphics.point(line.pointIndex(k));
                        glVertex3f(point[0], point[1], point[2]);
                    }
                }
            }
        glEnd();
    }

    glutSwapBuffers();
}

struct Projection
{
    typedef float value_type;
    typedef size_t size_type;

    Projection(GLfloat const* m, float off_x, float off_y, float scale = 1.0)
        : m_(m), off_x_(off_x), off_y_(off_y), scale_(scale)
    { }

    void operator()(value_type a, value_type b, value_type c, value_type& x, value_type& y) const
    {
        x = (m_[0]*a + m_[4]*b + m_[8]*c)*scale_ + off_x_;
        y = -(m_[1]*a + m_[5]*b + m_[9]*c)*scale_ + off_y_;
    }

private:
    GLfloat const* m_;
    float off_x_;
    float off_y_;
    float scale_;
};

void exportViewAsSVG() {
    GLfloat m[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, m);

    Projection projection(m, 0, 0);   

    float t[2] = { std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
    for(size_type j = 0; j < graphics.numberOfPoints(); ++j) {
        const auto& point = graphics.point(j);
        auto x = .0f;
        auto y = .0f;
        projection(point[0], point[1], point[2], x, y);

        t[0] = std::min(t[0], x);
        t[1] = std::min(t[1], y);
    }
    auto scale = 400.0;
    projection = Projection(m, -t[0]*scale, -t[1]*scale, scale);

    std::ofstream f("view.svg");
    saveSVG(graphics, projection, f);
    f.close();
}

void keyboard(unsigned char key, int x, int y) {
    const float angleStep = 3.0f;
    const float scaleUpStep = 1.1f;
    const float scaleDownStep = 1.0f / scaleUpStep;

    switch(key) {
    case '2': // roate left around x axis
        glRotatef(-angleStep, 1.0f, 0.0f, 0.0f);
        glutPostRedisplay();
        break;
    case '8': // roate right around x axis
        glRotatef(angleStep, 1.0f, 0.0f, 0.0f);
        glutPostRedisplay();
        break;

    case '4': // roate left around y axis
        glRotatef(-angleStep, 0.0f, 1.0f, 0.0f);
        glutPostRedisplay();
        break;
    case '6': // roate right around y axis
        glRotatef(angleStep, 0.0f, 1.0f, 0.0f);
        glutPostRedisplay();
        break;

    case '1': // roate left around z axis
        glRotatef(-angleStep, 0.0f, 0.0f, 1.0f);
        glutPostRedisplay();
        break;
    case '3': // roate right around z axis
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

    case 'q': // increase point size
        showPoints = true;
        pointSize += 1.0f;
        glutPostRedisplay();
        break;
    case 'a':// decrease point size
        pointSize -= 1.0f;
        if(pointSize < 1.0f) {
            showPoints = false;
            pointSize = 0.0f;
        }
        glutPostRedisplay();
        break;

    case 'w': // increase line width
        showLines = true;
        lineWidth += 1.0f;
        glutPostRedisplay();
        break;
    case 's':// decrease line width
        lineWidth -= 1.0f;
        if(lineWidth < 1.0f) {
            showLines = false;
            lineWidth = 0.0f;
        }
        glutPostRedisplay();
        break;

    case 'r': // enable/disable drawing of axes
        showAxes = !showAxes;
        glutPostRedisplay();
        break;
    case 't': // enable/disable drawing of horizon
        showHorizon = !showHorizon;
        glutPostRedisplay();
        break;

    case 'p': // make a screenshot
        exportViewAsSVG();
        break;

    case 27: // (escape key) quit
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
        hid_t file = andres::graphics::hdf5::openFile(fileName);
        andres::graphics::hdf5::load(file, graphics);
        andres::graphics::hdf5::closeFile(file);
    }

    graphics.center();
    graphics.normalize();

    std::cout << "Keys and functions:" << std::endl
        << "   2    rotate left around x axis" << std::endl
        << "   8    rotate right around x axis" << std::endl
        << "   4    rotate left around y axis" << std::endl
        << "   6    rotate right around y axis" << std::endl
        << "   1    rotate left around z axis" << std::endl
        << "   3    rotate right around z axis" << std::endl
        << "   +    zoom in" << std::endl
        << "   -    zoom out" << std::endl
        << "   q    increase point size" << std::endl
        << "   a    decrease point size" << std::endl
        << "   w    increase line width" << std::endl
        << "   s    decrease line width" << std::endl
        << "   r    enable/disable drawing of axes" << std::endl
        << "   t    enable/disable drawing of horizon" << std::endl
        << "   p    export current view as SVG file 'view.svg'"
        << std::endl;

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
