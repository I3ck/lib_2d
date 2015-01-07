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
 * \file    Arc.h
 * \author  Martin Buck
 * \date    November 2014
 * \version 1.0
 * \brief   contains the class Arc which represents an arc in 2d space
 *          derived from Path
 * \todo    many methods have to change the center aswell, maybe the diameter too
 */

#ifndef ARC_H_INCLUDED
#define ARC_H_INCLUDED

#include "Path.h"
#include "Point.h"


namespace lib_2d {

template <typename T>
class Arc : public Path<T> {
    using Path<T>::reserve;
    using Path<T>::emplace_back;

private:
    T diameter;
    Point<T> center;

public:

    Arc(const T &diameter,
        const unsigned int &nPoints,
        const bool &closePath = true,
        const T &radiansStart = 0,
        const T &radiansEnd = 3.14159265358979323846 * 2.0,
        const Point<T> &center = Point<T> (0.0, 0.0)) :
            Path<T>(),
            diameter(diameter),
            center(center) {

        T pDistance;
        unsigned int endIndex(nPoints);
        reserve(nPoints);

        if(closePath) {
            pDistance = fabs(radiansEnd - radiansStart) / (T)(nPoints - 2);
            endIndex--;
        }

        else
            pDistance = fabs(radiansEnd - radiansStart) / (T)(nPoints - 1);


        for(unsigned int i=0; i<endIndex; ++i) {
            T radians = radiansStart + i * pDistance;
            T x = center.get_x() + diameter/2.0 * cos(radians);
            T y = center.get_y() + diameter/2.0 * sin(radians);
            emplace_back(x, y);
        }

        if(closePath) {
            T x = center.get_x() + diameter/2.0 * cos(radiansStart);
            T y = center.get_y() + diameter/2.0 * sin(radiansStart);
            emplace_back(x, y);
        }


    }

//------------------------------------------------------------------------------

    T get_diameter() const {
        return diameter;
    }

    Point<T> get_center() const {
        return center;
    }


};

} //lib_2d

#endif // ARC_H_INCLUDED
