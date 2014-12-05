#ifndef POINTS_H_INCLUDED
#define POINTS_H_INCLUDED

#include <vector>

#include "Point.h"

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


};

#endif // POINT_H_INCLUDED
