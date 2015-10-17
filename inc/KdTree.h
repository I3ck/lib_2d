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
 * \file    KdTree.h
 * \author  Martin Buck
 * \date    November 2014
 * \version 1.0
 * \brief   contains the class KdTree
 */

#ifndef KDTREE_H_INCLUDED
#define KDTREE_H_INCLUDED

#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <memory>
#include <utility>

#include "Point.h"
#include "Path.h"

namespace lib_2d {

template <typename T>
class KdTree {

private:

    std::unique_ptr<KdTree<T> >
        left,
        right;

    Point<T> val;

    const int dimension;

public:
    KdTree(Path<T> path, int dim = 0) : dimension(dim % 2) { ///@todo no assignment / copy const, make second, public const without dimension and make this one here private
        if(path.size() == 1)
            val = path.first();

        else if(path.size() > 1) {
            size_t median = path.size() / 2;
            Path<T> pathLeft, pathRight;

            pathLeft.reserve(median - 1);
            pathRight.reserve(median - 1);

            dimension_sort(path, dimension);

            for(size_t i = 0; i < path.size(); ++i) {
                if(i < median)
                    pathLeft.emplace_back(path[i]);
                else if(i > median)
                    pathRight.emplace_back(path[i]);
            }

            val = path[median];

            if(pathLeft.size() > 0)  left =  std::unique_ptr<KdTree>(new KdTree(pathLeft, dimension+1));
            if(pathRight.size() > 0) right = std::unique_ptr<KdTree>(new KdTree(pathRight, dimension+1));
        }
    }

    size_t size() const {
        size_t out(0);
        if(left)  out += left->size();
        out +=1;
        if(right) out += right->size();
        return out;
    }

    Path<T> to_path() const {
        Path<T> out;
        if(left) out += left->to_path();
        out += val;
        if(right) out += right->to_path();
        return out;
    }

    bool is_leaf() const {
        return !left && !right;
    }

    Point<T> nearest(const Point<T> &search) const {
        if(is_leaf()) return val;

        auto comp = dimension_compare(search, val, dimension);
        if(comp == LEFT) {
            Point<T> pointLeft = left->nearest(search);
            double distVal = search.sqr_distance_to(val);
            double distLeft = search.sqr_distance_to(  pointLeft  );
            if( distVal < distLeft) return val;
            else return pointLeft; ///@todo else case has to check hyperplane
        }
        else
            return right->nearest(search);
    }

    Path<T> k_nearest(size_t k) const {
        /*@todo
            create a list of size k, walk to the nearest and keep adding / popping when addin newer ones
        */
    }

private:

    enum Compare {LEFT, RIGHT};

    static inline void dimension_sort(Path<T> &path, size_t dimension) {
        if(dimension == 0)
            path.sort_x();
        else
            path.sort_y();
    }

    static inline Compare dimension_compare(const Point<T> &lhs, const Point<T> &rhs, size_t dimension) {
        T val1, val2;
        if(dimension == 0) {
            val1 = lhs.x;
            val2 = rhs.x;
        }
        else {
            val1 = lhs.y;
            val2 = rhs.y;
        }

        if(val1 <= val2) return LEFT;
        else return RIGHT;
    }

    static inline T dimension_sqr_dist(const Point<T> &p1, const Point<T> &p2, size_t dimension) {
        if(dimension == 0)
            return (p1.x - p2.x) * (p1.x - p2.x);
        else
            return (p1.y - p2.y) * (p1.y - p2.y);
    }


};

} //lib_2d

#endif // POINT_H_INCLUDED