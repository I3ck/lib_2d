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
 * \file    calc.h
 * \author  Martin Buck
 * \date    January 2015
 * \version 1.0
 * \brief   contains basic calculation functions which are necessary for this lib
 */

#ifndef CALC_H_INCLUDED
#define CALC_H_INCLUDED

#include "Point.h"
#include "Path.h"

namespace lib_2d {

//------------------------------------------------------------------------------

    template <typename T>
    int turn(const Point<T> &p, const Point<T> &q, const Point<T> &r) {
        Point<T> pq = direction(p, q);
        Point<T> qr = direction(q, r);

        if(cclockwise(pq, qr)) return 1;
        if(clockwise(pq, qr)) return -1;
        else return 0;
    }

//------------------------------------------------------------------------------

    template <typename T>
    bool clockwise(const Point<T> &dir1, const Point<T> &dir2) {
        return cross(dir1, dir2) < 0;
    }

    template <typename T>
    bool cclockwise(const Point<T> &dir1, const Point<T> &dir2) {
        return cross(dir1, dir2) > 0;
    }

    template <typename T>
    bool colinear(const Point<T> &dir1, const Point<T> &dir2) {
        return cross(dir1, dir2) == 0;
    }

//------------------------------------------------------------------------------

    template <typename T>
    Point<T> direction(const Point<T> &p1, const Point<T> &p2) {
        return Point<T>({p2.x - p1.x, p2.y - p1.y});
    }

//------------------------------------------------------------------------------

    template <typename T>
    T dot(const Point<T> &p1, const Point<T> &p2) {
        return p1.x * p2.x + p1.y * p2.y;
    }

//------------------------------------------------------------------------------

    template <typename T>
    T cross(const Point<T> &p1, const Point<T> &p2) {
        return p1.x * p2.y - p2.x * p1.y;
    }

//------------------------------------------------------------------------------

    template <typename T>
    T distance_point_line(Point<T> p, Point<T> l1, Point<T> l2) {
        T a1 = l1.x;
        T a2 = l1.y;

        T b1 = l2.x;
        T b2 = l2.y;

        T c1 = p.x;
        T c2 = p.y;

        T x = (a1*a1*c1 - a1*a2*b2 + a1*a2*c2 - 2*a1*b1*c1 + a1*b2*b2 - a1*b2*c2 + a2*a2*b1 - a2*b1*b2 - a2*b1*c2 + b1*b1*c1 + b1*b2*c2)
              /(a1*a1 - 2*a1*b1 + a2*a2 - 2*a2*b2 + b1*b1 + b2*b2);

        T y = ((a2 - b2) * x + a1*b2 - a2*b1) / (a1 - b1);

        return sqrt(  pow(x-p.x,2) + pow(y-p.y,2)  );
    }

//------------------------------------------------------------------------------

    template <typename T>
    bool is_equal(const Point<T> &a, const Point<T> b) {
        return a.equal_to(b);
    }

    template <typename T>
    bool is_equal(const Path<T> &a, const Path<T> b) {
        return a.equal_to(b);
    }

//------------------------------------------------------------------------------

    template <typename T>
    bool is_similar(const Point<T> &a, const Point<T> b, T maxDistance) {
        return a.similar_to(b, maxDistance);
    }

    template <typename T>
    bool is_similar(const Path<T> &a, const Path<T> b, T maxDistance) {
        return a.similar_to(b, maxDistance);
    }

//------------------------------------------------------------------------------

    template <typename T>
    Path<T> calc_intersections(Point<T> p1, Point<T> p2, Point<T> q1, Point<T> q2) {
        bool
            pVertical(false),
            pHorizontal(false),
            qVertical(false),
            qHorizontal(false);
        T
            pSlope(0),
            qSlope(0);

        Point<T> intersection;
        Path<T> intersections;

        if(p1.x == p2.x)
            pVertical = true;
        else
            pSlope = p1.slope_to(p2);

        if(p1.y == p2.y)
            pHorizontal = true;

        if(q1.x == q2.x) {
            if(qVertical)
                return intersections;
            qVertical = true;
        }

        if(q1.y == q2.y) {
            if(qHorizontal)
                return intersections;
            qHorizontal = true;
        }

        if(pVertical) {
            intersection.x = p1.x;
            if(qHorizontal)
                intersection.y = q1.y;
            else
                intersection.y =
                  q1.slope_to(q2)
                  * intersection.x
                  + ( q2.x * q1.y - q1.x * q2.y )
                  / ( q2.x - q1.x )
                  ;
        }
        else if(qVertical) {
            intersection.x = q1.x;
            if(pHorizontal)
                intersection.y = p1.y;
            else
                intersection.y =
                  p1.slope_to(p2)
                  * intersection.x
                  + ( p2.x * p1.y - p1.x * p2.y )
                  / ( q2.x - q1.x )
                  ;
        }
        else {
            qSlope = q1.slope_to(q2);
            intersection.x = (q1.y - p1.y + pSlope * p1.x - qSlope * q1.x)
                             /  (pSlope - qSlope) ;
            intersection.y = pSlope * (intersection.x - p1.x) + p1.y;
        }

        if( ( (q2.x >= intersection.x && q1.x <= intersection.x )
            || (q1.x >= intersection.x && (q2.x <= intersection.x))
            )
            &&
            ( (p2.x >= intersection.x && p1.x <= intersection.x )
            || (p1.x >= intersection.x && (p2.x <= intersection.x))
            )
        )
            intersections.push_back(intersection);

        return intersections;
    }
} //lib_2d

#endif // CALC_H_INCLUDED
