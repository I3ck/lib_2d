#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

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

    ~Point() {
    }

    T get_x() const {
        return x;
    }

    T get_y() const {
        return y;
    }


    void move_by(const T &x, const T &y) {
        this->x+=x;
        this->y+=y;
    }

    void move_by(const Point &other) {
        this->x+=other.get_x();
        this->y+=other.get_y();
    }

    /*
    missing methods:
    rotate
    to_string
    construction from string
    setter for x and y
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
