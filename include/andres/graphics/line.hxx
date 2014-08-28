#pragma once
#ifndef ANDRES_GRAPHICS_LINE_HXX
#define ANDRES_GRAPHICS_LINE_HXX

#include <cstddef>

#include "types.hxx"

namespace andres {

namespace hdf5 {
    template<class T> class HDF5Type;
}

namespace graphics {

template<class T = float, class S = std::size_t>
class Line {
public:
    typedef T value_type;
    typedef S size_type;

    Line()
        : Line(0, 0, 0)
        {}
    Line(const size_type pointIndex0, const size_type pointIndex1, const size_type propertyIndex = 0)
        : propertyIndex_(propertyIndex)
        { pointIndices_[0] = pointIndex0; pointIndices_[1] = pointIndex1; }
    size_type& pointIndex(const size_type j)
        { assert(j < 2); return pointIndices_[j]; }
    size_type& propertyIndex()
        { return propertyIndex_; }
    size_type pointIndex(const size_type j) const
        { assert(j < 2); return pointIndices_[j]; }
    size_type propertyIndex() const
        { return propertyIndex_; }
    bool operator==(const Line<T, S>& other) const
        { return pointIndices_[0] == other.pointIndices_[0]
            && pointIndices_[1] == other.pointIndices_[1]
            && propertyIndex_ == other.propertyIndex_;
        }

private:
    size_type pointIndices_[2];
    size_type propertyIndex_;

friend class andres::hdf5::HDF5Type<Line<T, S> >;
};

template<class T = float, class S = std::size_t>
class LineProperty {
public:
    typedef T value_type;
    typedef S size_type;

    LineProperty(const Bit visibility = true)
        : LineProperty(visibility, 0, 0, 0)
        {}
    LineProperty(const Bit visibility, const Color r, const Color g, const Color b)
        : visibility_(visibility)
        { color_[0] = r; color_[1] = g; color_[2] = b; }
    Color& color(const size_type j)
        { assert(j < 3); return color_[j]; }
    Bit& visibility()
        { return visibility_; }
    Color color(const size_type j) const
        { assert(j < 3); return color_[j]; }
    Bit visibility() const
        { return visibility_; }
    bool operator==(const LineProperty<T, S>& other) const
        { return visibility_ == other.visibility_
            && color_[0] == other.color_[0]
            && color_[1] == other.color_[1]
            && color_[2] == other.color_[2];
        }

private:
    Bit visibility_;
    Color color_[3];

friend class andres::hdf5::HDF5Type<LineProperty<T, S> >;
};

} // namespace graphics
} // namespace andres

#endif // #ifndef ANDRES_GRAPHICS_LINE_HXX
