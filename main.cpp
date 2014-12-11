/*
    Copyright (c) 2014 Martin Buck
    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the rights to
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
    OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include <stdexcept>

#include "inc/Point.h"
#include "inc/Path.h"

using namespace std;
using namespace lib_2d;

int main()
{
    try {
        const double
            X(0.3),
            Y(4.5),
            MOVE_X(0.5),
            MOVE_Y(-10.0),
            MAX_DELTA(0.00001),
            PI (3.14159265358979323846);

        cout << "testing Point" << endl;

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
            throw runtime_error("horizontally mirroring doesn't work");



        p2 = p;
        p.mirror_point();

        if ( p.get_x() != -p2.get_x() || p.get_y() != -p2.get_y())
            throw runtime_error("point mirroring doesn't work");






        p2 = p;

        if ( p2 != p)
            throw runtime_error("equal points measured as unequal");

        p.move_by(0.00000000001, 0.0);

        if ( p2 == p)
            throw runtime_error("unequal points measured as equal");



        p2 = p;
        pair <double, double> tmpPair = p;
        p = Point <double> (tmpPair);

        if (p2 != p)
            throw runtime_error("conversion to and from pair yields new values");

        tmpPair = pair <double,double> (0.3, 17.7);
        p = Point <double> (tmpPair);

        if (p == p2)
            throw runtime_error("pair and point shouldn't be equal here");


        std::stringstream tmpSs;
        p2 = p;
        tmpSs << p;
        p.move_by(0.3, 7.9);
        p.from_string(tmpSs.str());

        if (p != p2)
            throw runtime_error("stringstream overload doesn't work");



        p2 = p;

        if (!p.similar_to(p2, MAX_DELTA))
            throw runtime_error("equal points should count as similar");

        p.move_by(0.999 * MAX_DELTA,0);

        if (!p.similar_to(p2, MAX_DELTA))
            throw runtime_error("edge case for similarity not working");

        p2 = p;
        p.move_by(1.0001 * MAX_DELTA,0);

        if (p.similar_to(p2, MAX_DELTA))
            throw runtime_error("edge case for similarity not working");



        p.set_x(0);
        p.set_y(1);

        if (abs( p.phi() - PI/2.0) > MAX_DELTA)
            throw runtime_error("phi not calculated correctly");

        p.set_x(-1);
        p.set_y(0);

        if (abs( p.phi() - PI) > MAX_DELTA)
            throw runtime_error("phi not calculated correctly");



        p.set_x(0);
        p.set_y(0);

        p2.set_x(1);
        p2.set_y(2);

        if (abs( p.slope_to(p2) - 2.0) > MAX_DELTA)
            throw runtime_error("slope calculated incorrectly");



        p.set_x(1);
        p.set_y(1);

        p2.set_x(1);
        p2.set_y(2);

        if (abs( p.rad_to(p2) - PI/2.0) > MAX_DELTA)
            throw runtime_error("radians between points calculated incorrectly");



        Point <double> center, centerShould;

        centerShould = Point <double> (1, 1.5);
        center = p.center_between(p2);

        if (!center.similar_to(centerShould, MAX_DELTA))
            throw runtime_error("center calculation seems to be wrong");


        cout << "Point working fine" << endl;





        cout << "testing Path" << endl;

        Path<double> path = Path<double>();
        path.push_back(0.0, 0.0);
        path.push_back(1.0, 0.0);
        path.push_back(-1.0, 0.0);

        auto pathCenter = path.center();

        if( pathCenter.abs() != 0.0)
            throw runtime_error("center calculation of path not working");

        path.move_by(1.0, 0.0);

        pathCenter = path.center();

        if( pathCenter.abs() != 1.0)
            throw runtime_error("moving path doesn't seem to work");

        path.rotate(PI);
        pathCenter = path.center();

        if ( pathCenter.abs() != 1.0 || pathCenter.get_x() != -1.0)
            throw runtime_error("rotation seems to not work");



        auto path2 = path;
        std::string path2String = path2.to_string();

        path.move_by(100, 1000);

        path.from_string(path2String);

        if(!path.similar_to(path2, MAX_DELTA))
            throw runtime_error("writing and reading from string doesn't work");

        path.push_back(13.0 ,137.3);
        path2 = path;

        path.reverse();

        if(path.similar_to(path2,MAX_DELTA))
            throw runtime_error("reversing doesn't do anything");

        path.reverse();

        if(!path.similar_to(path2,MAX_DELTA))
            throw runtime_error("reversing twice results in different result");

        path2 = path;

        path.mirror_horizontally();

        if(path.similar_to(path2,MAX_DELTA))
            throw runtime_error("mirroring doesn't do anything");

        path.mirror_horizontally();

        if(!path.similar_to(path2,MAX_DELTA))
            throw runtime_error("mirroring twice results in different result");




        path = Path<double> ();
        path.push_back(-100,0);
        path.push_back(100,0);

        path2 = Path<double> ();
        path2.push_back(0,-100);
        path2.push_back(0,100);

        auto intersections = path.intersections_with(path2);
        auto shouldIntersection = Point<double>(0,0);

        if(!intersections[0].similar_to(shouldIntersection,MAX_DELTA))
           throw runtime_error("intersections incorrect");

        path.move_by(0,-1);
        intersections = path.intersections_with(path2);
        shouldIntersection = Point<double>(0,-1);

        if(!intersections[0].similar_to(shouldIntersection,MAX_DELTA))
           throw runtime_error("intersections incorrect");


        path = Path<double> ();
        path.push_back(-100,100);
        path.push_back(100,-100);

        path2 = Path<double> ();
        path2.push_back(100,100);
        path2.push_back(-100,-100);

        intersections = path.intersections_with(path2);
        shouldIntersection = Point<double>(0,0);

        if(!intersections[0].similar_to(shouldIntersection,MAX_DELTA))
           throw runtime_error("intersections incorrect");



        path = Path<double> ();
        path.push_back(1,100);
        path.push_back(2,99);
        path.push_back(3,98);
        path.push_back(4,97);
        path.push_back(5,96);
        path.push_back(6,95);
        path.push_back(7,94);
        path.push_back(8,93);
        path.push_back(9,92);
        path.push_back(10,91);

        path.sort_y();

        for(unsigned int y=91; y <= 100; ++y) {
            if (path[y-91].get_y() != y)
                throw runtime_error("sorting by y doesn't work");
        }

        path.sort_x();

        for(unsigned int x=1; x <= 10; ++x) {
            if (path[x-1].get_x() != x)
                throw runtime_error("sorting by x doesn't work");
        }

        if(path[path.furthest_apart(Point<double>())] != Point<double>(1,100))
            throw runtime_error("furthest apart path to point not working properly");

        if(path[path.closest(Point<double>())] != Point<double>(10,91))
            throw runtime_error("closest path to point not working properly");



        path2.clear();

        path2.push_back(Point<double>());

        if(path[path.furthest_apart(path2)] != Point<double>(1,100))
            throw runtime_error("furthest apart path to path not working properly");

        if(path[path.closest(path2)] != Point<double>(10,91))
            throw runtime_error("closest path to path not working properly");



        path2.push_back(1,0);
        path2.push_back(2,0);

        if(path2.average_distance() != 1)
            throw runtime_error("average distance not working properly");



        cout << "Path working fine" << endl;

        cout << "everything working fine" << endl;
        return 0;
    }
    catch (runtime_error &e) {
        cout << e.what() << endl;
    }
    return 1;
}


