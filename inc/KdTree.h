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

    Point<T> nearest(const Point<T> &search) const {
        if(is_leaf()) return val; //reached the end, return current value

        auto comp = dimension_compare(search, val, dimension);
        Point<T> best; //nearest neighbor of search
        if(comp == LEFT && left)
            best = left->nearest(search);
        else if(comp == RIGHT && right)
            best = right->nearest(search);

        if(search.sqr_distance_to(val) < search.sqr_distance_to(best))
            best = val; //make this value the best if it is closer than the checked side

        //check whether other side might have candidates aswell
        T distanceBest 	= search.distance_to(best);
        T borderLeft 	= search[dimension] - distanceBest;
        T borderRight 	= search[dimension] + distanceBest;
        Point<T> otherBest;

        //check whether distances to other side are smaller than currently worst candidate
        //and recurse into the "wrong" direction, to check for possibly additional candidates
        if(comp == LEFT && right) {
            if(borderRight >= val[dimension]) {
                otherBest = right->nearest(search);
                if(search.sqr_distance_to(otherBest) < search.sqr_distance_to(best))
                    best = otherBest;
            }
        }
        else if (comp == RIGHT && left) {
            if(borderLeft <= val[dimension])
                otherBest = left->nearest(search);
                if(search.sqr_distance_to(otherBest) < search.sqr_distance_to(best))
                    best = otherBest;
        }

        return best;
    }

    Path<T> k_nearest(const Point<T> &search, size_t n) const {
        if(n < 1) return Path<T>(); //no real search if n < 1
        if(is_leaf()) return Path<T>({Point<T>(val)}); //no further recursion, return current value

        Path<T> res; //nearest neighbors of search
        if(res.size() < n || search.sqr_distance_to(val) < search.sqr_distance_to(res.last()))
            res += val; //add current node if there is still room or if it is closer than the currently worst candidate

        //decide which side to check and recurse into it
        auto comp = dimension_compare(search, val, dimension);
        if(comp == LEFT) {
            if(left) res += left->k_nearest(search, n);
        } else if(right) {
            res += right->k_nearest(search, n);
        }

        //only keep the required number of candidates and sort them by distance
        sort_and_limit(res, search, n);

        //check whether other side might have candidates aswell
        T distanceBest 	= search.distance_to(res.last());
        T borderLeft 	= search[dimension] - distanceBest;
        T borderRight 	= search[dimension] + distanceBest;

        //check whether distances to other side are smaller than currently worst candidate
        //and recurse into the "wrong" direction, to check for possibly additional candidates
        if(comp == LEFT && right) {
            if(res.size() < n || borderRight >= val[dimension])
                res += right->k_nearest(search, n);
        }
        else if (comp == RIGHT && left) {
            if(res.size() < n || borderLeft <= val[dimension])
                res += left->k_nearest(search, n);
        }

        sort_and_limit(res, search, n);
        return res;
    }

private:

    enum Compare {LEFT, RIGHT};

    bool is_leaf() const {
        return !left && !right;
    }
    
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

    static inline void sort_and_limit(Path<T> &target, const Point<T> &search, size_t maxSize) { ///@todo rename
        if(target.size() > maxSize) {
            auto uniqueIt = std::unique(target.begin(), target.end()); ///@todo might be quicker to use a set from the beginning
            target.remove_from( std::distance(target.begin(), uniqueIt));
            std::sort(target.begin(), target.end(),
                [&search](const Point<T> &a, const Point<T> &b) {
                    return search.sqr_distance_to(a) < search.sqr_distance_to(b);
                });
            target.remove_from(maxSize);
        }
    }


};

} //lib_2d

#endif // POINT_H_INCLUDED
