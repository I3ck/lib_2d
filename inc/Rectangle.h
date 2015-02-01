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
 * \file    Rectangle.h
 * \author  Martin Buck
 * \date    January 2015
 * \version 1.0
 * \brief   contains the class Rectangle which defines a Rectangle defined by center, width and height
 */

#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED

#include "Point.h"

namespace lib_2d {

template <typename T>
class Rectangle : public Path<T> {
    using Path<T>::emplace_back;
    using Path<T>::rotate;

private:
    T width, height;
    Point<T> m_center;

public:

    Rectangle(T width,
              T height,
              bool closePath = true,
              Point<T> m_center = Point<T>{},
              T angle = 0) :
                width(width),
                height(height),
                m_center(m_center){

        emplace_back(Point<T>{m_center.x - (T)width/2, m_center.y - (T)height/2});
        emplace_back(Point<T>{m_center.x + (T)width/2, m_center.y - (T)height/2});
        emplace_back(Point<T>{m_center.x + (T)width/2, m_center.y + (T)height/2});
        emplace_back(Point<T>{m_center.x - (T)width/2, m_center.y + (T)height/2});

        if(closePath)
            emplace_back(Point<T>{m_center.x - (T)width/2, m_center.y - (T)height/2});

        if(angle != 0)
            rotate(angle, m_center);
    }

//------------------------------------------------------------------------------

    T get_width() const {
        return width;
    }

    T get_height() const {
        return height;
    }

//------------------------------------------------------------------------------

    Point<T> center() const {
        return m_center;
    }

//------------------------------------------------------------------------------

    Rectangle& move_by(T x, T y) {
        Path<T>::move_by(x, y);
        m_center.move_by(x, y);
        return *this;
    }

    Rectangle& move_by(Point<T> other) {
        Path<T>::move_by(other);
        m_center.move_by(other);
        return *this;
    }

//------------------------------------------------------------------------------

    Rectangle& mirror_vertically(T xValue = 0) {
        Path<T>::mirror_vertically(xValue);
        m_center.mirror_vertically(xValue);
        return *this;
    }

    Rectangle& mirror_vertically(Point<T> other) {
        Path<T>::mirror_vertically(other);
        m_center.mirror_vertically(other);
        return *this;
    }

    Rectangle& mirror_horizontally(T yValue = 0) {
        Path<T>::mirror_horizontally(yValue);
        m_center.mirror_horizontally(yValue);
        return *this;
    }

    Rectangle& mirror_horizontally(Point<T> other) {
        Path<T>::mirror_horizontally(other);
        m_center.mirror_horizontally(other);
        return *this;
    }

    Rectangle& mirror_point(Point<T> other) {
        Path<T>::mirror_point(other);
        m_center.mirror_point(other);
        return *this;
    }

    Rectangle& mirror_point(T xValue = 0, T yValue = 0) {
        Path<T>::mirror_point(xValue, yValue);
        m_center.mirror_point(xValue, yValue);
        return *this;
    }

//------------------------------------------------------------------------------

    Rectangle& rotate(T radians, Point<T> m_center = Point<T>{}) {
        Path<T>::rotate(radians, m_center);
        this->m_center.rotate(radians, m_center);
        return *this;
    }

    Rectangle& rotate(T radians, T centerX, T centerY) {
        Path<T>::rotate(radians, centerX, centerY);
        this->m_center.rotate(radians, centerX, centerY);
        return *this;
    }

};

} //lib_2d

#endif // RECTANGLE_H_INCLUDED
