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
 * \file    InterpolationLinear.h
 * \author  Martin Buck
 * \date    January 2015
 * \version 1.0
 * \brief   contains the class InterpolationLinear which will add interpolation points linearly
 */

#ifndef INTERPOLATIONLINEAR_H_INCLUDED
#define INTERPOLATIONLINEAR_H_INCLUDED

#include "constants.h"
#include "Path.h"
#include "Point.h"

namespace lib_2d {

template <typename T>
class InterpolationLinear : public Path<T> {
    using Path<T>::reserve;
    using Path<T>::emplace_back;

public:

    InterpolationLinear(unsigned int nPoints,
                        const Path<T> &path) : ///@todo find proper name for path
        Path<T> (nPoints) {

        T pDistance = path.length() / (T)(nPoints-1);

        for (unsigned int i=0; i<nPoints; ++i ) {
            T   traveled(0),
                traveledBefore(0);

            for(auto j = path.cbegin()+1; j != path.cend(); ++j) {
                traveled += sqrt ( pow( j->x - (j-1)->x,2) + pow(j->y - (j-1)->y,2) );

                if(traveled >= pDistance*i) {
                    T proportion = (i*pDistance - traveledBefore) / (traveled - traveledBefore);
                    T newX = (j-1)->x + proportion * ( j->x - (j-1)->x);
                    T newY = (j-1)->y + proportion * ( j->y - (j-1)->y);

                    emplace_back(Point<T>(newX, newY));
                    break;
                }
                traveledBefore = traveled;
            }
        }

        }
};

} //lib_2d


#endif // INTERPOLATIONLINEAR_H_INCLUDED
