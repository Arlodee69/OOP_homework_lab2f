cmake_minimum_required(VERSION 3.10)
project(five_library)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавление опций компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Включение директорий
include_directories(include)

# Установка Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Основная библиотека
add_library(five_lib src/function.cpp)

# Тесты
enable_testing()

add_executable(five_tests test/test_five.cpp)
target_link_libraries(five_tests five_lib gtest_main)

add_test(NAME FiveLibraryTests COMMAND five_tests)