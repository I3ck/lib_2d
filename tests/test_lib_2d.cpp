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
 * \file    test_lib_2d.cpp
 * \author  Martin Buck
 * \date    November 2014
 * \version 1.0
 * \brief   contains several tests for the libary
 * \todo    since this test is using catch, split the libary away from it, and make the test a single repo (or give the test itself another license)
 * \todo    move this file to folder tests and split it onto several files?
 */

#include <iostream>
#include <stdexcept>

#include "../inc/Point.h"
#include "../inc/Path.h"
#include "../inc/Arc.h"
#include "../inc/InvolutCircle.h"
#include "../inc/InterpolationBezier.h"

#define CATCH_CONFIG_MAIN
#include "../inc/Catch.h"

#define OUTPUT_TEST_FILES

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

TEST_CASE("testing Path") {
    Path<double> path = Path<double>();
    path.push_back(0.0, 0.0);
    path.push_back(1.0, 0.0);
    path.push_back(-1.0, 0.0);

    SECTION("testing center of path") {
        auto pathCenter = path.center();

        REQUIRE(pathCenter.abs() == 0.0);
    }

    SECTION("testing moving of path") {
        path.move_by(1.0, 0.0);
        auto pathCenter = path.center();

        REQUIRE( pathCenter.abs() == 1.0);
    }

    SECTION("testing rotation of path") {
        path.move_by(1.0, 0.0);
        path.rotate(PI);
        auto pathCenter = path.center();

        REQUIRE(pathCenter.abs() == 1.0);
        REQUIRE(pathCenter.get_x() == -1.0);
    }

    SECTION("testing parsing from and to string") {
        auto path2 = path;
        std::string path2String = path2.to_string();

        path.move_by(100, 1000);
        path.from_string(path2String);

        REQUIRE(path.similar_to(path2, MAX_DELTA));
    }

    SECTION("testing reversing of path") {
        path.push_back(13.0 ,137.3);
        auto path2 = path;

        path.reverse();
        REQUIRE(!path.similar_to(path2,MAX_DELTA));

        path.reverse();
        REQUIRE(path.similar_to(path2,MAX_DELTA));
    }

    SECTION("testing mirroring") {
        path.push_back(13.0 ,137.3);
        auto path2 = path;

        path.mirror_horizontally();
        REQUIRE(!path.similar_to(path2,MAX_DELTA));

        path.mirror_horizontally();
        REQUIRE(path.similar_to(path2,MAX_DELTA));
    }

    SECTION("testing intersections between paths") {
        Path<double> tmp = Path<double> ();
        tmp.push_back(-100,0);
        tmp.push_back(100,0);

        Path<double> tmp2 = Path<double> ();
        tmp2.push_back(0,-100);
        tmp2.push_back(0,100);

        auto intersections = tmp.intersections_with(tmp2);
        auto shouldIntersection = Point<double>(0,0);
        REQUIRE(intersections[0].similar_to(shouldIntersection,MAX_DELTA));

        tmp.move_by(0,-1);
        intersections = tmp.intersections_with(tmp2);
        shouldIntersection = Point<double>(0,-1);
        REQUIRE(intersections[0].similar_to(shouldIntersection,MAX_DELTA));

        tmp = Path<double> ();
        tmp.push_back(-100,100);
        tmp.push_back(100,-100);

        tmp2 = Path<double> ();
        tmp2.push_back(100,100);
        tmp2.push_back(-100,-100);

        intersections = tmp.intersections_with(tmp2);
        shouldIntersection = Point<double>(0,0);
        REQUIRE(intersections[0].similar_to(shouldIntersection,MAX_DELTA));
    }

    SECTION("testing sorting") {
        Path<double> tmp = Path<double> ();
        tmp.push_back(1,100);
        tmp.push_back(2,99);
        tmp.push_back(3,98);
        tmp.push_back(4,97);
        tmp.push_back(5,96);
        tmp.push_back(6,95);
        tmp.push_back(7,94);
        tmp.push_back(8,93);
        tmp.push_back(9,92);
        tmp.push_back(10,91);

        tmp.sort_y();
        for(unsigned int y=91; y <= 100; ++y)
            REQUIRE(tmp[y-91].get_y() == y);

        tmp.sort_x();
        for(unsigned int x=1; x <= 10; ++x)
            REQUIRE(tmp[x-1].get_x() == x);
    }

    SECTION("testing closest and furthest") {
        Path<double> tmp = Path<double> ();
        tmp.push_back(1,100);
        tmp.push_back(9,92);
        tmp.push_back(10,91);

        REQUIRE(tmp[tmp.furthest_apart(Point<double>())] == Point<double>(1,100));

        REQUIRE(tmp[tmp.closest(Point<double>())] == Point<double>(10,91));

        Path<double> tmp2 = Path<double> ();
        tmp2.push_back(Point<double>());

        REQUIRE(tmp[tmp.furthest_apart(tmp2)] == Point<double>(1,100));

        REQUIRE(tmp[tmp.closest(tmp2)] == Point<double>(10,91));
    }

    SECTION("testing average distance") {
        Path<double> tmp = Path<double> ();
        tmp.push_back(Point<double>());
        tmp.push_back(1,0);
        tmp.push_back(2,0);

        REQUIRE(tmp.average_distance() == 1);
    }

    SECTION("testing finding of points") {
        Path<double> tmp = Path<double> ();
        tmp.push_back(Point<double>());
        tmp.push_back(1,0);
        tmp.push_back(2,0);

        REQUIRE(tmp.index_of(Point<double>(2,0)) == 2);

        REQUIRE(tmp.index_of(Point<double>(222,0)) == -1);
    }

    SECTION("testing removal of points") {
        Path<double> tmp = Path<double> ();
        tmp.push_back(Point<double>());
        tmp.push_back(1,0);
        tmp.push_back(2,0);

        tmp.remove_from(5);
        tmp.remove_until(0);
        tmp.remove_left_of(0);
        tmp.remove_right_of(2);
        tmp.remove_above_of(0);
        tmp.remove_below_of(0);
        REQUIRE(tmp.size() == 3);

        tmp.push_back(0,17);
        tmp.remove_above_of(16);
        REQUIRE(tmp.size() == 3);

        tmp.push_back(0,-17);
        tmp.remove_below_of(-16);
        REQUIRE(tmp.size() == 3);

        tmp.push_back(17,17);
        tmp.remove_right_of(16);
        REQUIRE(tmp.size() == 3);

        tmp.push_back(-17,17);
        tmp.remove_left_of(-16);
        REQUIRE(tmp.size() == 3);

        tmp.remove_from(2);
        REQUIRE(tmp.size() == 2);

        tmp.remove_until(1);
        REQUIRE(tmp.size() == 1);
    }
}

