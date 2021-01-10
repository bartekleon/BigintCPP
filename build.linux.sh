mkdir -p build

cd build
cmake -G "Unix Makefiles" -DCMAKE_CXX_FLAGS="-Werror" -DRUN_CLANG_TOOLS=ON -DBUILD_TESTS=OFF ..
make
make clang-tidy
make clang-format