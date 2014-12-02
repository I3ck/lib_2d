#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <sstream>

template <typename T>
class Point {

private:
    T x, y;

public:
    Point() :
        Point(0, 0)
    {}

    Point(const T &x, const T &y) :
        x(x),
        y(y)
    {}

//------------------------------------------------------------------------------

    ~Point() {
    }

//------------------------------------------------------------------------------

    T get_x() const {
        return x;
    }

    T get_y() const {
        return y;
    }

//------------------------------------------------------------------------------

    void set_x(const T &x) {
        this->x=x;
    }

    void set_y(const T &y) {
        this->y=y;
    }

//------------------------------------------------------------------------------

    void move_by(const T &x, const T &y) {
        this->x+=x;
        this->y+=y;
    }

    void move_by(const Point &other) {
        this->x+=other.get_x();
        this->y+=other.get_y();
    }

//------------------------------------------------------------------------------

    std::string to_string(const std::string &divider = " ") const {
        std::stringstream ss;
        ss << x << divider << y;
        return ss.str();
    }

    /*
    missing methods:
    rotate
    construction from string
    mirror anywhere
    polar coords
    steepness
    angle towards
    abs
    distance to
    set distance to
    move to (instead of set?)

    ops
    == != << to pair / from pair

    functions:
    center between




    */

};


#endif // POINT_H_INCLUDED
