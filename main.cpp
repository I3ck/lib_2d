#include <iostream>

#include "inc/Point.h"

using namespace std;

int main()
{
    Point <double> p(0.3, 2.7);
    cout << p.get_x() << endl;
    p.move_by(0.7, 7.9);
    cout << p.get_x() << endl;
    cout << p.to_string() << endl;
    return 0;
}


