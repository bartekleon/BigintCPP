rm -r build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=ON -DBUILD_TESTS=ON .. && make
make test
make Bigint_coverage
cd ..
coveralls --root . -E ".*gtest.*" -E ".*CMakeFiles.*"
