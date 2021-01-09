sudo apt-get update -y

chmod +x llvm.sh
sudo ./llvm.sh

pip install codecov
sudo apt-get install -y lcov
sudo apt-get install -y clang-tools
