mkdir -p build

cd build
cmake -DCMAKE_CXX_FLAGS="-Werror" -DCMAKE_BUILD_TYPE=Coverage -DUSE_CLANG_TIDY=True -DBUILD_TESTS=ON ..
make
make Bigint_coverage