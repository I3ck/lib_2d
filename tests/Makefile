CC = g++
DEBUG ?= 0

.PHONY: all

ifeq ($(DEBUG), 1)
  	CFLAGS = -std=c++11 -DDEBUG
  	TARGET = bin/test_lib_2d_DEBUG
else
    CFLAGS = -std=c++11 -DNDEBUG
    TARGET = bin/test_lib_2d
endif


all: test_double test_ldouble test_float

test_double:
	$(CC) $(CFLAGS) -D USE_DOUBLE test_lib_2d.cpp -o $(TARGET)_DOUBLE
	./$(TARGET)_DOUBLE

test_ldouble:
	$(CC) $(CFLAGS) -D USE_LDOUBLE test_lib_2d.cpp -o $(TARGET)_LDOUBLE
	./$(TARGET)_LDOUBLE

test_float:
	$(CC) $(CFLAGS) -D USE_FLOAT test_lib_2d.cpp -o $(TARGET)_FLOAT
	./$(TARGET)_FLOAT

clean:
	rm -rf *o *.so *.dll *.exe bin/* bin/ obj/* obj/
