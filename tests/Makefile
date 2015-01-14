CC = g++
DEBUG ?= 0

ifeq ($(DEBUG), 1)
  	CFLAGS = -std=c++11 -DDEBUG
  	TARGET = bin/test_lib_2d_DEBUG
else
    CFLAGS = -std=c++11 -DNDEBUG
    TARGET = bin/test_lib_2d
endif

ifeq  ($(DOUBLE),1)
	CFLAGS += -D USE_DOUBLE
else ifeq  ($(FLOAT),1)
	CFLAGS += -D USE_FLOAT
endif


all: test

test:
	$(CC) $(CFLAGS) test_lib_2d.cpp -o $(TARGET)
	./$(TARGET)

clean:
	rm -f *o *.so *.dll *.exe bin/*.exe
