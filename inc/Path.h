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
 * \brief   contains the class Path which represents a collection of numerical points in 2d space
 *          the data is defined as a vector <Point>
 *          Path provides several methods to work with such a dataset
 */

#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED

#include <vector>
#include <set>
#include <fstream>
#include <algorithm>
#include <utility>

#include "Point.h"

namespace lib_2d {

template <typename T>
class Path {

protected:
    std::vector < Point <T> > ps;

    T ccw(const Point<T> &p1,const Point<T> &p2, const Point<T> &p3) const {
        return (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
    }

public:
    Path(){};

    Path(unsigned int nPoints) {
        reserve(nPoints);
    }

    template<class InputIterator>
    Path(InputIterator first, InputIterator last) {
        while(first != last) {
            ps.push_back(*first);
            ++first;
        }
    }

    Path(const std::vector < Point <T> > &points) :
        ps(points){}

    ~Path(){};

//------------------------------------------------------------------------------

    Path& move_by(T x, T y) {
        for(auto &p : ps)
            p.move_by(x, y);
        return *this;
    }

    Path& move_by(const Point<T> &other) {
        for(auto &p : ps)
            p.move_by(other);
        return *this;
    }

//------------------------------------------------------------------------------

    Path& mirror_vertically(T xValue = 0) {
        for(auto &p : ps)
            p.mirror_vertically(xValue);
        return *this;
    }

    Path& mirror_vertically(const Point<T> &other) {
        for(auto &p : ps)
            p.mirror_vertically(other);
        return *this;
    }

    Path& mirror_horizontally(T yValue = 0) {
        for(auto &p : ps)
            p.mirror_horizontally(yValue);
        return *this;
    }

    Path& mirror_horizontally(const Point<T> &other) {
        for(auto &p : ps)
            p.mirror_horizontally(other);
        return *this;
    }

    Path& mirror_point(const Point<T> &other) {
        for(auto &p : ps)
            p.mirror_point(other);
        return *this;
    }

    Path& mirror_point(T xValue = 0, T yValue = 0) {
        for(auto &p : ps)
            p.mirror_point(xValue, yValue);
        return *this;
    }

//------------------------------------------------------------------------------

    Path& rotate(T radians, Point<T> center = Point<T>{}) {
        for(auto &p : ps)
            p.rotate(radians, center);
        return *this;
    }

    Path& rotate(T radians, T centerX, T centerY) {
        for(auto &p : ps)
            p.rotate(radians, centerX, centerY);
        return *this;
    }

//------------------------------------------------------------------------------

    std::string to_string(std::string divider = " ") const {
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
            Point<T> point = Point<T>{};
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

    Path& push_back(Point<T> point) {
        ps.push_back(point);
        return *this;
    }

    Path& push_back(T x, T y) {
        push_back(Point<T>{x, y});
        return *this;
    }

    Path& push_back(const Path &other) {
        ps.reserve( ps.size() + other.size() );
        ps.insert( ps.end(), other.cbegin(), other.cend() );
        return *this;
    }

    Path& emplace_back(Point<T> point) {
        ps.emplace_back(point);
        return *this;
    }

    Path& emplace_back(T x, T y) {
        emplace_back(Point<T>{x, y});
        return *this;
    }

    Path& emplace_back(const Path &other) {
        ps.reserve( ps.size() + other.size() );
        ps.insert( ps.end(), other.cbegin(), other.cend() );
        return *this;
    }

//------------------------------------------------------------------------------

    Path& pop_back() {
        ps.pop_back();
        return *this;
    }

//------------------------------------------------------------------------------

    size_t size() const {
        return ps.size();
    }

//------------------------------------------------------------------------------

    T length() const {
        if(size() < 2)
            return 0;
        T l(0);

        for(auto i = ps.cbegin()+1; i != ps.cend(); ++i)
            l += i->distance_to(*(i-1));

        return l;
    }

//------------------------------------------------------------------------------

    Point<T> first() const {
        return ps[0];
    }

    Point<T> last() const {
        return ps[size()-1];
    }

//------------------------------------------------------------------------------

    T get_min_x() const {
        if(size() == 0)
            return 0; ///@todo find better error handling
        T minX(ps[0].x);

        for(auto p :ps) {
            if(p.x < minX)
                minX = p.x;
        }
        return minX;
    }

    T get_max_x() const {
        if(size() == 0)
            return 0; ///@todo find better error handling
        T maxX(ps[0].x);

        for(auto p :ps) {
            if(p.x > maxX)
                maxX = p.x;
        }
        return maxX;
    }

    T get_min_y() const {
        if(size() == 0)
            return 0; ///@todo find better error handling
        T minY(ps[0].y);

        for(auto p :ps) {
            if(p.y < minY)
                minY = p.y;
        }
        return minY;
    }

    T get_max_y() const {
        if(size() == 0)
            return 0; ///@todo find better error handling
        T maxY(ps[0].y);

        for(auto p :ps) {
            if(p.y > maxY)
                maxY = p.y;
        }
        return maxY;
    }

//------------------------------------------------------------------------------

    int get_min_x_index() const {
        if(size() == 0)
            return -1;
        T minX(ps[0].x);
        unsigned int index;

        for(unsigned int i = 0; i < size(); ++i) {
            if(ps[i].x < minX) {
                minX = ps[i].x;
                index = i;
            }
        }
        return index;
    }

    int get_max_x_index() const {
        if(size() == 0)
            return -1;
        T maxX(ps[0].x);
        unsigned int index;

        for(unsigned int i = 0; i < size(); ++i) {
            if(ps[i].x > maxX) {
                maxX = ps[i].x;
                index = i;
            }
        }
        return index;
    }

    int get_min_y_index() const {
        if(size() == 0)
            return -1;
        T minY(ps[0].y);
        unsigned int index;

        for(unsigned int i = 0; i < size(); ++i) {
            if(ps[i].y < minY) {
                minY = ps[i].y;
                index = i;
            }
        }
        return index;
    }

    int get_max_y_index() const {
        if(size() == 0)
            return -1;
        T maxY(ps[0].y);
        unsigned int index;

        for(unsigned int i = 0; i < size(); ++i) {
            if(ps[i].y > maxY) {
                maxY = ps[i].y;
                index = i;
            }
        }
        return index;
    }

//------------------------------------------------------------------------------

    Path bounding_box(bool closePath = true) const {
        if(size() <= 1)
            return *this;

        T minX = get_min_x();
        T maxX = get_max_x();
        T minY = get_min_y();
        T maxY = get_max_y();

        Path<T> output;
        output.emplace_back(Point<T>{minX, minY});
        output.emplace_back(Point<T>{maxX, minY});
        output.emplace_back(Point<T>{maxX, maxY});
        output.emplace_back(Point<T>{minX, maxY});

        if(closePath)
            output.push_back(output[0]);
        return output;
    }


//------------------------------------------------------------------------------

    //Andrew's monotone chain convex hull algorithm
    Path convex_hull(bool closePath = true) const {
        int n = size();
        Path<T> path = *this;

        std::sort(path.begin(), path.end());

        Path<T> lower;
        for (int i = 0; i < n; ++i) {
            while (lower.size() >= 2 && ccw(lower[lower.size()-2], lower[lower.size()-1], path[i]) <= 0)
                lower.pop_back();
            lower.push_back(path[i]);
        }

        Path<T> upper;
        for (int i = n-1; i >= 0; i--) {
            while (upper.size() >= 2 && ccw(upper[upper.size()-2], upper[upper.size()-1], path[i]) <= 0)
                upper.pop_back();
            upper.push_back(path[i]);
        }

        Path<T> output;
        output.push_back(lower);
        output.push_back(upper);
        output.make_unique();

        if(closePath)
            output.push_back(output[0]);
        return output;
    }

//------------------------------------------------------------------------------

#ifdef LIB_2D_EXPERIMENTAL
    bool point_is_inside(const Point<T> &point) {
        if(size() <= 2)
            return false;

        auto pointOutside = ps[get_min_y_index()]; //lowest point
        pointOutside.move_by(1, -1); //move down, certainly outside now

        Path<T> ray = Path<T>();
        ray += pointOutside;
        ray += point;

        auto intersections = intersections_with(ray);

        std::cout << pointOutside << std::endl;

        std::cout << intersections << std::endl;

        unsigned int nIntersections = intersections.size();

        if (nIntersections % 2)
            return true;
        else
            return false;
    }
#endif
//------------------------------------------------------------------------------

    Path& make_unique() { ///@todo use std::unique
        std::set <unsigned int> nonUniqueIndexes;
        for(unsigned int i = 0; i < size()-1; ++i) {
            for(unsigned int j = i+1; j < size(); ++j) {
                if((*this)[i] == (*this)[j]) {
                    nonUniqueIndexes.insert(j);
                }
            }
        }
        auto out = Path<T>();
        for(unsigned int i = 0; i < size(); ++i) {
            if(nonUniqueIndexes.find(i) == nonUniqueIndexes.end())
                out.push_back((*this)[i]);
        }
        *this = out;
        return *this;
    }
//------------------------------------------------------------------------------

    T average_distance() const {
        if(size() < 2)
            return 0;
        return length() / (size()-1);
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

    bool has_point(T x, T y) {
        return has_point(Point<T>{x, y});
    }

//------------------------------------------------------------------------------

    Path& reserve(size_t i) {
        ps.reserve(i);
        return *this;
    }

//------------------------------------------------------------------------------

    Path& clear() {
        ps.clear();
        return *this;
    }

//------------------------------------------------------------------------------

    Path& reverse() {
        std::reverse(ps.begin(), ps.end());
        return *this;
    }

//------------------------------------------------------------------------------

    Path& remove_from(unsigned int index) {
        if(size() < index)
            return *this;
        ps.erase(ps.begin() + index, ps.end());
        return *this;
    }

    Path& remove_until(unsigned int index) {
        if(size() < index)
            clear();
        else
            ps.erase(ps.begin(), ps.begin() + index);
        return *this;
    }

    Path& remove_right_of(T x) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->x > x) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
        return *this;
    }

    Path& remove_right_of(const Point<T> &other) {
        remove_right_of(other.x);
        return *this;
    }

    Path& remove_left_of(T x) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->x < x) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
        return *this;
    }

