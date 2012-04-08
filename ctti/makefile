.PHONY: all clean init

all: bin/client

init:
	mkdir lib bin build

bin/client: build/client.o lib/libctti.a
	g++ -g -o bin/client build/client.o -Llib -lctti

lib/libctti.a: build/ctti.o
	ar rcs lib/libctti.a build/ctti.o

build/client.o: client/client.cpp build/ctti.o
	g++ -g -c -o build/client.o client/client.cpp

build/ctti.o: ctti/src/ctti.cpp ctti/ctti.h
	g++ -g -c -o build/ctti.o ctti/src/ctti.cpp

clean:
	rm -r lib/* build/* bin/*

