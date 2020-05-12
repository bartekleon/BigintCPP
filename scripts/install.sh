#!/bin/bash

wget https://github.com/google/googletest/archive/release-1.10.0.tar.gz

tar xf release-1.10.0.tar.gz
rm -r release.1.10.0.targ.gz
cd googletest-release-1.10.0
cmake .
make
cd ..

rm -r googletest-release-1.10.0/
