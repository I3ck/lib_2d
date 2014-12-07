#ifndef POINTS_H_INCLUDED
#define POINTS_H_INCLUDED

#include <vector>

#include "Point.h"

namespace lib_2d {

template <typename T>
class Points {

private:
    std::vector < Point <T> > ps;

public:
    Points(){};
    ~Points(){};

    void move_by(const T &x, const T &y) {
        for(auto &p : ps)
            p.move_by(x, y);
    }

    void move_by(const Point<T> &other) {
        for(auto &p : ps)
            p.move_by(other);
    }

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
                output += p.to_string() + "\n";

        return output;
    }

//------------------------------------------------------------------------------

    void push_back(const Point<T> &point) {
        ps.push_back(point);
    }

    void push_back(const T &x, const T &y) {
        push_back(Point<T>(x, y));
    }

    size_t size() const {
        return ps.size();
    }

    bool empty() const {
        return ps.empty();
    }

    void reserve(const size_t &i) {
        ps.reserve(i);
    }

    void clear() {
        ps.clear();
    }

    void flip() {
        ps.flip();
    }

    Point<T> operator [] (const unsigned int &i) const
    {
        return ps[i];
    }

    Point<T>& operator [] (const unsigned int &i)
    {
        return ps[i];
    }

};

} //lib_2d

#endif // POINT_H_INCLUDED
