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
 * \file    InvolutCircle.h
 * \author  Martin Buck
 * \date    December 2014
 * \version 1.0
 * \brief   contains the class InvolutCircle which represents an involut of a circle
 *          derived from Path
 * \todo    many methods have to change the center aswell, maybe the diameter too
 * \todo    the involut currently has its points with the same radial distance, but absolute distance would be prefered
 */

#ifndef INVOLUTCIRCLE_H_INCLUDED
#define INVOLUTCIRCLE_H_INCLUDED

#include "Path.h"
#include "Point.h"

namespace lib_2d {

template <typename T>
class InvolutCircle : public Path<T> {
    using Path<T>::reserve;
    using Path<T>::emplace_back;

private:
    T diameter;
    Point<T> center;

public:

    InvolutCircle(const T &diameter,
                  const unsigned int nPoints,
                  const T &radiansStart = 0,
                  const T &radiansEnd = 3.14159265358979323846 * 2.0,
                  const Point<T> &center = Point<T> (0.0, 0.0)) :
            Path<T>(nPoints),
            diameter(diameter),
            center(center) {

        T pDistance = abs(radiansEnd - radiansStart) / (T)(nPoints - 1);

        for (unsigned int i=0; i<nPoints; ++i ) {
            T current = i * pDistance;
            T x = center.get_x() + diameter/2.0 * (cos(current) + current * sin(current));
            T y = center.get_y() + diameter/2.0 * (sin(current) - current * cos(current));
            emplace_back(x,y);
        }
    }

//------------------------------------------------------------------------------

    T get_diameter() const {
        return diameter;
    }

//------------------------------------------------------------------------------

    Point<T> get_center() const {
        return center;
    }
};

} //lib_2d

#endif // INVOLUTCIRCLE_H_INCLUDED
