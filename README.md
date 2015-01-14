# lib_2d
A c++ library for paths defined by points within the 2d space (using floating point data types)  
using Catch as testing framework https://github.com/philsquared/Catch  
##usage  
add inc/ to your include path and include lib_2d.h within your code  
/tests/test_lib_2d.cpp also provides a basic usage example
##running the tests  
to run the tests  
cd tests/  
  
to test doubles:
make DOUBLE=1  
  
to test long doubles:
make LDOUBLE = 1
  
to test floats:
make FLOAT=1

##contribute  
If you find any bugs, feel free to open an issue  
If you'd like other Paths than Arc etc. open an issue  
Also feel free to open a pull request, in case you added / fixed something yourself
