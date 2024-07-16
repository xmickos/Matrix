cmake_minimum_required(VERSION 3.11)
project("matrix")
project("matrixtest")

SET(SRCS include/matrix.hpp matrix.cc)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(matrixlib
    STATIC ${SRCS}
)
add_executable(matrix ${SRCS})
add_executable(matrixtest include/matrix.hpp matrixtest.cc)

target_link_libraries(matrix
    PRIVATE matrixlib
)
target_link_libraries(matrixtest
    PRIVATE
        GTest::gtest_main
        matrixlib
)


target_compile_features(matrixlib
    PUBLIC cxx_std_20
)
target_compile_definitions(matrixlib
    PUBLIC
        TRANSF_MATMUL=1
        PYTHON_DEBUG=1
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(matrixtest)
