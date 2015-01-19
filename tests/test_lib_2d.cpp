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
 * \brief   contains several tests for the library
 */

#include <iostream>
#include <stdexcept>

#include "../lib_2d.h"

#define CATCH_CONFIG_MAIN
#include "../dependencies/Catch.h" //https://github.com/philsquared/Catch

#define OUTPUT_TEST_FILES


using namespace std;
using namespace lib_2d;

#ifdef USE_DOUBLE
using T = double;
#elif USE_LDOUBLE
using T = long double;
#elif USE_FLOAT
using T = float;
#else
using T = int;
#endif

const T
    X(0.3),
    Y(4.5),
    MOVE_X(0.5),
    MOVE_Y(-10.0),
    MAX_DELTA(0.00001);


TEST_CASE("testing Point") {

    Point <T> p(X, Y);
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
        REQUIRE(abs(  p.abs() - (T)sqrt(  pow(p.get_x(),2) + pow(p.get_y(),2)  )  ) < MAX_DELTA);
        REQUIRE(p.distance_to(0.0, 0.0) == p.abs());
        REQUIRE(p.distance_to(p) == 0.0);
    }

    SECTION("testing rotation") {
        p2 = p;

        p.rotate(LIB_2D_PI);
        REQUIRE(p.get_x() !=  p2.get_x());
        REQUIRE(p.get_y() !=  p2.get_y());

        p.rotate(LIB_2D_PI);
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

        p.move_by(0.001, 0.0);

        REQUIRE(p2 != p);
    }

    SECTION("testing conversion with pair") {
        p2 = p;
        pair <T, T> tmp = p;
        p = Point <T> (tmp);

        REQUIRE(p2 == p);

        tmp = pair <T,T> (0.3, 17.7);
        p = Point <T> (tmp);

        REQUIRE(p != p2);
    }

    SECTION("testing stringstream overload") {
        std::stringstream tmp;
        p2 = p;
        tmp << p;
        p.move_by(0.3, 7.9);
        p.from_string(tmp.str());

        REQUIRE(p.similar_to(p2,MAX_DELTA));
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
        REQUIRE(abs( p.phi() - LIB_2D_PI/2.0) <= MAX_DELTA);

        p.set_x(-1);
        p.set_y(0);
        REQUIRE(abs( p.phi() - LIB_2D_PI) <= MAX_DELTA);
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

        REQUIRE(abs( p.rad_to(p2) - LIB_2D_PI/2.0) <= MAX_DELTA);
    }

    SECTION("testing center calculation") {
        Point <T> center, centerShould;

        p = Point<T> (1,1);
        p2 = Point<T> (1,2);
        centerShould = Point <T> (1, 1.5);
        center = p.center_between(p2);

        REQUIRE(center.similar_to(centerShould, MAX_DELTA));
    }
}

