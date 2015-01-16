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
#include <map>
#include <fstream>
#include <algorithm>
#include <deque>
#include <forward_list>
#include <list>
#include <queue>
#include <stack>

#include "Point.h"

namespace lib_2d {

template <typename T>
class Path {

protected:
    std::vector < Point <T> > ps;

public:
    Path(){};

    Path(const unsigned int &nPoints) {
        reserve(nPoints);
    }

    Path(const std::vector < Point <T> > &points) :
        ps(points){}

    Path(const std::deque < Point <T> > &points) {
        for(const auto &i : points)
            ps.push_back(i);
    }

    Path(const std::forward_list < Point <T> > &points) {
        for(const auto &i : points)
            ps.push_back(i);
    }

    Path(const std::list < Point <T> > &points) {
        for(const auto &i : points)
            ps.push_back(i);
    }

    Path(std::queue < Point <T> > points) {
        while(!points.empty()) {
            ps.push_back(points.front());
            points.pop();
        }
    }

    Path(std::stack < Point <T> > points) {
        while(!points.empty()) {
            ps.push_back(points.top());
            points.pop();
        }
        reverse();
    }

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

    void push_back(const Path &other) {
        for(auto i = other.cbegin(); i != other.cend(); ++i)
            push_back(*i);
    }

    void emplace_back(const Point<T> &point) {
        ps.emplace_back(point);
    }

    void emplace_back(const T &x, const T &y) {
        emplace_back(Point<T>(x, y));
    }

    void emplace_back(const Path &other) {
        for(auto i = other.cbegin(); i != other.cend(); ++i)
            emplace_back(*i);
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
    Path convex_hull() const {
        if(size() < 2)
            return *this;
        Path convexHull = *this;

        unsigned int yMinIndex = 0;
        T yMin = convexHull[0].get_y();

        for(unsigned int i = 0; i < convexHull.size(); ++i) {
            if (convexHull[i].get_y() < yMin
            || ( convexHull[i].get_y() == yMin && convexHull[i].get_x() < convexHull[yMinIndex].get_x())) {
                yMin = convexHull[i].get_y();
                yMinIndex = i;
            }
        }

        swap(convexHull[1], convexHull[yMinIndex]);

        std::vector <T> radians;
        for(auto i : convexHull)
            radians.push_back( convexHull[1].rad_to(i));

        ///@todo this can lead to lost points, needs a better way to do this
        std::multimap< T,Point<T> > sorted;
        for(unsigned int i = 0; i < convexHull.size(); ++i)
            sorted.emplace_back(std::pair< T,Point<T> >(radians[i], convexHull[i]));
        convexHull.clear();
        for(auto i : sorted)
            convexHull += i.second;

        convexHull[0] = convexHull[convexHull.size()-1];
        unsigned int M =1;
        unsigned int N = size();
        for(unsigned int i=2; i < N; ++i) {
            while(true) {
                T orientation = (convexHull[M].get_x() - convexHull[M].get_x())
                            * (convexHull[i].get_y() - convexHull[M].get_y())
                            - (convexHull[M].get_y() - convexHull[M-1].get_y())
                            * (convexHull[i].get_x() - convexHull[M-1].get_x());

                if(orientation > 0) break;
                if(M > 1) --M;
                else if(i == N) break;
                else ++i;
            }
            ++M;
            convexHull[M] = convexHull[i];
        }
        return convexHull;
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
        std::reverse(ps.begin(), ps.end());
    }

//------------------------------------------------------------------------------

    void remove_from(const unsigned int &index) {
        if(size() < index)
            return;
        ps.erase(ps.begin() + index, ps.end());
    }

    void remove_until(const unsigned int &index) {
        if(size() < index)
            clear();
        else
            ps.erase(ps.begin(), ps.begin() + index);
    }

    void remove_right_of(const T &x) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->get_x() > x) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
    }

    void remove_right_of(const Point<T> &other) {
        remove_right_of(other.get_x());
    }