    Path& remove_left_of(const Point<T> &other) {
        remove_left_of(other.x);
        return *this;
    }

    Path& remove_above_of(T y) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->y > y) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
        return *this;
    }

    Path& remove_above_of(const Point<T> &other) {
        remove_above_of(other.y);
        return *this;
    }

    Path& remove_below_of(T y) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->y < y) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
        return *this;
    }

    Path& remove_below_of(const Point<T> &other) {
        remove_below_of(other.y);
        return *this;
    }

    Path& remove_closer_to_than(T distance, Point<T> other = Point<T>{}) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->distance_to(other) < distance) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
        return *this;
    }

    Path& remove_further_apart_to_than(T distance, Point<T> other = Point<T>{}) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->distance_to(other) > distance) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
        return *this;
    }

//------------------------------------------------------------------------------

    Point<T> center() const {
        T
            sumX(0.0),
            sumY(0.0);

        for(const auto &i : ps) {
            sumX += i.x;
            sumY += i.y;
        }

        return Point<T>{sumX / size(), sumY / size()};
    }

//------------------------------------------------------------------------------

    int furthest_apart(const Point<T> &other) const {
        T maxDistance(0);
        int furthestIndex(-1);
        for (unsigned int i = 0; i < size(); ++i) {
            if(ps[i].distance_to(other) >= maxDistance) {
                maxDistance = ps[i].distance_to(other);
                furthestIndex = i;
            }
        }
        return furthestIndex;
    }

    int furthest_apart(T x, T y) const {
        return furthest_apart(Point<T>{x, y});
    }

    int furthest_apart(const Path &other) const {
        T maxDistance(0);
        int furthestIndex(-1);

        for (unsigned int i = 0; i< size(); ++i) {
            T distance = ps[i].distance_to( other[other.closest(ps[i])] );
            if(maxDistance < distance) {
                maxDistance = distance;
                furthestIndex = i;
            }
        }
        return furthestIndex;
    }