TEST_CASE("testing Arc") {
    lib_2d::Arc<double> arc = lib_2d::Arc<double>(300.0, 100);
    REQUIRE(arc.size() == 100);

#ifdef OUTPUT_TEST_FILES
    arc.to_file("arc_full.test");
#endif // OUTPUT_TEST_FILES

    arc = lib_2d::Arc<double>(300.0, 100, false, PI);

    REQUIRE(arc.size() == 100);
#ifdef OUTPUT_TEST_FILES
    arc.to_file("arc_semi.test");
#endif // OUTPUT_TEST_FILES
}

TEST_CASE("testing InvolutCircle") {
    InvolutCircle<double> inv = InvolutCircle<double>(1.0, 100);

    REQUIRE(inv.size() == 100);
#ifdef OUTPUT_TEST_FILES
    inv.to_file("inv.test");
#endif // OUTPUT_TEST_FILES
}

TEST_CASE("testing bezier interpolation") {
    Path<double> tmp = Path<double> ();
    tmp.push_back(Point<double>());
    tmp.push_back(1,1);
    tmp.push_back(2,-5);
    InterpolationBezier<double> bezier = InterpolationBezier<double>(100, tmp);

    REQUIRE(bezier.size() == 100);
#ifdef OUTPUT_TEST_FILES
    bezier.to_file("bezier.test");
#endif // OUTPUT_TEST_FILES
}
