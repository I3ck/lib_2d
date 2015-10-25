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
 * \file    InterpolationCosine.h
 * \author  Martin Buck
 * \date    January 2015
 * \version 1.0
 * \brief   contains the class InterpolationCosine which will add interpolation points with the cosine function
 */

#ifndef INTERPOLATIONCOSINE_H_INCLUDED
#define INTERPOLATIONCOSINE_H_INCLUDED

#include "constants.h"
#include "PointCloud.h"
#include "Point.h"

namespace lib_2d {

template <typename T>
class InterpolationCosine : public PointCloud<T> {
    using PointCloud<T>::emplace_back;

public:

    InterpolationCosine(unsigned int nPoints,
                        const PointCloud<T> &path) : ///@todo find proper name for path
        PointCloud<T> (nPoints) {

        T pDistance = path.length() / (T)(nPoints-1);

        for (unsigned int i=0; i<nPoints; ++i ) {
            T   traveled(0),
                traveledBefore(0);

            for(auto j = path.cbegin()+1; j != path.cend(); ++j) {
                traveled += sqrt ( pow( j->x - (j-1)->x,2) + pow(j->y - (j-1)->y,2) );

                if(traveled >= pDistance*i) {
                    T proportion = (i*pDistance - traveledBefore) / (traveled - traveledBefore);
                    T proportion2 = ( 1 - cos(proportion*LIB_2D_PI) ) / 2.0;
                    T newX = (j-1)->x + proportion * (j->x - (j-1)->x);
                    T newY = (j-1)->y * ( 1 - proportion2) + j->y*proportion2;

                    emplace_back(Point<T>{newX, newY});
                    traveledBefore = traveled;
                    break;
                }
                traveledBefore = traveled;
            }
        }
        }
};

} //lib_2d


#endif // INTERPOLATIONCOSINE_H_INCLUDED
