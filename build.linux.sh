mkdir -p build

cd build

exit_if_error() {
  local exit_code=$1
  shift
  [[ $exit_code ]] &&
    ((exit_code != 0)) && {
      exit "$exit_code"
    }
}

cmake -G "Unix Makefiles" -DCMAKE_CXX_FLAGS="-Werror" -DRUN_CLANG_TOOLS=ON -DBUILD_TESTS=OFF .. || exit_if_error
make || exit_if_error
make clang-tidy || exit_if_error
make clang-format || exit_if_error