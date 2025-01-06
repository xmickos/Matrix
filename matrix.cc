#include"include/matrix.hpp"

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
    int cols;
    double cumsum = 0.0, float_tolerance = 1e-10;
    std::cin >> cols;
    std::vector<double> vec(cols * cols);
    std::vector<int> vec_int(cols * cols);
    for(int i = 0; i < cols * cols; ++i){
        std::cin >> vec[i];
        vec_int[i] = static_cast<int>(vec[i]);
        cumsum += std::fabs(vec[i] - vec_int[i]);
    }
    if(std::fabs(cumsum) < float_tolerance){
        Matrix<int> m(cols, cols, vec_int.begin(), vec_int.end());
        std::cout << m.calculate_det();
    }else{
        Matrix<double> m(cols, cols, vec.begin(), vec.end());
        std::cout << m.calculate_det();
    }
    return 0;
}
