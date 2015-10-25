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
 * \file    Factory2D.h
 * \author  Martin Buck
 * \date    Oct 2015
 * \version 1.0
 * \brief   contains higher order helpers which could not be methods of single classes
 */

#ifndef FACTORY2D_H_INCLUDED
#define FACTORY2D_H_INCLUDED

#include <set>
#include <algorithm>
#include <utility>
#include <iostream>

#include "Point.h"
#include "Path.h"
#include "KdTree.h"

namespace lib_2d {

template <typename T>
class Factory2D {
public:

//------------------------------------------------------------------------------

    //gift wrapping combined with knearest
    //(warning, needs more testing)
    static PointCloud<T> concave_hull(PointCloud<T> path, size_t nNearest, int maxIter = -1, bool closePath = true) {
        const bool dbg(false);
        PointCloud<T> hull;
        if(path.size() < 3) return hull;
        if(nNearest > path.size()) return hull;

        path.sort_x();

        const KdTree<T> tree(path);

        const Point<T> start = path.first();
        Point<T> prev = start;
        hull += path[1];
        for(int i = 2; maxIter == -1 || i < maxIter ; ++i) {
            if(dbg) std::cout << i << std::endl;
            Point<T> next;

            if(dbg) std::cout << "before tree" << std::endl;
            if(dbg) std::cout << "tree size: " << tree.size() << std::endl;
            if(dbg) std::cout << "prev: " << prev << std::endl;
            if(dbg) std::cout << "nNearest :" << nNearest << std::endl;
            //std::cout << tree.to_path() << std::endl;
            auto candidates = tree.k_nearest(prev, nNearest); ///@todo exclude first, since it will be the search itself
            if(dbg) std::cout << "after tree" << std::endl;
            //candidates.remove_until(1);
            if(dbg) std::cout << candidates << std::endl;

            std::sort(candidates.begin(), candidates.end(), [&](const Point<T> &p1, const Point<T> &p2){
                //return true if p1 better than p2
//                if(/*hull.size() > nNearest && */p1 == start) return false;
//                if(/*hull.size() > nNearest && */p2 == start) return true;

                bool p1Elem = any_of(hull.begin(), hull.end(), [&p1](const Point<T> &h){return h == p1;});
                bool p2Elem = any_of(hull.begin(), hull.end(), [&p2](const Point<T> &h){return h == p2;});

                if      (p1Elem && !p2Elem) return false;
                else if (!p1Elem && p2Elem) return true;
                else if (p1Elem && p2Elem) return false;

                auto trn = turn(prev, p1, p2);
                if      (trn < 0) return false;
                else if (trn > 0) return true;

                if(prev.sqr_distance_to(p1) < prev.sqr_distance_to(p2)) return false;

                return true;
            });
            
            if(dbg) std::cout << "AFTER SORT" << std::endl;


            if(candidates.size() < 1) continue;
            next = candidates[0];

            hull += next;
            prev = next;
            if(next == start) break;
        }
        hull += start;
        hull += hull[0]; ///@todo add a "close" boolean flag?
        return hull;
    }
};

//------------------------------------------------------------------------------

} //lib_2d

#endif // FACTORY2D_H_INCLUDED
