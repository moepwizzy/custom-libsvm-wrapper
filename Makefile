# the compiler to use.
CC=g++
AR=ar
# options I'll pass to the compiler.
CFLAGS=-c -Wall
LIBFLAGS=
ARFLAGS=crf

all: libsvm_wrapper.o svm.o
	$(AR) $(ARFLAGS) libsvm_wrapper.a libsvm_wrapper.o svm.o
	
libsvm_wrapper.o: libsvm_wrapper.cpp libsvm_wrapper.h
	$(CC) $(CFLAGS) libsvm_wrapper.cpp

svm.o: svm.cpp svm.h
	$(CC) $(CFLAGS) svm.cpp

.PHONY: clean
clean:
	rm -rf *.o
	rm -f libsvm_wrapper.a
