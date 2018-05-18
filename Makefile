CC=gcc
CFLAGS=-Wall -g -O1
LDFLAGS=

all: memcpy-test

memcpy-test: memcpy-test.o delaymemcpy.o

memcpy-test.o: memcpy-test.c delaymemcpy.h
delaymemcpy.o: delaymemcpy.c delaymemcpy.h

clean:
	-rm -rf memcpy-test.o delaymemcpy.o memcpy-test
