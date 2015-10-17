/*
    Copyright (c) 2014 - 2015 Martin Buck
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
 * \file    Path.h
 * \author  Martin Buck
 * \date    November 2014
 * \version 1.0
 * \brief   contains the class Point which represents a numerical point in 2d space
 *          Point provides several methods to work with
 */

#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <sstream>
#include <cmath>

namespace lib_2d {

template <typename T>
class Point {

public:
    T x, y;

//------------------------------------------------------------------------------

    Point& move_by(T x, T y) {
        this->x+=x;
        this->y+=y;
        return *this;
    }

    Point& move_by(const Point &other) {
        this->x+=other.x;
        this->y+=other.y;
        return *this;
    }

//------------------------------------------------------------------------------

    Point& mirror_vertically(T xValue = 0) {
        x = 2 * xValue - x;
        return *this;
    }

    Point& mirror_vertically(const Point &other) {
        mirror_vertically(other.x);
        return *this;
    }

    Point& mirror_horizontally(T yValue = 0) {
        y = 2 * yValue - y;
        return *this;
    }

    Point& mirror_horizontally(const Point &other) {
        mirror_horizontally(other.y);
        return *this;
    }

    Point& mirror_point(const Point &other) {
        mirror_vertically(other);
        mirror_horizontally(other);
        return *this;
    }

    Point& mirror_point(T xValue = 0, T yValue = 0) {
        mirror_point(Point{xValue, yValue});
        return *this;
    }


//------------------------------------------------------------------------------

    Point& rotate(T radians, Point center = Point{}) {
        T newX, newY;

        newX = center.x + cos(radians) * (x - center.x) - sin(radians) * (y - center.y);
        newY = center.y + sin(radians) * (x - center.y) + cos(radians) * (y - center.y);

        x = newX;
        y = newY;
        return *this;
    }

    Point& rotate(T radians, T centerX, T centerY) {
        rotate(radians, Point(centerX, centerY));
        return *this;
    }

//------------------------------------------------------------------------------

    std::string to_string(std::string divider = " ") const {
        std::stringstream ss;
        ss << x << divider << y;
        return ss.str();
    }

    bool from_string(const std::string &input, std::string divider = " ") {
        size_t pos = input.find(divider);
        std::stringstream ss;
        if(pos == std::string::npos)
            return false;
        ss << input.substr(0, pos);
        ss >> x;
        ss.str("");
        ss.clear();
        ss << input.substr(pos + divider.length());
        ss >> y;
        return true;
    }

//------------------------------------------------------------------------------

    T abs() const {
        return sqrt(  pow(x,2) + pow(y,2) );
    }

//------------------------------------------------------------------------------

    T phi() const {
        return atan2(y, x);
    }

//------------------------------------------------------------------------------

    T distance_to(const Point &other) const {
        return sqrt(  pow(x-other.x,2) + pow(y-other.y,2)  );
    }

    T distance_to(T x, T y) const {
        return distance_to(Point{x,y});
    }

    T sqr_distance_to(const Point &other) const {
        return pow(x-other.x,2) + pow(y-other.y,2);
    }

    T sqr_distance_to(T x, T y) const {
        return sqr_distance_to(Point{x,y});
    }

//------------------------------------------------------------------------------

    T slope_to(const Point &other) const {
        return (other.y - y) / (other.x - x);
    }

//------------------------------------------------------------------------------

    ///@todo rename to 'radians_to'
    T rad_to(const Point &other) const {
        return atan2(other.y - y, other.x - x);
    }

//------------------------------------------------------------------------------

    Point center_between(const Point &other) const {
        return Point{(T)(x + (other.x - x) / 2.0), (T)(y + (other.y - y) / 2.0)};
    }


//------------------------------------------------------------------------------

    bool similar_to(Point other, T maxDistance) const {
        if (distance_to(other) > maxDistance)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

    bool equal_to (const Point &other) const {
        if (x == other.x && y == other.y)
            return true;
        return false;
    }

//------------------------------------------------------------------------------

    bool operator == (const Point &other) const {
        return equal_to(other);
    }

    bool operator != (const Point &other) const {
        return !equal_to(other);
    }

    bool operator < (const Point &other) const {
        return x < other.x || (x == other.x && y < other.y);
    }

    friend std::ostream &operator << (std::ostream &os, Point point) {
        os << point.to_string();
        return os;
    }

    operator std::pair<T, T> () const {
        return std::pair<T, T> (x, y);
    }

    T& operator[](size_t idx) {
        if(idx == 0) return x;
        else if(idx == 1) return y;
        else throw std::out_of_range ("Point can only be accessed with index 0 or 1");
    };
    const T& operator[](size_t idx) const {
        if(idx == 0) return x;
        else if(idx == 1) return y;
        else throw std::out_of_range ("Point can only be accessed with index 0 or 1");
    };

//------------------------------------------------------------------------------

};

} //lib_2d


#endif // POINT_H_INCLUDED
