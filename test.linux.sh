mkdir -p build

cd build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Coverage -DBUILD_TESTS=ON ..
make
make Bigint_coverage