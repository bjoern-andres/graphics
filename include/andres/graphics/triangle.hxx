#pragma once
#ifndef ANDRES_GRAPHICS_TRIANGLE_HXX
#define ANDRES_GRAPHICS_TRIANGLE_HXX

#include <cstddef>

#include "types.hxx"

namespace andres {
namespace graphics {

namespace hdf5 {
    template<class T> class HDF5Type;
}

template<class T = float, class S = std::size_t>
class Triangle {
public:
    typedef T value_type;
    typedef S size_type;

    Triangle()
        : Triangle(0, 0, 0)
        {}
    Triangle(const size_type pointIndex0, const size_type pointIndex1, const size_type pointIndex2, const size_type propertyIndex = 0)
        : propertyIndex_(propertyIndex)
        { pointIndices_[0] = pointIndex0; pointIndices_[1] = pointIndex1; pointIndices_[2] = pointIndex2; }
    size_type& pointIndex(const size_type j)
        { assert(j < 3); return pointIndices_[j]; }
    size_type& propertyIndex()
        { return propertyIndex_; }
    size_type pointIndex(const size_type j) const
        { assert(j < 3); return pointIndices_[j]; }
    size_type propertyIndex() const
        { return propertyIndex_; }
    bool operator==(const Triangle<T, S>& other) const
        { return pointIndices_[0] == other.pointIndices_[0]
            && pointIndices_[1] == other.pointIndices_[1]
            && pointIndices_[2] == other.pointIndices_[2]
            && propertyIndex_ == other.propertyIndex_;
        }

private:
    size_type pointIndices_[3];
    size_type propertyIndex_;

friend class hdf5::HDF5Type<Triangle<T, S> >;
};

template<class T = float, class S = std::size_t>
class TriangleProperty {
public:
    typedef T value_type;
    typedef S size_type;

    TriangleProperty(const Bit visibility = true)
        : TriangleProperty(visibility, 0, 0, 0, 255)
        {}
    TriangleProperty(const Bit visibility, const Color r, const Color g, const Color b, const Color alpha)
        : visibility_(visibility)
        { color_[0] = r; color_[1] = g; color_[2] = b; alpha_ = alpha; }
    Bit& visibility()
        { return visibility_; }
    Color& color(const size_type j)
        { assert(j < 3); return color_[j]; }
    Color& alpha()
        { return alpha_; }

    Bit visibility() const
        { return visibility_; }
    Color color(const size_type j) const
        { assert(j < 3); return color_[j]; }
    Color alpha() const
        { return alpha_; }
    bool operator==(const TriangleProperty<T, S>& other) const
        { return visibility_ == other.visibility_
            && color_[0] == other.color_[0]
            && color_[1] == other.color_[1]
            && color_[2] == other.color_[2]
            && alpha_ == other.alpha_;
        }

private:
    Bit visibility_;
    Color color_[3];
    Color alpha_;

friend class hdf5::HDF5Type<TriangleProperty<T, S> >;
};

} // namespace graphics
} // namespace andres

#endif // #ifndef ANDRES_GRAPHICS_TRIANGLE_HXX
