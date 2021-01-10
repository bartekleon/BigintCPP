sudo apt-get update -y

pip install codecov

sudo apt-get install -y lcov
sudo apt-get install -y clang-tidy-10
sudo apt-get install -y clang-format-10

# Set default values for clang and clang tools for version 10
# It is a minimum version to properly handle C++17 standard

sudo update-alternatives --remove-all clang
sudo update-alternatives --remove-all clang-tidy
sudo update-alternatives --remove-all clang-format

sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-10 100
sudo update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-10 100
sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-10 100

# Debug - Check if versions of packages is "10"

clang --version
clang-tidy --version
clang-format --version