//------------------------------------------------------------------------------

    int closest(const Point<T> &other) const {
        int closestIndex(-1);
        if(size() == 0)
            return closestIndex;
        T minDistance = ps[0].distance_to(other);
        for (unsigned int i = 0; i < size(); ++i) {
            if(ps[i].distance_to(other) <= minDistance) {
                minDistance = ps[i].distance_to(other);
                closestIndex = i;
            }
        }
        return closestIndex;
    }

    int closest(T x, T y) const {
        return closest(Point<T>{x, y});
    }

    int closest(const Path &other) const {
        int closestIndex(-1);
        if(size() == 0 || other.size() == 0)
            return closestIndex;
        T minDistance = ps[0].distance_to(other[0]);
        for (unsigned int i = 0; i< size(); ++i) {
            T distance = ps[i].distance_to( other[other.closest(ps[i])] );
            if(minDistance > distance) {
                minDistance = distance;
                closestIndex = i;
            }
        }
        return closestIndex;
    }

//------------------------------------------------------------------------------

    bool similar_to(const Path &other, T maxDistance) const {
        if(size() != other.size())
            return false;
        for(unsigned int i = 0; i < size(); ++i) {
            if(!ps[i].similar_to(other[i], maxDistance))
                return false;
        }
        return true;
    }

//------------------------------------------------------------------------------

    bool equal_to (const Path &other) const {
        if(size() != other.size())
            return false;
        for(unsigned int i = 0; i < size(); ++i) {
            if(!ps[i].equal_to(other[i]))
                return false;
        }
        return true;
    }

//------------------------------------------------------------------------------

    int index_of(const Point<T> &other) {
        for(auto p = ps.cbegin(); p != ps.cend(); ++p) {
                if (*p == other)
                    return (p - ps.cbegin());
        }
        return -1;
    }

//------------------------------------------------------------------------------

    //this method should be kept similar to "intersects_with"
    Path intersections_with(const Path &other) const {

        Path intersections;

        if(size() < 2 || other.size() < 2)
            return intersections;

        if(!intersects_with(other)) //faster than checking for intersections
            return intersections;

        for(auto i = ps.cbegin(); i != ps.cend()-1; ++i) {
            for(auto j = other.cbegin(); j != other.cend()-1; ++j)
                intersections.push_back(calc_intersections(*i, *(i+1), *j, *(j+1)));
        }
        return intersections;
    }

