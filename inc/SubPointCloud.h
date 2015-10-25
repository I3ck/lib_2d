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
class SubPointCloud {

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
            result.push_back(   (*pc)[  (topology[i])[0]  ]   );
    }

};

} //lib_2d

#endif // SubPointCloud_H_INCLUDED
