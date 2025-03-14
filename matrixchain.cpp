#include"include/matrix.hpp"
#include"include/matrixchain.hpp"

using namespace matrix;

template <> int MatrixBuff<int>::alloc_summ = 0;
template <> int MatrixBuff<int>::allocations = 0;
template <> int MatrixBuff<int>::deallocations = 0;

template <> int MatrixBuff<double>::alloc_summ = 0;
template <> int MatrixBuff<double>::allocations = 0;
template <> int MatrixBuff<double>::deallocations = 0;

template <> int MatrixBuff<long double>::alloc_summ = 0;
template <> int MatrixBuff<long double>::allocations = 0;
template <> int MatrixBuff<long double>::deallocations = 0;


int main(int argc, char** argv) {
    int count, cols, rows;
    MatrixChain<int> mc;
    std::cin >> count >> cols >> rows;
    mc.append(Matrix<int>(cols, rows));
    for(int i = 0; i < count - 1; ++i) {
        cols = rows;
        std::cin >> rows;
        mc.append(Matrix<int>(cols, rows));
    }

    mc.matrix_chain_order();
    std::cout << std::endl;
    std::cout << mc.naive_multiply_cost() / mc.matrix_chain_order();

    return 0;
}
