cmake_minimum_required(VERSION 3.16)
project(TextEditor VERSION 0.1.0 LANGUAGES CXX)

# Use C++17 everywhere, and fail if the compiler can't do it.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Lets us run tests with "ctest".
enable_testing()

add_subdirectory(tests)
add_subdirectory(src/gui)