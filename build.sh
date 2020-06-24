rm -r build
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=ON -DBUILD_TESTS=ON .. && make
make test
make Bigint_coverage
cd ..
cd ${TRAVIS_BUILD_DIR}
lcov --directory . --capture --output-file coverage.info # capture coverage info
lcov --remove coverage.info '/usr/*' --output-file coverage.info # filter out system
lcov --list coverage.info #debug info
# Uploading report to CodeCov
bash <(curl -s https://codecov.io/bash) || echo "Codecov did not collect coverage reports"
