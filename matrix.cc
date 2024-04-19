#include "matrix.hpp"

using namespace matrix;

int main(int argc, char** argv){

    // g++ -std=c++17 -DDEBUG_ -fsanitize=address,bool,bounds matrix.cc -o matrix

    /*
        TODO:
        1) Add func for generating matrix with given det()

    */

    Matrix<int> m1eye = Matrix<int>::eye(8);    // iterator ctor
    Matrix<int> m2eye = Matrix<int>::eye(8);    // iterator ctor

    Matrix<int> m3 = m1eye; // Copy ctor

    Matrix<int> m4(std::move(m3));      // Move ctor
    Matrix<int> m5 = std::move(m1eye);  // Move ctor
    m5 = std::move(m4);                 // Move assign

    m5.print();

    return 0;
}
