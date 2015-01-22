#lib_2d
A c++ library for paths defined by points within the 2d space (using floating point data types)  
using Catch as testing framework https://github.com/philsquared/Catch  



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

//subclasses of Path
Arc<T> //an arc defined by diameter, center, start- and end angle
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
convex_hul(...) //calculate the convex hull of a Path  
intersections_with(...) //intersections between paths  
sort_x(...) //sort by x (or y)  
```  


##easily usable as other containers (or constructable from)

```cpp
vector< Point <T> >  
deque< Point <T> >  
forward_list< Point <T> >  
list< Point <T> >  
queue< Point <T> >  
stack< Point <T> >  
```  


##operator overloards  

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
to make and run the tests  
`cd tests/`  
`make`  
you can also use  
`make clean`  



##contribute  
If you find any bugs, feel free to open an issue  
If you'd like other Paths than Arc etc. open an issue  
Also feel free to open a pull request, in case you added / fixed something yourself