    void remove_left_of(const T &x) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->get_x() < x) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
    }

    void remove_left_of(const Point<T> &other) {
        remove_left_of(other.get_x());
    }

    void remove_above_of(const T &y) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->get_y() > y) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
    }

    void remove_above_of(const Point<T> &other) {
        remove_above_of(other.get_y());
    }

    void remove_below_of(const T &y) {
        for(auto i = ps.begin(); i!= ps.end();) {
            bool deleted(false);
            if(i->get_y() < y) {
                deleted = true;
                i = ps.erase(i);
            }
            if(!deleted)
                ++i;
        }
    }

    void remove_below_of(const Point<T> &other) {
        remove_below_of(other.get_y());
    }

    void remove_closer_to_than(const T &distance, const Point<T> &other = Point<T>(0, 0)) {
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

    void remove_further_apart_to_than(const T &distance, const Point<T> &other = Point<T>(0, 0)) {
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
            sumX += i.get_x();
            sumY += i.get_y();
        }

        return Point<T>(sumX / size(), sumY / size());
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

    int furthest_apart(const T &x, const T &y) const {
        return furthest_apart(Point<T>(x, y));
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

    int closest(const T &x, const T &y) const {
        return closest(Point<T>(x, y));
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

    bool similar_to(const Path &other, const T &maxDistance) const {
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

        if(empty() || other.empty())
            return intersections;

        for(auto i1 = ps.cbegin(); i1 != ps.cend()-1; ++i1) {
            bool path1vertical(false), path1horizontal(false);
            T slope1(0.0), slope2(0.0);
            Point<T> currentIntersection;

            if((i1+1)->get_x() == i1->get_x())
                path1vertical = true;
            else
                slope1 = i1->slope_to(*(i1+1));

            if((i1+1)->get_y() == i1->get_y())
                path1horizontal = true;

            for(auto i2 = other.cbegin(); i2 != other.cend()-1; ++i2) {
                T path2vertical(false), path2horizontal(false);

                if((i2+1)->get_x() == i2->get_x()) {
                    if(path1vertical) continue;
                    path2vertical = true;
                }

                if((i2+1)->get_y() == i2->get_y()) {
                    if(path1horizontal) continue;
                    path2horizontal = true;
                }

                if(path1vertical) {
                    currentIntersection.set_x(i1->get_x());
                    if(path2horizontal)
                        currentIntersection.set_y(i2->get_y());
                    else
                        currentIntersection.set_y(
                                                  i2->slope_to(*(i2+1))
                                                  * currentIntersection.get_x()
                                                  + ( (i2+1)->get_x() * i2->get_y() - i2->get_x() * (i2+1)->get_y() )
                                                  / ( (i2+1)->get_x() - i2->get_x() )
                                                  );
                }
                else if(path2vertical) {
                    currentIntersection.set_x(i2->get_x());
                    if(path1horizontal)
                        currentIntersection.set_y(i1->get_y());
                    else
                        currentIntersection.set_y(
                                                  i1->slope_to(*(i1+1))
                                                  * currentIntersection.get_x()
                                                  + ( (i1+1)->get_x() * i1->get_y() - i1->get_x() * (i1+1)->get_y() )
                                                  / ( (i2+1)->get_x() - i2->get_x() )
                                                  );
                }
                else {
                    slope2 = i2->slope_to(*(i2+1));
                    currentIntersection.set_x( (i2->get_y() - i1->get_y() + slope1 * i1->get_x() - slope2 * i2->get_x())  /  (slope1 - slope2) );
                    currentIntersection.set_y( slope1 * (currentIntersection.get_x() - i1->get_x()) + i1->get_y());
                }

                if(     ( ((i2+1)->get_x() >= currentIntersection.get_x() && i2->get_x() <= currentIntersection.get_x() )
                           || (i2->get_x() >= currentIntersection.get_x() && ((i2+1)->get_x() <= currentIntersection.get_x()))
                        )
                        && ( ((i1+1)->get_x() >= currentIntersection.get_x() && i1->get_x() <= currentIntersection.get_x() )
                            || (i1->get_x() >= currentIntersection.get_x() && ((i1+1)->get_x() <= currentIntersection.get_x()))
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

        for(auto i1 = ps.cbegin(); i1 != ps.cend()-1; ++i1) {
            bool path1vertical(false), path1horizontal(false);
            T slope1(0.0), slope2(0.0);
            Point<T> currentIntersection;

            if((i1+1)->get_x() == i1->get_x())
                path1vertical = true;
            else
                slope1 = i1->slope_to(*(i1+1));

            if((i1+1)->get_y() == i1->get_y())
                path1horizontal = true;

            for(auto i2 = other.cbegin(); i2 != other.cend()-1; ++i2) {
                T path2vertical(false), path2horizontal(false);

                if((i2+1)->get_x() == i2->get_x()) {
                    if(path1vertical) continue;
                    path2vertical = true;
                }

                if((i2+1)->get_y() == i2->get_y()) {
                    if(path1horizontal) continue;
                    path2horizontal = true;
                }

                if(path1vertical) {
                    currentIntersection.set_x(i1->get_x());
                    if(path2horizontal)
                        currentIntersection.set_y(i2->get_y());
                    else
                        currentIntersection.set_y(
                                                  i2->slope_to(*(i2+1))
                                                  * currentIntersection.get_x()
                                                  + ( (i2+1)->get_x() * i2->get_y() - i2->get_x() * (i2+1)->get_y() )
                                                  / ( (i2+1)->get_x() - i2->get_x() )
                                                  );
                }
                else if(path2vertical) {
                    currentIntersection.set_x(i2->get_x());
                    if(path1horizontal)
                        currentIntersection.set_y(i1->get_y());
                    else
                        currentIntersection.set_y(
                                                  i1->slope_to(*(i1+1))
                                                  * currentIntersection.get_x()
                                                  + ( (i1+1)->get_x() * i1->get_y() - i1->get_x() * (i1+1)->get_y() )
                                                  / ( (i2+1)->get_x() - i2->get_x() )
                                                  );
                }
                else {
                    slope2 = i2->slope_to(*(i2+1));
                    currentIntersection.set_x( (i2->get_y() - i1->get_y() + slope1 * i1->get_x() - slope2 * i2->get_x())  /  (slope1 - slope2) );
                    currentIntersection.set_y( slope1 * (currentIntersection.get_x() - i1->get_x()) + i1->get_y());
                }

                if(     ( ((i2+1)->get_x() >= currentIntersection.get_x() && i2->get_x() <= currentIntersection.get_x() )
                           || (i2->get_x() >= currentIntersection.get_x() && ((i2+1)->get_x() <= currentIntersection.get_x()))
                        )
                        && ( ((i1+1)->get_x() >= currentIntersection.get_x() && i1->get_x() <= currentIntersection.get_x() )
                            || (i1->get_x() >= currentIntersection.get_x() && ((i1+1)->get_x() <= currentIntersection.get_x()))
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

    operator std::deque < Point <T> > () const {
        std::deque< Point<T> > out;
        for(const auto p : ps)
            out.push_back(p);
        return out;
    }

    operator std::forward_list < Point <T> > () const {
        std::forward_list< Point<T> > out;
        for(auto i = ps.rbegin(); i != ps.rend(); ++i)
            out.push_front(*i);
        return out;
    }

    operator std::list < Point <T> > () const {
        std::list< Point<T> > out;
        for(const auto p : ps)
            out.push_back(p);
        return out;
    }

    operator std::queue < Point <T> > () const {
        std::queue< Point<T> > out;
        for(const auto p : ps)
            out.push(p);
        return out;
    }

    operator std::stack < Point <T> > () const {
        std::stack< Point<T> > out;
        for(const auto p : ps)
            out.push(p);
        return out;
    }

//------------------------------------------------------------------------------

    friend std::ostream &operator << (std::ostream &os, const Path &path) {
        os << path.to_string();
        return os;
    }

private:

    static bool compare_x(const Point<T> &lhs, const Point<T> &rhs) {
        return lhs.get_x() < rhs.get_x();
    }

    static bool compare_y(const Point<T> &lhs, const Point<T> &rhs) {
        return lhs.get_y() < rhs.get_y();
    }

};

} //lib_2d

#endif // POINT_H_INCLUDED
