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

    T ccw(Point<T> p1,Point<T> p2, Point<T> p3) const {
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

    void move_by(T x, T y) {
        for(auto &p : ps)
            p.move_by(x, y);
    }

    void move_by(Point<T> other) {
        for(auto &p : ps)
            p.move_by(other);
    }

//------------------------------------------------------------------------------

    void mirror_vertically(T xValue = 0) {
        for(auto &p : ps)
            p.mirror_vertically(xValue);
    }

    void mirror_vertically(Point<T> other) {
        for(auto &p : ps)
            p.mirror_vertically(other);
    }

    void mirror_horizontally(T yValue = 0) {
        for(auto &p : ps)
            p.mirror_horizontally(yValue);
    }

    void mirror_horizontally(Point<T> other) {
        for(auto &p : ps)
            p.mirror_horizontally(other);
    }

    void mirror_point(Point<T> other) {
        for(auto &p : ps)
            p.mirror_point(other);
    }

    void mirror_point(T xValue = 0, T yValue = 0) {
        for(auto &p : ps)
            p.mirror_point(xValue, yValue);
    }

//------------------------------------------------------------------------------

    void rotate(T radians, Point<T> center = Point<T>{}) {
        for(auto &p : ps)
            p.rotate(radians, center);
    }

    void rotate(T radians, T centerX, T centerY) {
        for(auto &p : ps)
            p.rotate(radians, centerX, centerY);
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

    void push_back(Point<T> point) {
        ps.push_back(point);
    }

    void push_back(T x, T y) {
        push_back(Point<T>{x, y});
    }

    void push_back(const Path &other) {
        for(auto i = other.cbegin(); i != other.cend(); ++i)
            push_back(*i);
    }

    void emplace_back(Point<T> point) {
        ps.emplace_back(point);
    }

    void emplace_back(T x, T y) {
        emplace_back(Point<T>{x, y});
    }

    void emplace_back(const Path &other) {
        for(auto i = other.cbegin(); i != other.cend(); ++i)
            emplace_back(*i);
    }

//------------------------------------------------------------------------------

    void pop_back() {
        ps.pop_back();
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

    Path bounding_box(bool closePath = true) const {
        if(size() <= 1)
            return *this;

        T   minX(ps[0].x),
            maxX(ps[0].x),
            minY(ps[0].y),
            maxY(ps[0].y);

        for(auto p : ps) {
            if(p.x < minX)
                minX = p.x;
            else if(p.x > maxX)
                maxX = p.x;

            if(p.y < minY)
                minY = p.y;
            else if(p.y > maxY)
                maxY = p.y;
        }

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

    void make_unique() {
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

    bool has_point(Point<T> point) {
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

    void reserve(size_t i) {
        ps.reserve(i);
    }

//------------------------------------------------------------------------------

    void clear() {
        ps.clear();
    }

//------------------------------------------------------------------------------

    void reverse() {
        std::reverse(ps.begin(), ps.end());
    }

//------------------------------------------------------------------------------

    void remove_from(unsigned int index) {
        if(size() < index)
            return;
        ps.erase(ps.begin() + index, ps.end());
    }

    void remove_until(unsigned int index) {
        if(size() < index)
            clear();
        else
            ps.erase(ps.begin(), ps.begin() + index);
    }

    void remove_right_of(T x) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->x > x) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
    }

    void remove_right_of(Point<T> other) {
        remove_right_of(other.x);
    }

    void remove_left_of(T x) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->x < x) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
    }

    void remove_left_of(Point<T> other) {
        remove_left_of(other.x);
    }

    void remove_above_of(T y) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->y > y) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
    }

    void remove_above_of(Point<T> other) {
        remove_above_of(other.y);
    }

    void remove_below_of(T y) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->y < y) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
    }

    void remove_below_of(Point<T> other) {
        remove_below_of(other.y);
    }

    void remove_closer_to_than(T distance, Point<T> other = Point<T>{}) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->distance_to(other) < distance) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
    }

    void remove_further_apart_to_than(T distance, Point<T> other = Point<T>{}) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->distance_to(other) > distance) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
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

    int furthest_apart(Point<T> other) const {
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

    int closest(Point<T> other) const {
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
        return false;
    }

//------------------------------------------------------------------------------

    int index_of(Point<T> other) {
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

        if(empty() || other.empty())
            return intersections;

        if(!intersects_with(other)) //faster than checking for intersections
            return intersections;

        for(auto i1 = ps.cbegin(); i1 != ps.cend()-1; ++i1) {
            bool path1vertical(false), path1horizontal(false);
            T slope1(0.0), slope2(0.0);
            Point<T> currentIntersection;

            if((i1+1)->x == i1->x)
                path1vertical = true;
            else
                slope1 = i1->slope_to(*(i1+1));

            if((i1+1)->y == i1->y)
                path1horizontal = true;

            for(auto i2 = other.cbegin(); i2 != other.cend()-1; ++i2) {
                T path2vertical(false), path2horizontal(false);

                if((i2+1)->x == i2->x) {
                    if(path1vertical) continue;
                    path2vertical = true;
                }

                if((i2+1)->y == i2->y) {
                    if(path1horizontal) continue;
                    path2horizontal = true;
                }

                if(path1vertical) {
                    currentIntersection.x = i1->x;
                    if(path2horizontal)
                        currentIntersection.y = i2->y;
                    else
                        currentIntersection.y =
                                                  i2->slope_to(*(i2+1))
                                                  * currentIntersection.x
                                                  + ( (i2+1)->x * i2->y - i2->x * (i2+1)->y )
                                                  / ( (i2+1)->x - i2->x )
                                                  ;
                }
                else if(path2vertical) {
                    currentIntersection.x = i2->x;
                    if(path1horizontal)
                        currentIntersection.y = i1->y;
                    else
                        currentIntersection.y =
                                                  i1->slope_to(*(i1+1))
                                                  * currentIntersection.x
                                                  + ( (i1+1)->x * i1->y - i1->x * (i1+1)->y )
                                                  / ( (i2+1)->x - i2->x )
                                                  ;
                }
                else {
                    slope2 = i2->slope_to(*(i2+1));
                    currentIntersection.x =  (i2->y - i1->y + slope1 * i1->x - slope2 * i2->x)  /  (slope1 - slope2) ;
                    currentIntersection.y = slope1 * (currentIntersection.x - i1->x) + i1->y;
                }

                if(     ( ((i2+1)->x >= currentIntersection.x && i2->x <= currentIntersection.x )
                           || (i2->x >= currentIntersection.x && ((i2+1)->x <= currentIntersection.x))
                        )
                        && ( ((i1+1)->x >= currentIntersection.x && i1->x <= currentIntersection.x )
                            || (i1->x >= currentIntersection.x && ((i1+1)->x <= currentIntersection.x))
                        )
                   )
                    intersections.push_back(currentIntersection);
            }
        }
        return intersections;
    }

//------------------------------------------------------------------------------

    //this method should be kept similar to "intersections_with"
    bool intersects_with(const Path &other) const {

        if(empty() || other.empty())
            return false;

        if(size() > 5 && other.size() > 5) { //prevent endless recursion
            auto boundingBoxThis = bounding_box();
            auto boundingBoxOther = other.bounding_box();

            if(!boundingBoxThis.intersects_with(boundingBoxOther))
                return false; //only can intersect if boundig boxes intersect
        }

        for(auto i1 = ps.cbegin(); i1 != ps.cend()-1; ++i1) {
            bool path1vertical(false), path1horizontal(false);
            T slope1(0.0), slope2(0.0);
            Point<T> currentIntersection;

            if((i1+1)->x == i1->x)
                path1vertical = true;
            else
                slope1 = i1->slope_to(*(i1+1));

            if((i1+1)->y == i1->y)
                path1horizontal = true;

            for(auto i2 = other.cbegin(); i2 != other.cend()-1; ++i2) {
                T path2vertical(false), path2horizontal(false);

                if((i2+1)->x == i2->x) {
                    if(path1vertical) continue;
                    path2vertical = true;
                }

                if((i2+1)->y == i2->y) {
                    if(path1horizontal) continue;
                    path2horizontal = true;
                }

                if(path1vertical) {
                    currentIntersection.x = i1->x;
                    if(path2horizontal)
                        currentIntersection.y = i2->y;
                    else
                        currentIntersection.y =
                                                  i2->slope_to(*(i2+1))
                                                  * currentIntersection.x
                                                  + ( (i2+1)->x * i2->y - i2->x * (i2+1)->y )
                                                  / ( (i2+1)->x - i2->x )
                                                  ;
                }
                else if(path2vertical) {
                    currentIntersection.x = i2->x;
                    if(path1horizontal)
                        currentIntersection.y = i1->y;
                    else
                        currentIntersection.y =
                                                  i1->slope_to(*(i1+1))
                                                  * currentIntersection.x
                                                  + ( (i1+1)->x * i1->y - i1->x * (i1+1)->y )
                                                  / ( (i2+1)->x - i2->x )
                                                  ;
                }
                else {
                    slope2 = i2->slope_to(*(i2+1));
                    currentIntersection.x = (i2->y - i1->y + slope1 * i1->x - slope2 * i2->x)  /  (slope1 - slope2);
                    currentIntersection.y = slope1 * (currentIntersection.x - i1->x) + i1->y;
                }

                if(     ( ((i2+1)->x >= currentIntersection.x && i2->x <= currentIntersection.x )
                           || (i2->x >= currentIntersection.x && ((i2+1)->x <= currentIntersection.x))
                        )
                        && ( ((i1+1)->x >= currentIntersection.x && i1->x <= currentIntersection.x )
                            || (i1->x >= currentIntersection.x && ((i1+1)->x <= currentIntersection.x))
                        )
                   )
                    return true;
            }
        }
        return false;
    }


//------------------------------------------------------------------------------

    void sort_x() {
        if(!empty())
            sort(ps.begin(), ps.end(), compare_x);
    }

    void sort_y() {
        if(!empty())
            sort(ps.begin(), ps.end(), compare_y);
    }

//------------------------------------------------------------------------------

    void range(unsigned int indexStart, unsigned int indexEnd) {
        if(indexStart > indexEnd)
            return;

        if(indexStart >= size() || indexEnd >= size())
            return;

        if(indexStart == 0 && indexEnd == size()-1)
            return;

        Path<T> tmp;

        for(unsigned int i = indexStart; i <= indexEnd; ++i) {
            tmp += (*this)[i];
        }

        *this = tmp;
    }

//------------------------------------------------------------------------------

    void reduce_points(T epsilon) {
        *this = douglas_peucker(*this, epsilon);
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

    static bool compare_x(Point<T> lhs, Point<T> rhs) {
        return lhs.x < rhs.x;
    }

    static bool compare_y(Point<T> lhs, Point<T> rhs) {
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
