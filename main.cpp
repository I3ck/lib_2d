#include <iostream>
#include <stdexcept>

#include "inc/Point.h"

using namespace std;

int main()
{
    try {
        const double
            X(0.3),
            Y(4.5);

        Point <double> p(X, Y);

        if(p.get_x() != X)
            throw runtime_error("x read incorrectly");

        if(p.get_y() != Y)
            throw runtime_error("y read incorrectly");


        cout << p.get_x() << endl;
        p.move_by(0.7, 7.9);
        cout << p.get_x() << endl;
        cout << p.to_string() << endl;
        return 0;
    }
    catch (runtime_error &e) {
        cout << e.what() << endl;
    }
}


