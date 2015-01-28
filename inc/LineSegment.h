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
 * \todo    either define with two Points as member OR define as a Path of size 2
 *          the Path solution might be better, since it'll make many methods available
 */

#ifndef LINESEGMENT_H_INCLUDED
#define LINESEGMENT_H_INCLUDED

#include "Point.h"

namespace lib_2d {

template <typename T>
class LineSegment {

    public:

        Point<T> p1, p2;



};

} //lib_2d

#endif // LINESEGMENT_H_INCLUDED
