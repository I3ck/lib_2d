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
 * \file    SubPointCloud.h
 * \author  Martin Buck
 * \date    October 2015
 * \version 1.0
 * \brief   contains the class SubPointCloud which is used to only mark certain points of a PointCloud
 */

#ifndef SubPointCloud_H_INCLUDED
#define SubPointCloud_H_INCLUDED

#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <utility>
#include <array>

#include "Point.h"

namespace lib_2d {

template <typename T>
class SubPointCloud { ///@todo rename to topological PC or similar

protected:
    Topology<1> topology;
    PointCloud<T>* pc;
public:
    SubPointCloud() : pc(nullptr){};

    void push_back(size_t pId) {
        topology.push_back({pId});
    }

    PointCloud<T> as_pointcloud() const {
        PointCloud<T> result;
        for (size_t i = 0; i < topology.n_elements(); ++i)
            result.push_back(get_tpoint(i));
    }

    ///@todo remove from PC once solely used from here
    SubPointCloud& sort_x() {
        std::sort(topology.begin(), topology.end(),
            [](size_t lhs, size_t rhs){return get_point(lhs).x < get_point(rhs).x; });
        return *this;
    }

    SubPointCloud& sort_y() {
        std::sort(topology.begin(), topology.end(),
            [](size_t lhs, size_t rhs){return get_point(lhs).y < get_point(rhs).y; });
        return *this;
    }

private:

    inline Point<T> get_tpoint(size_t tId) {
        return get_point(topology[tId][0]);
    }

    inline Point<T> get_point(size_t pId) {
        return (*pc)[pId];
    }

};

} //lib_2d

#endif // SubPointCloud_H_INCLUDED
