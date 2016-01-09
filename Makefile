CC = g++
DEBUG ?= 0
CFLAGS = -std=c++11 -Wall -Wextra -Werror -fmax-errors=10

.PHONY: tests, run_tests

ifeq ($(DEBUG), 1)
  	CFLAGS += -DDEBUG
  	TARGET = bin/test_lib_2d_DEBUG
else
    CFLAGS += -DNDEBUG
    TARGET = bin/test_lib_2d
endif


tests: make_tests run_tests

make_tests:
	mkdir -p bin/
	$(CC) $(CFLAGS) -D USE_DOUBLE tests/test_lib_2d.cpp -o $(TARGET)_DOUBLE
	$(CC) $(CFLAGS) -D USE_LDOUBLE tests/test_lib_2d.cpp -o $(TARGET)_LDOUBLE
	$(CC) $(CFLAGS) -D USE_FLOAT tests/test_lib_2d.cpp -o $(TARGET)_FLOAT

run_tests:
	./$(TARGET)_DOUBLE
	./$(TARGET)_LDOUBLE
	./$(TARGET)_FLOAT

clean:
	rm -rf *o *.so *.dll *.exe bin/* bin/ obj/* obj/
