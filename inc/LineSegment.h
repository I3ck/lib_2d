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
 * \file    LineSegment.h
 * \author  Martin Buck
 * \date    January 2015
 * \version 1.0
 * \brief   contains the class LineSegment which defines a line segment defined by two points
 * \todo    overload methods which could be simplified (if many are being overloaded, maybe define as own class)
 */

#ifndef LINESEGMENT_H_INCLUDED
#define LINESEGMENT_H_INCLUDED

#include "Point.h"

namespace lib_2d {

template <typename T>
class LineSegment : public Path<T> {
    using Path<T>::emplace_back;

public:

    LineSegment(Point<T> p1,
                Point<T> p2) {

        emplace_back(p1);
        emplace_back(p2);
    }
};

} //lib_2d

#endif // LINESEGMENT_H_INCLUDED
