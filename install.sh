sudo apt-get update -y

pip install codecov
sudo apt-get install -y lcov
sudo apt-get install -y clang-tidy-10
sudo apt-get install -y clang-format-10

sudo update-alternatives --remove-all clang
sudo update-alternatives --remove-all clang-tidy
sudo update-alternatives --remove-all clang-format

sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-10 100
sudo update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-10 100
sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-10 100
sudo update-alternatives --install /usr/bin/clang-format-6.0 clang-format-6.0 /usr/bin/clang-format-10 100

clang --version
clang-tidy --version
clang-format --version