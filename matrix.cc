#include "matrix.hpp"

using namespace matrix;

int main(int argc, char** argv){

    // g++ -std=c++17 -DDEBUG_ -fsanitize=address,bool,bounds matrix.cc -o matrix

    Matrix<int> m1eye = Matrix<int>::eye(8);
    Matrix<int> m2eye = Matrix<int>::eye(8);

    Matrix<int> m3 = Matrix<int>::Matrix_add(m1eye, m2eye);

    Matrix<int> m4(m3);
    Matrix<int> m5 = m3;

    const Matrix<int> m10 = Matrix<int>::eye(10);

    m4 = std::move(m10);

    m4.print();

    m1eye.print();

    std::cout << m1eye.cols() << std::endl;

    return 0;
}
