/*
    Copyright (c) 2014 - 2015 Martin Buck
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
 * \file    InterpolationBezier.h
 * \author  Martin Buck
 * \date    January 2015
 * \version 1.0
 * \brief   contains the class InterpolationBezier which represents a bezier curve
 */

#ifndef INTERPOLATIONBEZIER_H_INCLUDED
#define INTERPOLATIONBEZIER_H_INCLUDED

#include "calc.h"
#include "constants.h"
#include "Path.h"
#include "Point.h"

namespace lib_2d {

template <typename T>
class InterpolationBezier : public Path<T> {
    using Path<T>::reserve;
    using Path<T>::emplace_back;

private:

    Point<T> control_polygon(const Path<T> &path, unsigned int nPoints, T t) {
        T x(0), y(0);

        for(unsigned int i = 0; i <= nPoints; ++i) {
            x += bernstein_polynomal(nPoints,i,t) * path[i].x;
            y += bernstein_polynomal(nPoints,i,t) * path[i].y;
        }

        return Point<T>{x,y};
    }


public:

    InterpolationBezier(unsigned int nPoints,
                        const Path<T> &path) : ///@todo find proper name for path
        Path<T>(nPoints) {

        T pDistance = 1.0 / (T)(nPoints);

        for (unsigned int i=0; i<nPoints; ++i ) {
            T t = i * pDistance;
            emplace_back(control_polygon(path, path.size()-1, t));
        }
    }

//------------------------------------------------------------------------------

};

} //lib_2d

#endif // INTERPOLATIONBEZIER_H_INCLUDED
