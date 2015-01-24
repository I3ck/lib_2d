CC = g++
DEBUG ?= 0
CFLAGS = -std=c++11

.PHONY: all, run_tests

ifeq ($(DEBUG), 1)
  	CFLAGS += -DDEBUG
  	TARGET = bin/test_lib_2d_DEBUG
else
    CFLAGS += -DNDEBUG
    TARGET = bin/test_lib_2d
endif


all: make_tests run_tests

make_tests:
	$(CC) $(CFLAGS) -D USE_DOUBLE test_lib_2d.cpp -o $(TARGET)_DOUBLE
	$(CC) $(CFLAGS) -D USE_LDOUBLE test_lib_2d.cpp -o $(TARGET)_LDOUBLE
	$(CC) $(CFLAGS) -D USE_FLOAT test_lib_2d.cpp -o $(TARGET)_FLOAT

run_tests:
	./$(TARGET)_DOUBLE
	./$(TARGET)_LDOUBLE
	./$(TARGET)_FLOAT

clean:
	rm -rf *o *.so *.dll *.exe bin/* bin/ obj/* obj/
