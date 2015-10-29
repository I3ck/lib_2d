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
 * \file    TopologicalPointCloud.h
 * \author  Martin Buck
 * \date    October 2015
 * \version 1.0
 * \brief   contains the class TopologicalPointCloud which is used to only mark certain points of a PointCloud
 */

#ifndef TOPOLOGICALPOINTCLOUD_H_INCLUDED
#define TOPOLOGICALPOINTCLOUD_H_INCLUDED

#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <utility>
#include <array>
#include <memory>

#include "PointCloud.h"
#include "Topology.h"

namespace lib_2d {

template <typename T>
class TopologicalPointCloud {

    using Element = std::array<size_t, 1>;

public:
    ///@todo leave these public or write wrappers?
    ///@todo clean up this file
    ///@todo const correctness
    ///@todo make it possible to create subsets of subsets
    Topology<1> topology;
    std::shared_ptr<PointCloud<T>> pc;

    TopologicalPointCloud() {}

    TopologicalPointCloud(std::shared_ptr<PointCloud<T>> points)
        : pc(points) {
        topology.reserve_elements(points->size());
        for(size_t i = 0; i < points->size(); ++i)
            topology.push_back(Element{i});
    }

    TopologicalPointCloud(std::shared_ptr<PointCloud<T> > points, Topology<1> top) :
        topology(top),
        pc(points) {}

    inline void push_back(size_t pId) {
        topology.push_back(Element{pId});
    }

    PointCloud<T> as_pointcloud() const {
        PointCloud<T> result;
        for (size_t i = 0; i < topology.n_elements(); ++i)
            result.push_back(get_tpoint(i));
    }

    ///@todo remove from PC once solely used from here
    TopologicalPointCloud& sort_x() {
        std::sort(topology.begin(), topology.end(),
            [this](Element lhs, Element rhs){return get_point(lhs[0]).x < get_point(rhs[0]).x; });
        return *this;
    }

    TopologicalPointCloud& sort_y() {
        std::sort(topology.begin(), topology.end(),
            [this](Element lhs, Element rhs){return get_point(lhs[0]).y < get_point(rhs[0]).y; });
        return *this;
    }

    inline Point<T> first() const {
        return get_tpoint(0);
    }

    inline size_t first_id() const {
        return topology[0][0];
    }

    inline Point<T> last() const {
        return get_tpoint(topology.n_elements() - 1);
    }

    inline size_t last_id() const {
        return topology[topology.n_elements() - 1][0];
    }

    inline std::shared_ptr<PointCloud<T>> get_parent() {
        return pc;
    }

    inline void set_parent(std::shared_ptr<PointCloud<T>> p) {
        pc = p;
    }

    inline size_t n_elements() const {
        return topology.n_elements();
    }

    inline void reserve(size_t n) {
        topology.reserve_elements(n);
    }

    inline void push_back_id(size_t i) {
        topology.emplace_back(Element{i});
    }

    inline size_t get_id(size_t i) const {
        return topology[i][0];
    }

    inline Point<T> get_tpoint(size_t tId) const {
        return get_point(topology[tId][0]);
    }

    inline Point<T> get_point(size_t pId) const {
        return (*pc)[pId];
    }

    ///@todo add all other analog methods and return this
    inline void push_back(Point<T> p) {
        pc->push_back(p);
        push_back_id(pc->size() - 1);
    }

//------------------------------------------------------------------------------

    std::string to_string(std::string divider = " ") const {
        std::string output("");

        for(size_t i = 0; i < n_elements(); ++i) {
            auto p = get_tpoint(i);
            output += p.to_string(divider) + "\n";
        }

        return output;
    }

//------------------------------------------------------------------------------

    bool to_file(const std::string &path) const {
        std::ofstream out(path.c_str());
        if(!out.good())
        return false;
        out << to_string() << "\n";
        out.close();
        return true;
    }

//------------------------------------------------------------------------------

    typename std::vector <Element>::iterator begin() {
        return topology.begin();
    }

    typename std::vector <Element>::iterator end() {
        return topology.end();
    }

    typename std::vector <Element>::const_iterator cbegin() const {
        return topology.cbegin();
    }

    typename std::vector <Element>::const_iterator cend() const {
        return topology.cend();
    }

    typename std::vector <Element>::reverse_iterator rbegin() {
        return topology.rbegin();
    }

    typename std::vector <Element>::reverse_iterator rend() {
        return topology.rend();
    }

};

} //lib_2d

#endif // TOPOLOGICALPOINTCLOUD_H_INCLUDED
