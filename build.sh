mkdir -p build

cd build
cmake -DCMAKE_CXX_FLAGS="-Werror" -DUSE_CLANG_TIDY=True ..
make