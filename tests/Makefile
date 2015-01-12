CC = g++
DEBUG ?= 0

ifeq ($(DEBUG), 1)
  	CFLAGS = -std=c++11 -DDEBUG
  	TARGET = bin/test_lib_2d_DEBUG
else
    CFLAGS = -std=c++11 -DNDEBUG
    TARGET = bin/test_lib_2d
endif


all: test

test:
	$(CC) $(CFLAGS) test_lib_2d.cpp -o $(TARGET)

clean:
	rm -f *o *.so *.dll *.exe bin/*.exe