TEST_CASE("testing Path") {
    Path<T> path = Path<T>();
    path.push_back(0.0, 0.0);
    path.push_back(1.0, 0.0);
    path.push_back(-1.0, 0.0);

    SECTION("testing usage a vector") {
        std::vector< Point <T> > vec = path;
        REQUIRE(vec.size() == 3);

        Path<T> tmp = Path<T>(vec);
        REQUIRE(tmp.similar_to(path, MAX_DELTA));
    }

    SECTION("testing usage a deque") {
        std::deque< Point <T> > deq = path;
        REQUIRE(deq.size() == 3);

        Path<T> tmp = Path<T>(deq);
        REQUIRE(tmp.similar_to(path, MAX_DELTA));
    }

    SECTION("testing usage of forward list") {
        std::forward_list< Point <T> > fl = path;

        Path<T> tmp = Path<T>(fl);
        REQUIRE(tmp.similar_to(path, MAX_DELTA));
    }

    SECTION("testing usage of list") {
        std::list< Point <T> > l = path;
        REQUIRE(l.size() == 3);

        Path<T> tmp = Path<T>(l);
        REQUIRE(tmp.similar_to(path, MAX_DELTA));
    }

    SECTION("testing usage of queue") {
        std::queue< Point <T> > q = path;
        REQUIRE(q.size() == 3);

        Path<T> tmp = Path<T>(q);
        REQUIRE(tmp.similar_to(path, MAX_DELTA));
    }

    SECTION("testing usage of stack") {
        std::stack< Point <T> > s = path;
        REQUIRE(s.size() == 3);

        Path<T> tmp = Path<T>(s);
        REQUIRE(tmp.similar_to(path, MAX_DELTA));
    }

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
        path.rotate(LIB_2D_PI);
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
        Path<T> tmp = Path<T> ();
        tmp.push_back(-100,0);
        tmp.push_back(100,0);

        Path<T> tmp2 = Path<T> ();
        REQUIRE(!tmp.intersects_with(tmp2));
        tmp2.push_back(0,-100);
        tmp2.push_back(0,100);

        auto intersections = tmp.intersections_with(tmp2);
        auto shouldIntersection = Point<T>(0,0);
        REQUIRE(intersections[0].similar_to(shouldIntersection,MAX_DELTA));
        REQUIRE(tmp.intersects_with(tmp2));

        tmp.move_by(0,-1);
        intersections = tmp.intersections_with(tmp2);
        shouldIntersection = Point<T>(0,-1);
        REQUIRE(intersections[0].similar_to(shouldIntersection,MAX_DELTA));
        REQUIRE(tmp.intersects_with(tmp2));

        tmp = Path<T> ();
        tmp.push_back(-100,100);
        tmp.push_back(100,-100);

        tmp2 = Path<T> ();
        tmp2.push_back(100,100);
        tmp2.push_back(-100,-100);

        intersections = tmp.intersections_with(tmp2);
        shouldIntersection = Point<T>(0,0);
        REQUIRE(intersections[0].similar_to(shouldIntersection,MAX_DELTA));
        REQUIRE(tmp.intersects_with(tmp2));
    }

    SECTION("testing sorting") {
        Path<T> tmp = Path<T> ();
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
        Path<T> tmp = Path<T> ();
        tmp.push_back(1,100);
        tmp.push_back(9,92);
        tmp.push_back(10,91);

        REQUIRE(tmp[tmp.furthest_apart(Point<T>())] == Point<T>(1,100));

        REQUIRE(tmp[tmp.closest(Point<T>())] == Point<T>(10,91));

        Path<T> tmp2 = Path<T> ();
        tmp2.push_back(Point<T>());

        REQUIRE(tmp[tmp.furthest_apart(tmp2)] == Point<T>(1,100));

        REQUIRE(tmp[tmp.closest(tmp2)] == Point<T>(10,91));
    }

    SECTION("testing average distance") {
        Path<T> tmp = Path<T> ();
        tmp.push_back(Point<T>());
        tmp.push_back(1,0);
        tmp.push_back(2,0);

        REQUIRE(tmp.average_distance() == 1);
    }

    SECTION("testing finding of points") {
        Path<T> tmp = Path<T> ();
        tmp.push_back(Point<T>());
        tmp.push_back(1,0);
        tmp.push_back(2,0);

        REQUIRE(tmp.index_of(Point<T>(2,0)) == 2);

        REQUIRE(tmp.index_of(Point<T>(222,0)) == -1);
    }

    SECTION("testing removal of points") {
        Path<T> tmp = Path<T> ();
        tmp.push_back(Point<T>());
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

        tmp.remove_closer_to_than(1.0);
        tmp.remove_further_apart_to_than(1.0);
        REQUIRE(tmp.size() == 1);

        auto tmp2 = tmp;

        tmp.remove_closer_to_than(1.00001);
        REQUIRE(tmp.size() == 0);

        tmp = tmp2;
        tmp.remove_further_apart_to_than(0.99999);
        REQUIRE(tmp.size() == 0);
    }

    SECTION("testing convex hull") {
        Path<T> tmp = Path<T> ();
        tmp.push_back(0,0);
        tmp.push_back(3,-1);
        tmp.push_back(3,3);
        tmp.push_back(-5,4);
        tmp.push_back(-6,0);

        tmp.push_back(1,1);
        tmp.push_back(2,1);

        auto convexHull = tmp.convex_hull();

        std::cout << convexHull << std::endl;

#ifdef OUTPUT_TEST_FILES
        tmp.to_file("convex_before.test");
        convexHull.to_file("convex_after.test");
#endif // OUTPUT_TEST_FILES


        REQUIRE(convexHull.size() == 5);



    }
}

