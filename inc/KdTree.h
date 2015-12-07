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
#include "OrderedPointCloud.h"

namespace lib_2d {

template <typename T>
class KdTree {

using Element = std::array<size_t, 1>;

private:

//------------------------------------------------------------------------------

    enum Compare {LEFT, RIGHT};

    std::unique_ptr<KdTree<T> >
        left,
        right;

    size_t pId;

    std::shared_ptr<OrderedPointCloud<T>> parent;

    const int dimension;

//------------------------------------------------------------------------------

public:
    KdTree& operator=(const KdTree&) = delete;
    KdTree(const KdTree&) = delete;

//------------------------------------------------------------------------------

    KdTree(std::shared_ptr<OrderedPointCloud<T>> tpc, int dim = 0) :
        dimension(dim % 2),
        parent(tpc) {

        if(tpc->n_elements() == 1)
            pId = tpc->first_id();

        else if(tpc->n_elements() > 1) {
            size_t median = tpc->n_elements() / 2;
            std::shared_ptr<OrderedPointCloud<T>>
                tpcL = std::make_shared<OrderedPointCloud<T>>(),
                tpcR = std::make_shared<OrderedPointCloud<T>>();
            tpcL->set_parent(tpc->get_parent());
            tpcR->set_parent(tpc->get_parent());

            tpcL->reserve(median - 1);
            tpcR->reserve(median - 1);

            dimension_sort(tpc, dimension);

            for(size_t i = 0; i < tpc->n_elements(); ++i) {
                if(i < median)
                    tpcL->push_back_id(tpc->get_id(i));
                else if(i > median)
                    tpcR->push_back_id(tpc->get_id(i));
            }

            pId = tpc->get_id(median);

            if(tpcL->n_elements() > 0)  left = std::unique_ptr<KdTree>(new KdTree(tpcL, dimension+1));
            if(tpcR->n_elements() > 0) right = std::unique_ptr<KdTree>(new KdTree(tpcR, dimension+1));
        }
    }

//------------------------------------------------------------------------------

    size_t size() const {
        size_t out(0);
        if(left)  out += left->size();
        out +=1;
        if(right) out += right->size();
        return out;
    }

//------------------------------------------------------------------------------

    Topology<1> to_topology() const {
        Topology<1> out;
        if(left) out.push_back(left->to_topology());
        out.push_back(pId);
        if(right) out.push_back(right->to_topology());
        return out;
    }

//------------------------------------------------------------------------------

    std::shared_ptr<OrderedPointCloud<T>> get_parent() const {
        return parent;
    }

//------------------------------------------------------------------------------

    size_t nearest(const Point<T> &search) const {
        if(is_leaf()) return pId; //reached the end, return current value
        auto val = parent->get_point(pId);
        auto comp = dimension_compare(search, val , dimension);
        size_t idBest; //nearest neighbor of search
        if(comp == LEFT && left)
            idBest = left->nearest(search);
        else if(comp == RIGHT && right)
            idBest = right->nearest(search);
        else
            return pId;

        T distanceBest 	= search.distance_to(parent->get_point(idBest));
        T distanceThis  = search.sqr_distance_to(val);

        if(distanceThis < distanceBest) {
            distanceBest = distanceThis;
            idBest = pId; //make this value the best if it is closer than the checked side
        }

        //check whether other side might have candidates aswell
        T borderLeft 	= search[dimension] - distanceBest;
        T borderRight 	= search[dimension] + distanceBest;
        size_t idOtherBest;

        //check whether distances to other side are smaller than currently best
        //and recurse into the "wrong" direction, to check for possibly additional candidates
        if(comp == LEFT && right) {
            if(borderRight >= val[dimension]) {
                idOtherBest = right->nearest(search);
                if(search.sqr_distance_to(parent->get_point(idOtherBest)) < search.sqr_distance_to(parent->get_point(idBest)))
                    idBest = idOtherBest;
            }
        }
        else if (comp == RIGHT && left) {
            if(borderLeft <= val[dimension]) {
                idOtherBest = left->nearest(search);
                if(search.sqr_distance_to(parent->get_point(idOtherBest)) < search.sqr_distance_to(parent->get_point(idBest)))
                    idBest = idOtherBest;
            }
        }

        return idBest;
    }

//------------------------------------------------------------------------------

