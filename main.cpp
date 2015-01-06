/*
    Copyright (c) 2014 - 2015 Martin Buck
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

/**
 * \file    main.cpp
 * \author  Martin Buck
 * \date    November 2014
 * \version 1.0
 * \brief   contains several tests for the libary
 * \todo    since this test is using catch, split the libary away from it, and make the test a single repo (or give the test itself another license)
 * \todo    move this file to folder tests and split it onto several files?
 */

#include <iostream>
#include <stdexcept>

#include "inc/Point.h"
#include "inc/Path.h"
#include "inc/Arc.h"
#include "inc/InvolutCircle.h"

#define CATCH_CONFIG_MAIN
#include "inc/Catch.h"

using namespace std;
using namespace lib_2d;

const double
    X(0.3),
    Y(4.5),
    MOVE_X(0.5),
    MOVE_Y(-10.0),
    MAX_DELTA(0.00001),
    PI (3.14159265358979323846);


TEST_CASE("testing Point") {

    Point <double> p(X, Y);
    auto p2 = p;

    SECTION ("testing initialisation") {
        REQUIRE(p.get_x() == X);
        REQUIRE(p.get_y() == Y);
    }

    SECTION("testing moving") {
        p.move_by(MOVE_X, MOVE_Y);

        REQUIRE(p.get_x() == X+MOVE_X);
        REQUIRE(p.get_y() == Y+MOVE_Y);
    }

    SECTION("testing parsing from and to strings") {
        std::string tmp = p.to_string();
        p.set_x(17);
        p.set_y(32);
        p.from_string(tmp);

        REQUIRE(p.to_string() == tmp);
    }

    SECTION("testing distances and abs") {
        REQUIRE(p.abs() == sqrt(  pow(p.get_x(),2) + pow(p.get_y(),2)  ));
        REQUIRE(p.distance_to(0.0, 0.0) == p.abs());
        REQUIRE(p.distance_to(p) == 0.0);
    }

    SECTION("testing rotation") {
        p2 = p;

        p.rotate(PI);
        REQUIRE(p.get_x() !=  p2.get_x());
        REQUIRE(p.get_y() !=  p2.get_y());

        p.rotate(PI);
        REQUIRE(abs(p.get_x() - p2.get_x()) < MAX_DELTA);
        REQUIRE(abs(p.get_y() - p2.get_y()) < MAX_DELTA);
    }

    SECTION("testing mirroring") {
        p2 = p;
        p.mirror_vertically();
        REQUIRE(p.get_y() == p2.get_y());
        REQUIRE(p.get_x() == -p2.get_x());

        p2 = p;
        p.mirror_horizontally();
        REQUIRE(p.get_x() == p2.get_x());
        REQUIRE(p.get_y() == -p2.get_y());

        p2 = p;
        p.mirror_point();
        REQUIRE(p.get_x() == -p2.get_x());
        REQUIRE(p.get_y() == -p2.get_y());
    }

    SECTION("testing equality tests") {
        p2 = p;
        REQUIRE(p2 == p);

        p.move_by(0.00000000001, 0.0);

        REQUIRE(p2 != p);
    }

    SECTION("testing conversion with pair") {
        p2 = p;
        pair <double, double> tmp = p;
        p = Point <double> (tmp);

        REQUIRE(p2 == p);

        tmp = pair <double,double> (0.3, 17.7);
        p = Point <double> (tmp);

        REQUIRE(p != p2);
    }

    SECTION("testing stringstream overload") {
        std::stringstream tmp;
        p2 = p;
        tmp << p;
        p.move_by(0.3, 7.9);
        p.from_string(tmp.str());

        REQUIRE(p == p2);
    }

    SECTION("testing similarity") {
        p2 = p;
        REQUIRE(p.similar_to(p2, MAX_DELTA));

        p.move_by(0.999 * MAX_DELTA,0);
        REQUIRE(p.similar_to(p2, MAX_DELTA));

        p2 = p;
        p.move_by(1.0001 * MAX_DELTA,0);
        REQUIRE(!p.similar_to(p2, MAX_DELTA));
    }

    SECTION("testing phi calculation") {
        p.set_x(0);
        p.set_y(1);
        REQUIRE(abs( p.phi() - PI/2.0) <= MAX_DELTA);

        p.set_x(-1);
        p.set_y(0);
        REQUIRE(abs( p.phi() - PI) <= MAX_DELTA);
    }

    SECTION("testing slope calculation") {
        p.set_x(0);
        p.set_y(0);

        p2.set_x(1);
        p2.set_y(2);

        REQUIRE(abs( p.slope_to(p2) - 2.0) <= MAX_DELTA);
    }

    SECTION("testing radians calculation") {
        p.set_x(1);
        p.set_y(1);

        p2.set_x(1);
        p2.set_y(2);

        REQUIRE(abs( p.rad_to(p2) - PI/2.0) <= MAX_DELTA);
    }

    SECTION("testing center calculation") {
        Point <double> center, centerShould;

        p = Point<double> (1,1);
        p2 = Point<double> (1,2);
        centerShould = Point <double> (1, 1.5);
        center = p.center_between(p2);

        REQUIRE(center.similar_to(centerShould, MAX_DELTA));
    }
}

/*
int main()
{
    try {

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



        if(path2.index_of(Point<double>(2,0)) != 2)
            throw runtime_error("index of point couldn't be found");

        if(path2.index_of(Point<double>(222,0)) != -1)
            throw runtime_error("point which is not in path was found");


        path2.remove_from(5);
        path2.remove_until(0);
        path2.remove_left_of(0);
        path2.remove_right_of(2);
        path2.remove_above_of(0);
        path2.remove_below_of(0);


        if(path2.size() != 3)
            throw runtime_error("path had points removed although that shouldn't have happened");

        path2.push_back(0,17);
        path2.remove_above_of(16);

        if(path2.size() != 3)
            throw runtime_error("remove above doesn't work");

        path2.push_back(0,-17);
        path2.remove_below_of(-16);

        if(path2.size() != 3)
            throw runtime_error("remove below doesn't work");

        path2.push_back(17,17);
        path2.remove_right_of(16);

        if(path2.size() != 3)
            throw runtime_error("remove right of doesn't work");

        path2.push_back(-17,17);
        path2.remove_left_of(-16);

        if(path2.size() != 3)
            throw runtime_error("remove left of doesn't work");

        path2.remove_from(2);

        if(path2.size() != 2)
            throw runtime_error("remove from doesn't work");

        path2.remove_until(1);

        if(path2.size() != 1)
            throw runtime_error("remove until doesn't work");

        cout << "Path working fine" << endl;







        cout << "testing Arc" << endl;

        Arc<double> arc = Arc<double>(300.0, 100);

        arc.to_file("arc_full.test");

        if(arc.size() != 100)
            throw runtime_error("full arc size incorrect");

        arc = Arc<double>(300.0, 100, false, PI);

        arc.to_file("arc_semi.test");

        if(arc.size() != 100)
            throw runtime_error("semi arc size incorrect");




        cout << "Arc working fine" << endl;


        cout << "testing InvolutCircle" << endl;

        InvolutCircle<double> inv = InvolutCircle<double>(1.0, 100);

        inv.to_file("inv.test");

        if(inv.size() != 100)
            throw runtime_error("semi arc size incorrect");






        cout << "everything working fine" << endl;
        return 0;
    }
    catch (runtime_error &e) {
        cout << e.what() << endl;
    }
    return 1;
}
*/


