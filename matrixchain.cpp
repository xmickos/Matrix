#include"include/matrix.hpp"
#include"include/matrixchain.hpp"
#include"allocations.cc"

using namespace matrix;


int main(int argc, char** argv) {
    int count, cols, rows;
    MatrixChain<int> mc;
    std::cin >> count >> cols >> rows;
    mc.append(Matrix<int>(cols, rows));
    for(int i = 0; i < count - 2; ++i) {
        cols = rows;
        std::cin >> rows;
        mc.append(Matrix<int>(cols, rows));
    }
    std::cout << mc.naive_multiply_cost() / static_cast<long double>(mc.matrix_chain_order());

    return 0;
}
