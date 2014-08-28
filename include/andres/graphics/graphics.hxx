#pragma once
#ifndef ANDRES_GRAPHICS_GRAPHICS_HXX
#define ANDRES_GRAPHICS_GRAPHICS_HXX

#include <cstddef>
#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "point.hxx"
#include "line.hxx"

namespace andres {
namespace graphics {

template<class T = float, class S = std::size_t>
class Graphics {
public:
    typedef T value_type;
    typedef S size_type;
    typedef Point<value_type, size_type> PointType;
    typedef PointProperty<value_type, size_type> PointPropertyType;
    typedef Line<value_type, size_type> LineType;
    typedef LineProperty<value_type, size_type> LinePropertyType;
    typedef std::vector<PointType> PointsVector;
    typedef std::vector<PointPropertyType> PointPropertiesVector;
    typedef std::vector<LineType> LinesVector;
    typedef std::vector<LinePropertyType> LinePropertiesVector;

    Graphics();
    void clear();
    void assign(const PointPropertiesVector&, const PointsVector&, const LinePropertiesVector&, const LinesVector&);
    void normalize();
    size_type definePointProperty(const bool, const unsigned char, const unsigned char, const unsigned char);
    size_type defineLineProperty(const bool, const unsigned char, const unsigned char, const unsigned char);
    size_type definePoint(const value_type x, const value_type y, const value_type z, const size_type = 0);
    size_type defineLine(const size_type, const size_type, const size_type = 0);

