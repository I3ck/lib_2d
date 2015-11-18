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
#include <memory>

#include "calc.h"
#include "Point.h"
#include "PointCloud.h"
#include "KdTree.h"

namespace lib_2d {

template <typename T>
class Factory2D {
    using Element = std::array<size_t, 1>;
public:

//------------------------------------------------------------------------------

    //gift wrapping combined with knearest
    ///@todo consider benchmarking against gift-opening algorithm
    static OrderedPointCloud<T> concave_hull(std::shared_ptr<PointCloud<T>> pc, size_t nNearest, int maxIter = -1, bool closePath = true) {
        auto path = std::make_shared<OrderedPointCloud<T>>(pc);
        auto hull = OrderedPointCloud<T>();
        hull.set_parent(path->get_parent());
        if(path->n_elements() < 3) return hull;
        if(nNearest > path->n_elements()) return hull;

        path->sort_x();

        const KdTree<T> tree(path);

        const size_t start = path->get_id(0);
        size_t prev = start;
        hull.push_back_id(path->get_id(1));
        for(int i = 2; maxIter == -1 || i < maxIter ; ++i) {
            auto pPrev = path->get_point(prev);
            size_t next;

            auto candidates = tree.k_nearest(pPrev, nNearest);

            std::sort(candidates.begin(), candidates.end(), [&](Element ip1, Element ip2){
                auto p1 = path->get_point(ip1[0]);
                auto p2 = path->get_point(ip2[0]);

                bool p1Elem = any_of(hull.begin(), hull.end(), [&p1, &path](Element h){return path->get_point(h[0]) == p1;});
                bool p2Elem = any_of(hull.begin(), hull.end(), [&p2, &path](Element h){return path->get_point(h[0]) == p2;});

                if      (p1Elem && !p2Elem) return false;
                else if (!p1Elem && p2Elem) return true;
                else if (p1Elem && p2Elem) return false;

                auto trn = turn(pPrev, p1, p2);
                if      (trn < 0) return false;
                else if (trn > 0) return true;

                if(pPrev.sqr_distance_to(p1) < pPrev.sqr_distance_to(p2)) return false;

                return true;
            });

            if(candidates.n_elements() < 1) continue;
            next = candidates[0][0];

            hull.push_back_id(next);
            prev = next;
            if(next == start) break;
        }
        if(closePath) hull.push_back_id(start);
        return hull;
    }
};

//------------------------------------------------------------------------------

} //lib_2d

#endif // FACTORY2D_H_INCLUDED