    Topology<1> k_nearest(const Point<T> &search, size_t n) const {
        if(n < 1) return Topology<1>(); //no real search if n < 1
        if(is_leaf()) return Topology<1>(Element{pId}); //no further recursion, return current value

        auto val = parent->get_point(pId);

        Topology<1> res; //nearest neighbors of search
        if(res.n_elements() < n || search.sqr_distance_to(val) < search.sqr_distance_to(  parent->get_point(res.last()[0])  ))
            res += Element({pId}); //add current node if there is still room or if it is closer than the currently worst candidate

        //decide which side to check and recurse into it
        auto comp = dimension_compare(search, val, dimension);

        if(comp == LEFT) {
            if(left) res += left->k_nearest(search, n);
        } else if(right) {
            res += right->k_nearest(search, n);
        }

        //only keep the required number of candidates and sort them by distance
        sort_and_limit(res, parent->get_parent(), search, n);

        //check whether other side might have candidates aswell
        T distanceBest 	= search.distance_to(parent->get_point(res.last()[0]));
        T borderLeft 	= search[dimension] - distanceBest;
        T borderRight 	= search[dimension] + distanceBest;

        //check whether distances to other side are smaller than currently worst candidate
        //and recurse into the "wrong" direction, to check for possibly additional candidates
        if(comp == LEFT && right) {
            if(res.n_elements() < n || borderRight >= val[dimension])
                res += right->k_nearest(search, n);
        }
        else if (comp == RIGHT && left) {
            if(res.n_elements() < n || borderLeft <= val[dimension])
                res += left->k_nearest(search, n);
        }

        sort_and_limit(res, parent->get_parent(), search, n);
        return res;
    }

//------------------------------------------------------------------------------

    Topology<1> in_circle(const Point<T> &search, T radius) const {
        if(radius <= 0.0) return Topology<1>(); //no real search if radius <= 0

        auto val = parent->get_point(pId);

        Topology<1> res; //all points within the sphere
        if(search.distance_to(val) <= radius)
            res += Element({pId}); //add current node if it is within the search radius

        if(is_leaf()) return res; //no children, return result

        //decide which side to check and recurse into it
        auto comp = dimension_compare(search, val, dimension);
        if(comp == LEFT) {
            if(left) res += left->in_circle(search, radius);
        } else if(right) {
            res += right->in_circle(search, radius);
        }

        T borderLeft 	= search[dimension] - radius;
        T borderRight 	= search[dimension] + radius;

        //check whether distances to other side are smaller than radius
        //and recurse into the "wrong" direction, to check for possibly additional candidates
        if(comp == LEFT && right) {
            if(borderRight >= val[dimension])
                res += right->in_circle(search, radius);
        }
        else if (comp == RIGHT && left) {
            if(borderLeft <= val[dimension])
                res += left->in_circle(search, radius);
        }

        return res;
    }

//------------------------------------------------------------------------------

    Topology<1> in_box(const Point<T> &search, T xSize, T ySize) const {
        if(xSize <= 0.0 || ySize <= 0.0) return Topology<1>(); //no real search if width or height <= 0

        auto val = parent->get_point(pId);

        Topology<1> res; //all points within the box
        if(   dimension_dist(search, val, 0) <= 0.5 * xSize
           && dimension_dist(search, val, 1) <= 0.5 * ySize)
            res += Element({pId}); //add current node if it is within the search box

        if(is_leaf()) return res; //no children, return result

        //decide which side to check and recurse into it
        auto comp = dimension_compare(search, val, dimension);
        if(comp == LEFT) {
            if(left) res += left->in_box(search, xSize, ySize);
        } else if(right) {
            res += right->in_box(search, xSize, ySize);
        }

        T borderLeft 	= search[dimension] - 0.5 * (dimension == 0 ? xSize : ySize);
        T borderRight 	= search[dimension] + 0.5 * (dimension == 0 ? xSize : ySize);

        //check whether distances to other side are smaller than radius
        //and recurse into the "wrong" direction, to check for possibly additional candidates
        if(comp == LEFT && right) {
            if(borderRight >= val[dimension])
                res += right->in_box(search, xSize, ySize);
        }
        else if (comp == RIGHT && left) {
            if(borderLeft <= val[dimension])
                res += left->in_box(search, xSize, ySize);
        }

        return res;
    }

//------------------------------------------------------------------------------

private:

    inline bool is_leaf() const {
        return !left && !right;
    }

//------------------------------------------------------------------------------

    static inline void dimension_sort(std::shared_ptr<OrderedPointCloud<T>> path, size_t dimension) {
        std::nth_element(path->begin(), path->begin() + path->n_elements()/2, path->end(),
                         [&path, dimension] (Element lhs, Element rhs){return path->get_point(lhs[0])[dimension] < path->get_point(rhs[0])[dimension]; });
    }

    static inline T dimension_dist(const Point<T> &lhs, const Point<T> &rhs, size_t dimension) {
        return std::fabs( lhs[dimension] - rhs[dimension] );
    }

    static inline Compare dimension_compare(const Point<T> &lhs, const Point<T> &rhs, size_t dimension) {
        if(lhs[dimension] <= rhs[dimension]) return LEFT;
        else return RIGHT;
    }

//------------------------------------------------------------------------------

    static inline void sort_and_limit(Topology<1> &target, const std::shared_ptr<PointCloud<T>> pc, const Point<T> &search, size_t maxSize) {
        if(target.n_elements() > maxSize) {
            std::partial_sort(target.begin(), target.begin() + maxSize, target.end(),
                [&search, &pc](const Element &a, const Element &b) {
                    return search.sqr_distance_to(pc->get_point(a[0])) < search.sqr_distance_to(pc->get_point(b[0]));
                });
            target.remove_from(maxSize);
        }
    }
};

} //lib_2d

#endif //KDTREE_H_INCLUDED
