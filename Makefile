# the compiler to use.
CC=g++
AR=ar
# options I'll pass to the compiler.
CFLAGS=-c -Wall
LIBFLAGS= -L. -llibsvm_wrapper# --verbose
ARFLAGS=crf

all: test

lib: libsvm_wrapper.a

libsvm_wrapper.a: libsvm_wrapper.o svm.o
	$(AR) $(ARFLAGS) liblibsvm_wrapper.a libsvm_wrapper.o svm.o
	
libsvm_wrapper.o: libsvm_wrapper.cpp libsvm_wrapper.h
	$(CC) $(CFLAGS) libsvm_wrapper.cpp

svm.o: svm.cpp svm.h
	$(CC) $(CFLAGS) svm.cpp

test: lib test.cpp
	$(CC) $(CFLAGS) -std=c++11 test.cpp
	$(CC) test.o $(LIBFLAGS) -o test

.PHONY: clean
clean:
	rm -rf *.o
	rm -f libsvm_wrapper.a
	rm -f test
