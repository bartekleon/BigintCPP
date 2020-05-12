# Building tests
test: tests/bigint.o
	g++ src/bigint-test.cpp -o bin/tests-bigint.o 

# Installing & Building deps
install: 
	./scripts/install.sh

# Building bigint
build: src/bigint.cpp
	g++ -c src/bigint.cpp -o bin/benches-bigint.o

# Cleaning executables
clean: 
	rm bin/*.o