//------------------------------------------------------------------------------

    //this method should be kept similar to "intersections_with"
    bool intersects_with(const Path &other) const {

        if(size() < 2 || other.size() < 2)
            return false;

        if(size() > 5 && other.size() > 5) { //prevent endless recursion
            auto boundingBoxThis = bounding_box();
            auto boundingBoxOther = other.bounding_box();

            if(!boundingBoxThis.intersects_with(boundingBoxOther))
                return false; //only can intersect if boundig boxes intersect
        }

        for(auto i = ps.cbegin(); i != ps.cend()-1; ++i) {
            for(auto j = other.cbegin(); j != other.cend()-1; ++j) {
                Path<T> intersections;
                intersections.push_back(calc_intersections(*i, *(i+1), *j, *(j+1)));
                if(intersections.size() > 0)
                    return true;
            }
        }
        return false;
    }


//------------------------------------------------------------------------------

    Path& sort_x() {
        if(!empty())
            sort(ps.begin(), ps.end(), compare_x);
        return *this;
    }

    Path& sort_y() {
        if(!empty())
            sort(ps.begin(), ps.end(), compare_y);
        return *this;
    }

//------------------------------------------------------------------------------

    Path& range(unsigned int indexStart, unsigned int indexEnd) {
        if(indexStart > indexEnd)
            return *this;

        if(indexStart >= size() || indexEnd >= size())
            return *this;

        if(indexStart == 0 && indexEnd == size()-1)
            return *this;

        Path<T> tmp;

        for(unsigned int i = indexStart; i <= indexEnd; ++i) {
            tmp += (*this)[i];
        }

        *this = tmp;
        return *this;
    }

//------------------------------------------------------------------------------

    Path& reduce_points(T epsilon) {
        *this = douglas_peucker(*this, epsilon);
        return *this;
    }

//------------------------------------------------------------------------------

    typename std::vector <Point<T> >::iterator begin() {
        return ps.begin();
    }

    typename std::vector <Point<T> >::iterator end() {
        return ps.end();
    }

    typename std::vector <Point<T> >::const_iterator cbegin() const {
        return ps.cbegin();
    }

    typename std::vector <Point<T> >::const_iterator cend() const {
        return ps.cend();
    }

    typename std::vector <Point<T> >::reverse_iterator rbegin() {
        return ps.rbegin();
    }

    typename std::vector <Point<T> >::reverse_iterator rend() {
        return ps.rend();
    }

//------------------------------------------------------------------------------

    bool operator == (const Path &other) const {
        return equal_to(other);
    }

    bool operator != (const Path &other) const {
        return !equal_to(other);
    }

    Path<T>& operator += (const Path<T> &other) {
        push_back(other);
        return *this;
    }

    Path<T>& operator += (Point<T> other) {
        push_back(other);
        return *this;
    }

    Path<T> operator + (const Path<T> &other) const {
        auto out = *this;
        out.push_back(other);
        return out;
    }

    Path<T> operator + (Point<T> other) const {
        auto out = *this;
        out.push_back(other);
        return out;
    }

    Point<T> operator [] (unsigned int i) const {
        return ps[i];
    }

    Point<T>& operator [] (unsigned int i) {
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

private:

    static bool compare_x(const Point<T> &lhs, const Point<T> &rhs) {
        return lhs.x < rhs.x;
    }

    static bool compare_y(const Point<T> &lhs, const Point<T> &rhs) {
        return lhs.y < rhs.y;
    }

    //using http://en.wikipedia.org/wiki/Ramer%E2%80%93Douglas%E2%80%93Peucker_algorithm
    Path<T> douglas_peucker(Path<T> path, T epsilon) const { ///@todo rename variables to make it more readable
        T dmax = 0;
        unsigned int index = 0;
        unsigned int end = path.size()-1;

        for(unsigned int i=1; i < end; ++i) {
            T d = distance_point_line(path[i], path[0], path[end]);
            if(d > dmax) {
                index = i;
                dmax = d;
            }
        }
        if(dmax > epsilon) {
            auto path1 = path;
            auto path2 = path;

            path1.range(0,index);
            path1 = douglas_peucker(path1, epsilon);

            path2.range(index,end);
            path2 = douglas_peucker(path2, epsilon);

            path1.range(0,path1.size()-2);
            path = path1 + path2;
        }
        else {
            auto p1 = path[0];
            auto p2 = path[end];
            path.clear();
            path += p1;
            path += p2;
        }
        return path;
    }
};

} //lib_2d

#endif // POINT_H_INCLUDED