TEST_CASE("testing Arc") {

    lib_2d::Arc<T> arc = lib_2d::Arc<T>(300.0, 100);
    SECTION("creation") {
        REQUIRE(arc.size() == 100);

#ifdef OUTPUT_TEST_FILES
        arc.to_file("arc_full.test");
#endif // OUTPUT_TEST_FILES

        arc = lib_2d::Arc<T>(300.0, 100, false, LIB_2D_PI);

        REQUIRE(arc.size() == 100);
#ifdef OUTPUT_TEST_FILES
        arc.to_file("arc_semi.test");
#endif // OUTPUT_TEST_FILES
    }

    SECTION("moving") {
        arc.move_by(10,0);
        REQUIRE(arc.center() == Point<T>(10,0));
    }

    SECTION("rotation") {
        arc.move_by(10,0);
        arc.rotate(LIB_2D_PI);
        auto center = arc.center();
        REQUIRE(center.get_x() == -10);
    }

    SECTION("mirroring") {
        arc.move_by(10,5);

        arc.mirror_vertically();
        auto center = arc.center();
        REQUIRE(center.get_x() == -10);
        REQUIRE(center.get_y() == 5);

        arc.mirror_horizontally();
        center = arc.center();
        REQUIRE(center.get_x() == -10);
        REQUIRE(center.get_y() == -5);
    }

}

TEST_CASE("testing InvolutCircle") {
    InvolutCircle<T> inv = InvolutCircle<T>(1.0, 100);

    SECTION("creation") {
        REQUIRE(inv.size() == 100);
#ifdef OUTPUT_TEST_FILES
        inv.to_file("inv.test");
#endif // OUTPUT_TEST_FILES
    }

    SECTION("moving") {
        inv.move_by(10,0);
        REQUIRE(inv.center() == Point<T>(10,0));
    }

    SECTION("rotation") {
        inv.move_by(10,0);
        inv.rotate(LIB_2D_PI);
        auto center = inv.center();
        REQUIRE(center.get_x() == -10);
    }

    SECTION("mirroring") {
        inv.move_by(10,5);

        inv.mirror_vertically();
        auto center = inv.center();
        REQUIRE(center.get_x() == -10);
        REQUIRE(center.get_y() == 5);

        inv.mirror_horizontally();
        center = inv.center();
        REQUIRE(center.get_x() == -10);
        REQUIRE(center.get_y() == -5);
    }

}

TEST_CASE("testing bezier interpolation") {
    Path<T> tmp = Path<T> ();
    tmp.push_back(Point<T>());
    tmp.push_back(1,1);
    tmp.push_back(2,-5);
    InterpolationBezier<T> bezier = InterpolationBezier<T>(100, tmp);

    REQUIRE(bezier.size() == 100);
#ifdef OUTPUT_TEST_FILES
    bezier.to_file("bezier.test");
#endif // OUTPUT_TEST_FILES
}

TEST_CASE("testing linear interpolation") {
    Path<T> tmp = Path<T> ();
    tmp.push_back(Point<T>());
    tmp.push_back(1,1);
    tmp.push_back(2,-5);
    InterpolationLinear<T> linear = InterpolationLinear<T>(100, tmp);

    REQUIRE(linear.size() == 100);
#ifdef OUTPUT_TEST_FILES
    linear.to_file("linear.test");
#endif // OUTPUT_TEST_FILES
}

TEST_CASE("testing cosine interpolation") {
    Path<T> tmp = Path<T> ();
    tmp.push_back(Point<T>());
    tmp.push_back(1,1);
    tmp.push_back(2,-5);
    InterpolationCosine<T> cosine = InterpolationCosine<T>(100, tmp);

    REQUIRE(cosine.size() == 100);
#ifdef OUTPUT_TEST_FILES
    cosine.to_file("cosine.test");
#endif // OUTPUT_TEST_FILES
}
