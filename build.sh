rm -r build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=ON -DBUILD_TESTS=ON .. && make
make test
make Bigint_coverage
cd ..
codecov -t 8d1c8c84-247f-4c93-b694-77494affac62 -X gcov
