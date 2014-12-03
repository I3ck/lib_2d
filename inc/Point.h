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
        this->x+=other.x;
        this->y+=other.y;
    }

//------------------------------------------------------------------------------

    void mirror_vertically(const T &xValue = 0) {
        x = 2 * xValue - x;
    }

    void mirror_vertically(const Point &other) {
        mirror_vertically(other.x);
    }

//------------------------------------------------------------------------------

    void rotate(const T &radians, const Point &center = Point(0, 0)) {
        T newX, newY;

        newX = center.x + cos(radians) * (x - center.x) - sin(radians) * (y - center.y);
        newY = center.y + sin(radians) * (x - center.y) + cos(radians) * (y - center.y);

        x = newX;
        y = newY;
    }

    void rotate(const T &radians, const T &centerX, const T &centerY) {
        rotate(radians, Point(centerX, centerY));
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

//------------------------------------------------------------------------------

    double distance_to(const Point &other) const {
        return sqrt(  pow(x-other.x,2) + pow(y-other.y,2)  );
    }

    double distance_to(const T &x, const T &y) const {
        return distance_to(Point(x,y));
    }

//------------------------------------------------------------------------------


    /*
    missing methods:
    mirror anywhere
    polar coords
    steepness
    angle towards
    set distance to
    move to (instead of set?)

    ops
    == != << to pair / from pair

    functions:
    center between




    */

};


#endif // POINT_H_INCLUDED
