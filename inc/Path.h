/*
    Copyright (c) 2014 Martin Buck
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

#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED

#include <vector>
#include <fstream>

#include "Point.h"

/*
    missing:
        intersections (with sweep?)
        sorting
        closest / furthest
        average point distance
        convex hull
        index of point
        remove after/before  above/below  left/right
        iteration overloads
*/

namespace lib_2d {

template <typename T>
class Path {

private:
    std::vector < Point <T> > ps;

public:
    Path(){};
    Path(const std::vector < Point <T> > &points) :
        ps(points){}

    ~Path(){};

//------------------------------------------------------------------------------

    void move_by(const T &x, const T &y) {
        for(auto &p : ps)
            p.move_by(x, y);
    }

    void move_by(const Point<T> &other) {
        for(auto &p : ps)
            p.move_by(other);
    }

//------------------------------------------------------------------------------

    void mirror_vertically(const T &xValue = 0) {
        for(auto &p : ps)
            p.mirror_vertically(xValue);
    }

    void mirror_vertically(const Point<T> &other) {
        for(auto &p : ps)
            p.mirror_vertically(other);
    }

    void mirror_horizontally(const T &yValue = 0) {
        for(auto &p : ps)
            p.mirror_horizontally(yValue);
    }

    void mirror_horizontally(const Point<T> &other) {
        for(auto &p : ps)
            p.mirror_horizontally(other);
    }

    void mirror_point(const Point<T> &other) {
        for(auto &p : ps)
            p.mirror_point(other);
    }

    void mirror_point(const T &xValue = 0, const T &yValue = 0) {
        for(auto &p : ps)
            p.mirror_point(xValue, yValue);
    }

//------------------------------------------------------------------------------

    void rotate(const T &radians, const Point<T> &center = Point<T>(0, 0)) {
        for(auto &p : ps)
            p.rotate(radians, center);
    }

    void rotate(const T &radians, const T &centerX, const T &centerY) {
        for(auto &p : ps)
            p.rotate(radians, centerX, centerY);
    }

//------------------------------------------------------------------------------

    std::string to_string(const std::string &divider = " ") const {
        std::string output("");

        for(const auto &p : ps)
                output += p.to_string(divider) + "\n";

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

    bool from_string(const std::string &input) {
        clear();
        std::stringstream ss(input);
        std::string line("");
        while(getline(ss, line)) {
            Point<T> point = Point<T>();
            if(point.from_string(line))
                ps.push_back(point);
        }
        if(size() == 0)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

    bool from_file(const std::string &path) {
        std::ifstream in(path.c_str());
        if(!in.good())
            return false;
        std::stringstream buffer;
        buffer << in.rdbuf();
        in.close();
        return from_string(buffer.str());
    }

//------------------------------------------------------------------------------

    void push_back(const Point<T> &point) {
        ps.push_back(point);
    }

    void push_back(const T &x, const T &y) {
        push_back(Point<T>(x, y));
    }

//------------------------------------------------------------------------------

    size_t size() const {
        return ps.size();
    }

//------------------------------------------------------------------------------

    T length() const {
        T l(0);

        for(const auto &i = ps.cbegin()+1; i != ps.cend(); ++i)
            l += i->distance_to(*(i-1));

        return l;
    }

//------------------------------------------------------------------------------

    bool empty() const {
        return ps.empty();
    }

//------------------------------------------------------------------------------

    bool has_point(const Point<T> &point) {
        for(const auto &p : ps) {
            if(p.equal_to(point))
                return true;
        }
        return false;
    }

//------------------------------------------------------------------------------

    bool has_point(const T &x, const T &y) {
        return has_point(Point<T>(x, y));
    }

//------------------------------------------------------------------------------

    void reserve(const size_t &i) {
        ps.reserve(i);
    }

//------------------------------------------------------------------------------

    void clear() {
        ps.clear();
    }

//------------------------------------------------------------------------------

    void reverse() {
        ps.reverse();
    }

//------------------------------------------------------------------------------

    void flip() {
        ps.flip();
    }

//------------------------------------------------------------------------------

    Point<T> center() const {
        double
            sumX(0.0),
            sumY(0.0);

        for(const auto &i : ps) {
            sumX += i.get_x();
            sumY += i.get_y();
        }

        return Point<T>(sumX / size(), sumY / size());
    }

//------------------------------------------------------------------------------

    bool similar_to(const Path &other, const T &maxDistance) const {
        for(unsigned int i = 0; i < size(); ++i) {
            if(!ps[i].similar_to(other[i], maxDistance))
                return false;
        }
        return true;
    }

//------------------------------------------------------------------------------

    bool equal_to (const Path &other) const {
        for(unsigned int i = 0; i < size(); ++i) {
            if(!ps[i].equal_to(other[i]))
                return false;
        }
        return false;
    }

//------------------------------------------------------------------------------

    bool operator == (const Path &other) const {
        return equal_to(other);
    }

    bool operator != (const Path &other) const {
        return !equal_to(other);
    }


    Point<T> operator [] (const unsigned int &i) const
    {
        return ps[i];
    }

    Point<T>& operator [] (const unsigned int &i)
    {
        return ps[i];
    }

    operator std::vector < Point <T> > () const {
        return ps;
    }

//------------------------------------------------------------------------------

    friend std::ostream &operator << (std::ostream &os, const Path &path) {
        os << path.to_string();
        return os;
    }

};

} //lib_2d

#endif // POINT_H_INCLUDED