    const size_type numberOfPoints() const;
    const size_type numberOfLines() const;
    const size_type numberOfPointProperties() const;
    const size_type numberOfLineProperties() const;
    const PointType& point(const size_type) const;
    const LineType& line(const size_type) const;
    const PointPropertyType& pointProperty(const size_type) const;
    const LinePropertyType& lineProperty(const size_type) const;
    const PointsVector& points() const;
    const LinesVector& lines() const;
    const PointPropertiesVector& pointProperties() const;
    const LinePropertiesVector& lineProperties() const;

private:
    PointsVector points_;
    LinesVector lines_;
    PointPropertiesVector pointProperties_;
    LinePropertiesVector lineProperties_;
};

template<class T, class S>
inline
Graphics<T, S>::Graphics()
:   points_(),
    lines_(),
    pointProperties_(1), // default point property
    lineProperties_(1) // default line property
{}

template<class T, class S>
inline void
Graphics<T, S>::clear() {
    points_.clear();
    lines_.clear();
    pointProperties_.resize(1);
    lineProperties_.resize(1);
}

template<class T, class S>
inline void
Graphics<T, S>::assign(
    const PointPropertiesVector& pointProperties,
    const PointsVector& points,
    const LinePropertiesVector& lineProperties,
    const LinesVector& lines
) {
    // TODO: sanity check input

    pointProperties_ = pointProperties;
    points_ = points;
    lineProperties_ = lineProperties;
    lines_ = lines;
}

template<class T, class S>
inline void
Graphics<T, S>::normalize() {
    // calculate min max and mean vector
    value_type vectorMin[3] = {
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity(),
        std::numeric_limits<float>::infinity()
    };
    value_type vectorMax[3] = {
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity(),
        -std::numeric_limits<float>::infinity()
    };
    value_type vectorMean[3] = {0.0f, 0.0f, 0.0f};

    for(size_type j = 0; j < numberOfPoints(); ++j) {
        for(size_type k = 0; k < 3; ++k) {
            if(points_[j][k] < vectorMin[k]) {
                vectorMin[k] = points_[j][k];
            }
            if(points_[j][k] > vectorMax[k]) {
                vectorMax[k] = points_[j][k];
            }
        }
    }
    for(size_type k = 0; k < 3; ++k) {
        vectorMean[k] = (vectorMin[k] + vectorMax[k]) / 2.0f;
    }

    // calculate scale
    float scale = 1.0f;
    {
        float v[3] = {0.0f, 0.0f, 0.0f};
        for(size_type k = 0; k < 3; ++k) {
            v[k] = vectorMax[k] - vectorMean[k];
        }
        scale = std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
        if(scale == 0.0f) {
            scale = 1.0f;
        }
    }

    // translate and scale
    for(size_type j = 0; j < numberOfPoints(); ++j) {
        for(size_type k = 0; k < 3; ++k) {
            points_[j][k] -= vectorMean[k];
            points_[j][k] /= scale;
        }
    }
}

template<class T, class S>
inline typename Graphics<T, S>::size_type
Graphics<T, S>::definePointProperty(
    const bool visibility,
    const unsigned char r,
    const unsigned char g,
    const unsigned char b
) {
    pointProperties_.push_back(PointPropertyType(visibility, r, g, b));
    return pointProperties_.size() - 1; // index of the point property just added
}

template<class T, class S>
inline typename Graphics<T, S>::size_type
Graphics<T, S>::defineLineProperty(
    const bool visibility,
    const unsigned char r,
    const unsigned char g,
    const unsigned char b
) {
    lineProperties_.push_back(LinePropertyType(visibility, r, g, b));
    return lineProperties_.size() - 1; // index of the line property just added
}

template<class T, class S>
inline typename Graphics<T, S>::size_type
Graphics<T, S>::definePoint(
    const value_type x,
    const value_type y,
    const value_type z,
    const size_type propertyIndex
) {
    if(propertyIndex >= pointProperties_.size()) {
        throw std::out_of_range("point property index out of range");
    }
    points_.push_back(PointType(x, y, z, propertyIndex));
    return points_.size() - 1; // index of the point just added
}

template<class T, class S>
inline typename Graphics<T, S>::size_type
Graphics<T, S>::defineLine(
    const size_type pointIndex0,
    const size_type pointIndex1,
    const size_type propertyIndex
) {
    if(pointIndex0 >= points_.size()) {
        throw std::out_of_range("point index 0 out of range");
    }
    if(pointIndex1 >= points_.size()) {
        throw std::out_of_range("point index 1 out of range");
    }
    if(propertyIndex >= lineProperties_.size()) {
        throw std::out_of_range("line property index out of range");
    }
    lines_.push_back(LineType(pointIndex0, pointIndex1, propertyIndex));
    return lines_.size() - 1; // index of the point just added
}

template<class T, class S>
inline const typename Graphics<T, S>::PointType&
Graphics<T, S>::point(
    const size_type index
) const {
    return points_[index];
}

template<class T, class S>
inline const typename Graphics<T, S>::LineType&
Graphics<T, S>::line(
    const size_type index
) const {
    return lines_[index];
}

template<class T, class S>
inline const typename Graphics<T, S>::PointPropertyType&
Graphics<T, S>::pointProperty(
    const size_type index
) const {
    return pointProperties_[index];
}

template<class T, class S>
inline const typename Graphics<T, S>::LinePropertyType&
Graphics<T, S>::lineProperty(
    const size_type index
) const {
    return lineProperties_[index];
}

template<class T, class S>
inline const typename Graphics<T, S>::size_type
Graphics<T, S>::numberOfPoints() const {
    return points_.size();
}

template<class T, class S>
inline const typename Graphics<T, S>::size_type
Graphics<T, S>::numberOfLines() const {
    return lines_.size();
}

template<class T, class S>
inline const typename Graphics<T, S>::size_type
Graphics<T, S>::numberOfPointProperties() const {
    return pointProperties_.size();
}

template<class T, class S>
inline const typename Graphics<T, S>::size_type
Graphics<T, S>::numberOfLineProperties() const {
    return lineProperties_.size();
}

template<class T, class S>
inline const typename Graphics<T, S>::PointsVector&
Graphics<T, S>::points() const {
    return points_;
}

template<class T, class S>
inline const typename Graphics<T, S>::LinesVector&
Graphics<T, S>::lines() const {
    return lines_;
}

template<class T, class S>
inline const typename Graphics<T, S>::PointPropertiesVector&
Graphics<T, S>::pointProperties() const {
    return pointProperties_;
}

template<class T, class S>
inline const typename Graphics<T, S>::LinePropertiesVector&
Graphics<T, S>::lineProperties() const {
    return lineProperties_;
}

} // namespace graphics
} // namespace andres

#endif // #ifndef ANDRES_GRAPHICS_GRAPHICS_HXX
