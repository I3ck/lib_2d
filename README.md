#lib_2d
A c++ library for anything related to points within the 2d space (using floating point data types)  
using Catch as testing framework https://github.com/philsquared/Catch  

##version 0.9

##templated  
tested for:  

```cpp
float  
double  
long double
```


##classes
currently, the following classes are supported:  

```cpp
Point<T> //a point defined with two coordinates (x and y)  
PointCloud<T> //a collection Points, without topological information
OrderedPointCloud<T> //a PointCloud with additional information regarding sorting and filtering of points
KdTree<T> //search tree to quickly find nearest neighbors

//subclasses of PointCloud
LineSegment<T> //a line segment defined by start and end point
Rectangle<T> //a rectangle defined by width, height and center
Arc<T> //an arc defined by diameter, center, start- and end angle
Ellipse<T> //an ellipse
InterpolationBezier<T> //a bezier curve interpolation  
InterpolationLinear<T> //a linear interpolation
InvolutCircle<T> //involut curve of a circle
```


##methods  
some of the supported methods:  

```cpp
move_by(...) //moves the Point or PointCloud  
mirror_vertically(...) //mirrors the Point or PointCloud at a vertical line (horizontally and point also supported)  
rotate(...) //rotate by angle around a center  
to_string(...) //write coordinates to a string  
from_string(...) //parse from a string
distance_to(...) //calculate distances between points
load(...) //load coordinates from file
to_file(...) //write coordinates to file  
bounding_box(...)  //the minimum bounding rectangle of a PointCloud  
convex_hull(...) //calculate the convex hull of a PointCloud  
concave_hull(...) //compareable to the convex hull, while better following the shape of a pointcloud
intersections_with(...) //intersections between paths  
sort_x(...) //sort by x (or y)  
range(from,to) //get ranges of PointCloud
and_many(more)  
```  

also chainable:
```cpp
path.move_by(...).rotate(...).sort_x(...)
```


##compatible with other containers  

```cpp
vector< Point <T> > vec = vector<Point <T> >(path.begin(), path.end());  
...  
PointCloud<T> path = PointCloud<T>(vec.begin(), vec.end());
```  


##operator overloads  

```cpp
== != //equality  
+ += //appending to PointCloud  
[] //access elements of PointCloud
std::cout << PointCloud<T> << Point<T>; //overloads for streams
```  



##using lib_2d
add `inc/` to your include path and `#include lib_2d.h` within your code  
`/tests/test_lib_2d.cpp` also provides a basic usage example



##testing
`make tests`  
you can also use  
`make clean`  



##contribute  
If you find any bugs, feel free to open an issue  
If you'd like other PointClouds than Arc etc. open an issue  
Also feel free to open a pull request, in case you added / fixed something yourself
