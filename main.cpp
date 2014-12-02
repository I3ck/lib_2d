#include <iostream>
#include <stdexcept>

#include "inc/Point.h"

using namespace std;

int main()
{
    try {
        const double
            X(0.3),
            Y(4.5),
            MOVE_X(0.5),
            MOVE_Y(-10.0);

        Point <double> p(X, Y);

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



        return 0;
    }
    catch (runtime_error &e) {
        cout << e.what() << endl;
        return 1;
    }
}


