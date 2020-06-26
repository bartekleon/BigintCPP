rm -r build
mkdir -p build

cd build
cmake -DCMAKE_CXX_FLAGS="-Werror" -DCMAKE_BUILD_TYPE=Coverage -DBUILD_TESTS=ON ..
make Bigint_coverage