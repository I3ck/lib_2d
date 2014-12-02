#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include <sstream>
#include <cmath>

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

    bool from_string(const std::string &input, const std::string &divider = " ") {
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

    double abs() const {
        return sqrt(  pow(x,2) + pow(y,2) );
    }

    /*
    missing methods:
    rotate
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
