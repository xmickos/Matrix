#include"include/matrix.hpp"

using namespace matrix;

int main(int argc, char** argv) {
    #if 0
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
    #endif

    std::cout << "std::is_move_assignable<Matrix<int>>: " << std::is_move_assignable<Matrix<int>>::value << std::endl;
    std::cout << "std::is_move_constructible<Matrix<int>>: " << std::is_move_constructible<Matrix<int>>::value << std::endl;
    // std::cout << "std::is_convertible<int, matrix::A>::value: " << std::is_convertible<int, matrix::A>::value << std::endl;
    // std::cout << "static_cast<A>(1): " << static_cast<matrix::A>((int)1) << std::endl;

    try {
        Matrix<int> m = Matrix<int>::eye(3);
        m.fill(2);
        m.print();
    }
    catch(std::bad_alloc& e){
        std::cout << "Catched.\n";
    }

    return 0;
}
