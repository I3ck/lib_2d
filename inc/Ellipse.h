/*
    Copyright (c) 2015 Martin Buck
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/**
 * \file    Ellipse.h
 * \author  Martin Buck
 * \date    November 2014
 * \version 1.0
 * \brief   contains the class Ellipse which represents an ellipse in 2d space
 *          derived from Path
 */

#ifndef ELLIPSE_H_INCLUDED
#define ELLIPSE_H_INCLUDED

#include "constants.h"
#include "PointCloud.h"
#include "Point.h"


namespace lib_2d {

template <typename T>
class Ellipse : public PointCloud<T> {
    using PointCloud<T>::emplace_back;

private:
    T a, b, angle;
    Point<T> m_center;

public:

    Ellipse(T a,
            T b,
            unsigned int nPoints,
            bool closePath = true,
            Point<T> m_center = Point<T>{},
            T angle = 0) :

        PointCloud<T>(nPoints),
        a(a),
        b(b),
        angle(angle),
        m_center(m_center) {

        T pDistance;
        unsigned int endIndex(nPoints);

        if(closePath) {
            pDistance = LIB_2D_2PI / (T)(nPoints - 2);
            endIndex--;
        }

        else
            pDistance = LIB_2D_2PI / (T)(nPoints - 1);

        for(unsigned int i=0; i<endIndex; ++i) {
            T radians = i * pDistance;
            T x = m_center.x + a * cos(radians)*cos(angle) - b*sin(radians)*sin(angle);
            T y = m_center.y + a * cos(radians)*sin(angle) + b*sin(radians)*cos(angle);
            emplace_back(x, y);
        }

        if(closePath) {
            T x = m_center.x + a * cos(0)*cos(angle) - b*sin(0)*sin(angle);
            T y = m_center.y + a * cos(0)*sin(angle) + b*sin(0)*cos(angle);
            emplace_back(x, y);
        }
    }

//------------------------------------------------------------------------------

    Point<T> center() const {
        return m_center;
    }

//------------------------------------------------------------------------------

    Ellipse& move_by(T x, T y) {
        PointCloud<T>::move_by(x, y);
        m_center.move_by(x, y);
        return *this;
    }

    Ellipse& move_by(Point<T> other) {
        PointCloud<T>::move_by(other);
        m_center.move_by(other);
        return *this;
    }

//------------------------------------------------------------------------------

    Ellipse& mirror_vertically(T xValue = 0) {
        PointCloud<T>::mirror_vertically(xValue);
        m_center.mirror_vertically(xValue);
        return *this;
    }

    Ellipse& mirror_vertically(Point<T> other) {
        PointCloud<T>::mirror_vertically(other);
        m_center.mirror_vertically(other);
        return *this;
    }

    Ellipse& mirror_horizontally(T yValue = 0) {
        PointCloud<T>::mirror_horizontally(yValue);
        m_center.mirror_horizontally(yValue);
        return *this;
    }

    Ellipse& mirror_horizontally(Point<T> other) {
        PointCloud<T>::mirror_horizontally(other);
        m_center.mirror_horizontally(other);
        return *this;
    }

    Ellipse& mirror_point(Point<T> other) {
        PointCloud<T>::mirror_point(other);
        m_center.mirror_point(other);
        return *this;
    }

    Ellipse& mirror_point(T xValue = 0, T yValue = 0) {
        PointCloud<T>::mirror_point(xValue, yValue);
        m_center.mirror_point(xValue, yValue);
        return *this;
    }

//------------------------------------------------------------------------------

    Ellipse& rotate(T radians, Point<T> m_center = Point<T>{}) {
        PointCloud<T>::rotate(radians, m_center);
        this->m_center.rotate(radians, m_center);
        return *this;
    }

    Ellipse& rotate(T radians, T centerX, T centerY) {
        PointCloud<T>::rotate(radians, centerX, centerY);
        this->m_center.rotate(radians, centerX, centerY);
        return *this;
    }
};

} //lib_2d

#endif // ELLIPSE_H_INCLUDED
