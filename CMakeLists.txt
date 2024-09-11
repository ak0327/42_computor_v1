cmake_minimum_required(VERSION 3.22)
project(computor_v1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address,undefined -fno-omit-frame-pointer")


## google test -----------------------------------------------------------------
include(FetchContent)
include(GoogleTest)

FetchContent_Declare(
        googletest
        DOWNLOAD_EXTRACT_TIMESTAMP true
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()


## includes --------------------------------------------------------------------
include_directories(
        srcs
        srcs/Calculator
        srcs/Parser
        srcs/Result
        srcs/Tokenizer
        tests/utest
)

## srcs ------------------------------------------------------------------------
# test code
set (computor_srcs
        srcs/computor.cpp
        srcs/Calculator/Calculator.cpp
        srcs/Parser/Parser.cpp
        srcs/Tokenizer/Tokenizer.cpp
)


# test code
set (utest_srcs
        tests/utest/TestCalcEquation.cpp
        tests/utest/TestParser.cpp
        tests/utest/TestTokenizer.cpp
)


add_executable(computor
        srcs/main.cpp
        ${computor_srcs}
)


add_executable(utest
        ${computor_srcs}
        ${utest_srcs}
)


## test ------------------------------------------------------------------------
target_link_libraries(
        utest
        GTest::gtest_main
        GTest::gmock
)

gtest_discover_tests(utest)
