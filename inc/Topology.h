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
 * \file    Topology.h
 * \author  Martin Buck
 * \date    October 2015
 * \version 1.0
 * \brief   contains the class Topology which defines lines / triangles / quads etc.
 */

#ifndef TOPOLOGY_H_INCLUDED
#define TOPOLOGY_H_INCLUDED

#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <utility>
#include <array>

#include "Point.h"

namespace lib_2d {

template <size_t ELEMENTSIZE>
class Topology {

    using Element = std::array<size_t, ELEMENTSIZE>;

protected:
    std::vector < size_t > top;
public:
    Topology(){};

    Topology(unsigned int nElements) {
        top.reserve(ELEMENTSIZE * nElements);
    }

    template<class InputIterator>
    Topology(InputIterator first, InputIterator last) {
        while(first != last) {
            top.push_back(*first);
            ++first;
        }
    }

    ~Topology(){};

//------------------------------------------------------------------------------

    Topology& push_back(const Element &e) {
        for (const auto &i : e)
            top.push_back(i);
        return *this;
    }

    Topology& emplace_back(Element e) {
        for (const auto &i : e)
            top.emplace_back(i);
        return *this;
    }

//------------------------------------------------------------------------------

    Topology& pop_back() {
        for(size_t i = 0; i < ELEMENTSIZE; ++i)
            top.pop_back();
        return *this;
    }

//------------------------------------------------------------------------------

    size_t n_elements() const {
        return top.size() / ELEMENTSIZE;
    }

//------------------------------------------------------------------------------

    Element first() const {
        Element e;
        for(unsigned int i = 0; i < ELEMENTSIZE; ++i)
            e[i] = top[i];
        return e;
    }

    Element last() const {
        Element e;
        for(unsigned int i = top.size() - 1 - ELEMENTSIZE; i < top.size(); ++i)
            e[i - top.size() - 1 - ELEMENTSIZE] = top[i];
        return e;
    }

//------------------------------------------------------------------------------

    bool empty() const {
        return top.empty();
    }

//------------------------------------------------------------------------------

    Topology& reserve_elements(size_t i) {
        top.reserve(i * ELEMENTSIZE);
        return *this;
    }

//------------------------------------------------------------------------------

    Topology& clear() {
        top.clear();
        return *this;
    }

//------------------------------------------------------------------------------

    Topology& reverse() {
        std::reverse(top.begin(), top.end());
        return *this;
    }

//------------------------------------------------------------------------------

    bool equal_to (const Topology &other) const {
        if(top.size() != other.top.size())
            return false;
        for(size_t i = 0; i < top.size(); ++i) {
            if(top[i]  != other.top[i])
                return false;
        }
        return true;
    }

//------------------------------------------------------------------------------

    Element operator [] (unsigned int i) const {
        Element e;
        for(size_t j = 0; j < ELEMENTSIZE; ++j)
            e[j] = top[ELEMENTSIZE * i + j];
        return e;
    }

};

} //lib_2d

#endif // TOPOLOGY_H_INCLUDED
