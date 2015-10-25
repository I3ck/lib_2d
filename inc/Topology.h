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
 * \brief   contains the class Topology which defines ordered points / lines / triangles / quads etc.
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
    std::vector < Element > elements;
public:
    Topology(){};

    Topology(unsigned int nElements) {
        elements.reserve(nElements);
    }

    template<class InputIterator>
    Topology(InputIterator first, InputIterator last) {
        while(first != last) {
            elements.push_back(*first);
            ++first;
        }
    }

    ~Topology(){};

//------------------------------------------------------------------------------

    Topology& push_back(const Element &e) {
        elements.push_back(e);
        return *this;
    }

    Topology& emplace_back(Element e) {
        elements.emplace_back(e);
        return *this;
    }

//------------------------------------------------------------------------------

    Topology& pop_back() {
        elements.pop_back();
        return *this;
    }

//------------------------------------------------------------------------------

    size_t n_elements() const {
        return elements.size();
    }

//------------------------------------------------------------------------------

    Element first() const {
        return elements[0];
    }

    Element last() const {
        return elements[elements.size() - 1];
    }

//------------------------------------------------------------------------------

    bool empty() const {
        return elements.empty();
    }

//------------------------------------------------------------------------------

    Topology& reserve_elements(size_t i) {
        elements.reserve(i);
        return *this;
    }

//------------------------------------------------------------------------------

    Topology& clear() {
        elements.clear();
        return *this;
    }

//------------------------------------------------------------------------------

    Topology& reverse() { ///@todo several versions (reverse element order, reverse each element)
        std::reverse(elements.begin(), elements.end());
        return *this;
    }

//------------------------------------------------------------------------------

    bool equal_to (const Topology &other) const {
        if(elements.size() != other.elements.size())
            return false;
        for(size_t i = 0; i < elements.size(); ++i) {
            if(elements[i]  != elements.top[i])
                return false;
        }
        return true;
    }
//------------------------------------------------------------------------------

    typename std::vector <Element>::iterator begin() {
        return elements.begin();
    }

    typename std::vector <Element>::iterator end() {
        return elements.end();
    }

    typename std::vector <Element>::const_iterator cbegin() const {
        return elements.cbegin();
    }

    typename std::vector <Element>::const_iterator cend() const {
        return elements.cend();
    }

    typename std::vector <Element>::reverse_iterator rbegin() {
        return elements.rbegin();
    }

    typename std::vector <Element>::reverse_iterator rend() {
        return elements.rend();
    }

//------------------------------------------------------------------------------

    Element operator [] (unsigned int i) const {
        return elements[i];
    }

    Element& operator [] (unsigned int i) {
        return elements[i];
    }

};

} //lib_2d

#endif // TOPOLOGY_H_INCLUDED
