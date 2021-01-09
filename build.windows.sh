mkdir -p build

cd build
cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS="-Werror" -DRUN_CLANG_TOOLS=ON -DBUILD_TESTS=OFF ..
make
make clang-tidy
make clang-format