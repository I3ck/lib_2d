#lib_2d
A c++ library for paths defined by points within the 2d space (using floating point data types)  
using Catch as testing framework https://github.com/philsquared/Catch  



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


##usage  
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
