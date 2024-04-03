#include "matrix.hpp"


int main(int argc, char** argv){

    // std::vector<int> a(2500, 42);


    Matrix<int> m = Matrix<int>::eye(8);

    m.print();

    return 0;
}
