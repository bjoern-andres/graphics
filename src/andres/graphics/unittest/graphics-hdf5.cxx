#include <stdexcept>

#include "andres/graphics/graphics-hdf5.hxx"

inline void test(const bool condition) {
    if(!condition) throw std::logic_error("test failed.");
}

int main() {
    typedef andres::graphics::Graphics<> Graphics;
    typedef typename Graphics::PointType Point;
    typedef typename Graphics::LineType Line;
    typedef typename Graphics::size_type size_type;

    // draw and save graphics
    Graphics graphics;
    {
        graphics.definePoint(0, 0, 0);
        graphics.definePoint(0, 0, 1);
        graphics.definePoint(0, 1, 0);
        graphics.definePoint(0, 1, 1);
        {
            const size_type pointProperty = graphics.definePointProperty(true, 255, 0, 0);
            graphics.definePoint(1, 0, 0, pointProperty);
            graphics.definePoint(1, 0, 1, pointProperty);
            graphics.definePoint(1, 1, 0, pointProperty);
            graphics.definePoint(1, 1, 1, pointProperty);
        }
        graphics.defineLine(0, 4);
        graphics.defineLine(1, 5);
        graphics.defineLine(2, 6);
        graphics.defineLine(3, 7);

        {
            const size_type lineProperty = graphics.defineLineProperty(true, 0, 255, 0);
            graphics.defineLine(0, 2, lineProperty);
            graphics.defineLine(1, 3, lineProperty);
            graphics.defineLine(4, 6, lineProperty);
            graphics.defineLine(5, 7, lineProperty);
        }
        graphics.defineLine(0, 1);
        graphics.defineLine(2, 3);
        graphics.defineLine(4, 5);
        graphics.defineLine(6, 7);

        hid_t file = andres::hdf5::createFile("graphics.h5");
        andres::hdf5::save(file, graphics);
        andres::hdf5::closeFile(file);
    }

    // load graphics
    Graphics graphicsLoaded;
    {
        hid_t file = andres::hdf5::openFile("graphics.h5");
        andres::hdf5::load(file, graphicsLoaded);
        andres::hdf5::closeFile(file);
    }

    // test equality of graphics
    test(graphics.numberOfPointProperties() == graphicsLoaded.numberOfPointProperties());
    test(graphics.numberOfPoints() == graphicsLoaded.numberOfPoints());
    test(graphics.numberOfLineProperties() == graphicsLoaded.numberOfLineProperties());
    test(graphics.numberOfLines() == graphicsLoaded.numberOfLines());
    for(size_type j = 0; j < graphics.numberOfPointProperties(); ++j) {
        test(graphics.pointProperty(j) == graphicsLoaded.pointProperty(j));
    }
    for(size_type j = 0; j < graphics.numberOfPoints(); ++j) {
        test(graphics.point(j) == graphicsLoaded.point(j));
    }
    for(size_type j = 0; j < graphics.numberOfLineProperties(); ++j) {
        test(graphics.lineProperty(j) == graphicsLoaded.lineProperty(j));
    }
    for(size_type j = 0; j < graphics.numberOfLines(); ++j) {
        test(graphics.line(j) == graphicsLoaded.line(j));
    }
}
