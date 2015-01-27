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

///@todo uncomment once running
/*
    Point(std::pair<T, T> values) :
        x(values.first),
        y(values.second)
    {}
*/
//------------------------------------------------------------------------------

    void move_by(T x, T y) {
        this->x+=x;
        this->y+=y;
    }

    void move_by(Point other) {
        this->x+=other.x;
        this->y+=other.y;
    }

//------------------------------------------------------------------------------

    void mirror_vertically(T xValue = 0) {
        x = 2 * xValue - x;
    }

    void mirror_vertically(Point other) {
        mirror_vertically(other.x);
    }

    void mirror_horizontally(T yValue = 0) {
        y = 2 * yValue - y;
    }

    void mirror_horizontally(Point other) {
        mirror_horizontally(other.y);
    }

    void mirror_point(Point other) {
        mirror_vertically(other);
        mirror_horizontally(other);
    }

    void mirror_point(T xValue = 0, T yValue = 0) {
        mirror_point(Point{xValue, yValue});
    }


//------------------------------------------------------------------------------

    void rotate(T radians, Point center = Point{}) {
        T newX, newY;

        newX = center.x + cos(radians) * (x - center.x) - sin(radians) * (y - center.y);
        newY = center.y + sin(radians) * (x - center.y) + cos(radians) * (y - center.y);

        x = newX;
        y = newY;
    }

    void rotate(T radians, T centerX, T centerY) {
        rotate(radians, Point(centerX, centerY));
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

    T distance_to(Point other) const {
        return sqrt(  pow(x-other.x,2) + pow(y-other.y,2)  );
    }

    T distance_to(T x, T y) const {
        return distance_to(Point{x,y});
    }

//------------------------------------------------------------------------------

    T slope_to(Point other) const {
        return (other.y - y) / (other.x - x);
    }

//------------------------------------------------------------------------------

    ///@todo rename to 'radians_to'
    T rad_to(Point other) const {
        return atan2(other.y - y, other.x - x);
    }

//------------------------------------------------------------------------------

    Point center_between(Point other) const {
        return Point{x + (other.x - x) / 2.0, y + (other.y - y) / 2.0};
    }


//------------------------------------------------------------------------------

    bool similar_to(Point other, T maxDistance) const {
        if (distance_to(other) > maxDistance)
            return false;
        return true;
    }

//------------------------------------------------------------------------------

    bool equal_to (Point other) const {
        if (x == other.x && y == other.y)
            return true;
        return false;
    }

//------------------------------------------------------------------------------

    bool operator == (Point other) const {
        return equal_to(other);
    }

    bool operator != (Point other) const {
        return !equal_to(other);
    }

    bool operator < (Point other) const {
        return x < other.x || (x == other.x && y < other.y);
    }

    friend std::ostream &operator << (std::ostream &os, Point point) {
        os << point.to_string();
        return os;
    }

    operator std::pair<T, T> () const {
        return std::pair<T, T> (x, y);
    }

//------------------------------------------------------------------------------

};

} //lib_2d


#endif // POINT_H_INCLUDED
