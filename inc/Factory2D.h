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
    //(warning, needs more testing)
    ///@todo pass a normal PC and create the topological one inside?
    static TopologicalPointCloud<T> concave_hull(std::shared_ptr<TopologicalPointCloud<T>> path, size_t nNearest, int maxIter = -1, bool closePath = true) { ///@todo use closePath
        const bool dbg(false);
        auto hull = TopologicalPointCloud<T>(); ///@todo only save ids
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
            if(dbg) std::cout << i << std::endl;
            size_t next;

            if(dbg) std::cout << "before tree" << std::endl;
            if(dbg) std::cout << "tree size: " << tree.size() << std::endl;
            if(dbg) std::cout << "prev: " << prev << std::endl;
            if(dbg) std::cout << "nNearest :" << nNearest << std::endl;
            //std::cout << tree.to_path() << std::endl;
            auto candidates = tree.k_nearest(pPrev, nNearest); ///@todo exclude first, since it will be the search itself
            if(dbg) std::cout << "after tree" << std::endl;
            //candidates.remove_until(1);
            //if(dbg) std::cout << candidates << std::endl;

            std::sort(candidates.begin(), candidates.end(), [&](Element ip1, Element ip2){
                auto p1 = path->get_point(ip1[0]);
                auto p2 = path->get_point(ip2[0]);
                //return true if p1 better than p2
//                if(/*hull.size() > nNearest && */p1 == start) return false;
//                if(/*hull.size() > nNearest && */p2 == start) return true;

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

            if(dbg) std::cout << "AFTER SORT" << std::endl;


            if(candidates.n_elements() < 1) continue;
            next = candidates[0][0];

            hull.push_back_id(next);
            prev = next;
            if(next == start) break;
        }
        hull.push_back_id(start);
        hull.push_back_id(path->get_id(0)); ///@todo add a "close" boolean flag?
        return hull;
    }
};

//------------------------------------------------------------------------------

} //lib_2d

#endif // FACTORY2D_H_INCLUDED
