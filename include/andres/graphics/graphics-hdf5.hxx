#pragma once
#ifndef ANDRES_GRAPHICS_GRAPHICS_HDF5_HXX
#define ANDRES_GRAPHICS_GRAPHICS_HDF5_HXX

#include <andres/hdf5/hdf5.hxx>

#include "andres/graphics/graphics.hxx"

namespace andres {
namespace hdf5 {

template<class T, class S>
class HDF5Type<andres::graphics::PointProperty<T, S> > {
private:
    typedef andres::graphics::PointProperty<T, S> PointPropertyType;

public:
    HDF5Type()
        : type_(H5Tcreate(H5T_COMPOUND, sizeof(PointPropertyType)))
        { H5Tinsert(type_, "visibility", HOFFSET(PointPropertyType, visibility_), HDF5Type<andres::graphics::Bit>().type());
          H5Tinsert(type_, "color", HOFFSET(PointPropertyType, color_), HDF5Type<andres::graphics::Color>().type());
          H5Tinsert(type_, "alpha", HOFFSET(PointPropertyType, alpha_), HDF5Type<andres::graphics::Color>().type()); }
    ~HDF5Type()
        { H5Tclose(type_); }
    hid_t type() const
        { return type_; }

private:
    hid_t type_;
};

template<class T, class S>
class HDF5Type<andres::graphics::Point<T, S> > {
private:
    typedef T value_type;
    typedef S size_type;
    typedef andres::graphics::Point<T, S> PointType;

public:
    HDF5Type()
        : type_(H5Tcreate(H5T_COMPOUND, sizeof(PointType)))
        { H5Tinsert(type_, "vector", HOFFSET(PointType, r_), HDF5Type<value_type>().type());
          H5Tinsert(type_, "property-index", HOFFSET(PointType, propertyIndex_), HDF5Type<size_type>().type()); }
    ~HDF5Type()
        { H5Tclose(type_); }
    hid_t type() const
        { return type_; }

private:
    hid_t type_;
};

template<class T, class S>
class HDF5Type<andres::graphics::LineProperty<T, S> > {
private:
    typedef andres::graphics::LineProperty<T, S> LinePropertyType;

public:
    HDF5Type()
        : type_(H5Tcreate(H5T_COMPOUND, sizeof(LinePropertyType)))
        { H5Tinsert(type_, "visibility", HOFFSET(LinePropertyType, visibility_), HDF5Type<andres::graphics::Bit>().type());
          H5Tinsert(type_, "color", HOFFSET(LinePropertyType, color_), HDF5Type<andres::graphics::Color>().type());
          H5Tinsert(type_, "alpha", HOFFSET(LinePropertyType, alpha_), HDF5Type<andres::graphics::Color>().type()); }
    ~HDF5Type()
        { H5Tclose(type_); }
    hid_t type() const
        { return type_; }

private:
    hid_t type_;
};


template<class T, class S>
class HDF5Type<andres::graphics::Line<T, S> > {
private:
    typedef T value_type;
    typedef S size_type;
    typedef andres::graphics::Line<T, S> LineType;

public:
    HDF5Type()
        : type_(H5Tcreate(H5T_COMPOUND, sizeof(LineType)))
        { H5Tinsert(type_, "point-indices", HOFFSET(LineType, pointIndices_), HDF5Type<size_type>().type());
          H5Tinsert(type_, "property-index", HOFFSET(LineType, propertyIndex_), HDF5Type<size_type>().type()); }
    ~HDF5Type()
        { H5Tclose(type_); }
    hid_t type() const
        { return type_; }

private:
    hid_t type_;
};

template<class T, class S>
void
save(
    const hid_t parentHandle,
    const andres::graphics::Graphics<T, S>& graphics
) {
    save(parentHandle, "point-properties", graphics.pointProperties());
    save(parentHandle, "points", graphics.points());
    save(parentHandle, "line-properties", graphics.lineProperties());
    save(parentHandle, "lines", graphics.lines());
}

template<class T, class S>
void
load(
    const hid_t parentHandle,
    andres::graphics::Graphics<T, S>& graphics
) {
    typedef andres::graphics::Graphics<T, S> GraphicsType;
    typedef typename GraphicsType::PointType PointType;
    typedef typename GraphicsType::PointsVector PointsVector;
    typedef typename GraphicsType::PointPropertiesVector PointPropertiesVector;
    typedef typename GraphicsType::LineType LineType;
    typedef typename GraphicsType::LinesVector LinesVector;
    typedef typename GraphicsType::LinePropertiesVector LinePropertiesVector;

    PointPropertiesVector pointProperties;
    PointsVector points;
    LinePropertiesVector lineProperties;
    LinesVector lines;

    load(parentHandle, "point-properties", pointProperties);
    load(parentHandle, "points", points);
    load(parentHandle, "line-properties", lineProperties);
    load(parentHandle, "lines", lines);

    graphics.assign(pointProperties, points, lineProperties, lines);
}

} // namespace hdf5
} // namespace andres

#endif // #ifndef ANDRES_GRAPHICS_GRAPHICS_HDF5_HXX
