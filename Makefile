bigint: bin/bigint.o
	g++ bin/bigint.o -o big-int

bigint-tests: tests/bigint.o
	g++ src/bigint-test.cpp -o 

build: src/bigint.cpp
	g++ -c src/bigint.cpp -o bin/bigint.o

clean: 
	rm bin/*.o