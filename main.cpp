#include <iostream>
#include <stdexcept>

#define PI 3.14159265358979323846

#include "inc/Point.h"

using namespace std;

int main()
{
    try {
        const double
            X(0.3),
            Y(4.5),
            MOVE_X(0.5),
            MOVE_Y(-10.0),
            MAX_DELTA(0.00001);

        Point <double> p(X, Y);
        auto p2 = p;

        if(p.get_x() != X)
            throw runtime_error("x read incorrectly");

        if(p.get_y() != Y)
            throw runtime_error("y read incorrectly");

        p.move_by(MOVE_X, MOVE_Y);

        if(p.get_x() != X+MOVE_X)
            throw runtime_error("moving by x failed");

        if(p.get_y() != Y+MOVE_Y)
            throw runtime_error("moving by y failed");



        std::string tmp = p.to_string();
        p.set_x(17);
        p.set_y(32);
        p.from_string(tmp);

        if(p.to_string() != tmp)
            throw runtime_error("parsing from and to string failed");



        if (p.abs() != sqrt(  pow(p.get_x(),2) + pow(p.get_y(),2)  ))
            throw runtime_error("absolute value failed");

        if (p.distance_to(0.0, 0.0) != p.abs())
            throw runtime_error("distance to 0/0 unequal to abs");

        if (p.distance_to(p) != 0.0)
            throw runtime_error("distance to point itself not 0");



        p2 = p;
        p.rotate(PI);

        if (p.get_x() ==  p2.get_x() || p.get_y() ==  p2.get_y())
            throw runtime_error("rotation does nothing");

        p.rotate(PI);

        if ( abs(p.get_x() - p2.get_x()) > MAX_DELTA || abs(p.get_y() - p2.get_y()) > MAX_DELTA)
            throw runtime_error("rotation by 360 degree yields other results");



        p2 = p;
        p.mirror_vertically();

        if ( p.get_y() != p2.get_y() || p.get_x() != -p2.get_x())
            throw runtime_error("vertically mirroring doesn't work");



        p2 = p;
        p.mirror_horizontally();

        if ( p.get_x() != p2.get_x() || p.get_y() != -p2.get_y())
            throw runtime_error("vertically mirroring doesn't work");





        cout << "everything working fine" << endl;



        return 0;
    }
    catch (runtime_error &e) {
        cout << e.what() << endl;
        return 1;
    }
}


