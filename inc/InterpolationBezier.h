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
class InterpolationBezier : public PointCloud<T> {
    using PointCloud<T>::emplace_back;

private:

    unsigned long long int faculty(unsigned int number) {
        unsigned long long int f = 1;
        for(unsigned int i =1; i <= number; ++i)
            f *= i;
        return f;
    }

    T binomial_coeff(unsigned int n, unsigned int k) {
        return faculty(n) / ( faculty(k) * faculty(n-k) );
    }

    T bernstein_polynomal(unsigned int n, unsigned int i, T t) {
        return binomial_coeff(n,i) * pow(t,i) * pow(1.0-t, n-i);
    }

    Point<T> control_polygon(const PointCloud<T> &path, unsigned int nPoints, T t) {
        T x(0), y(0);

        for(unsigned int i = 0; i <= nPoints; ++i) {
            x += bernstein_polynomal(nPoints,i,t) * path[i].x;
            y += bernstein_polynomal(nPoints,i,t) * path[i].y;
        }

        return Point<T>{x,y};
    }


public:

    InterpolationBezier(unsigned int nPoints,
                        const PointCloud<T> &path) : ///@todo find proper name for path
        PointCloud<T>(nPoints) {

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
