#lib_2d
A c++ library for paths defined by points within the 2d space (using floating point data types)  
using Catch as testing framework https://github.com/philsquared/Catch  

##version 0.8

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
Path<T> //a collection Points, which have a specified enumeration
KdTree<T> //search tree to quickly find nearest neighbors

//subclasses of Path
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
move_by(...) //moves the Point or Path  
mirror_vertically(...) //mirrors the Point or Path at a vertical line (horizontally and point also supported)  
rotate(...) //rotate by angle around a center  
to_string(...) //write coordinates to a string  
from_string(...) //parse from a string
distance_to(...) //calculate distances between points
load(...) //load coordinates from file
to_file(...) //write coordinates to file  
bounding_box(...)  //the minimum bounding rectangle of a Path  
convex_hull(...) //calculate the convex hull of a Path  
intersections_with(...) //intersections between paths  
sort_x(...) //sort by x (or y)  
range(from,to) //get ranges of Path
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
Path<T> path = Path<T>(vec.begin(), vec.end());
```  


##operator overloads  

```cpp
== != //equality  
+ += //appending to Path  
[] //access elements of Path
std::cout << Path<T> << Point<T>; //overloads for streams
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
If you'd like other Paths than Arc etc. open an issue  
Also feel free to open a pull request, in case you added / fixed something yourself
