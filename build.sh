rm -r build
mkdir -p build

cd build
cmake -Wall -pedantic -Werror -Wno-variadic-macros -Wno-long-long -Wno-shadow -DCMAKE_BUILD_TYPE=Coverage -DCMAKE_BUILD_TYPE=ON -DBUILD_TESTS=ON .. && make
make Bigint_coverage
codecov -t 8d1c8c84-247f-4c93-b694-77494affac62 -X gcov