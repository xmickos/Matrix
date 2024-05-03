#include "matrix.hpp"

using namespace matrix;

int main(int argc, char** argv){

    // g++ -std=c++17 -DDEBUG_ -fsanitize=address,bool,bounds matrix.cc -o matrix

    /*
        TODO:
        1) Add func for generating matrix with given det()

        Способы генерации матрицы с заданным определителем:
        1) Перестановка строк - опредитель меняет знак
        2) Умножение строки на число - определитель умножается на это число
        3) Прибавление к одной строке другой, умноженной на число - определитель не поменяется
        4) Перемножение двух квадратных матрицы - определители перемножатся
        5) Определитель верхнетреугольной матрицы равен произведению диагональных элементов
        6) Транспонирование - определитель не меняется

    */

    // Matrix<int> m1eye = Matrix<int>::eye(8);    // iterator ctor
//     Matrix<int> m2eye = Matrix<int>::eye(8);    // iterator ctor
//
//     Matrix<int> m3 = m1eye; // Copy ctor
//
//     Matrix<int> m4(std::move(m3));      // Move ctor
//     Matrix<int> m5 = std::move(m1eye);  // Move ctor
//     m5 = std::move(m4);                 // Move assign
//
//     m5.print();
//
//     m5.add_columns(0, 1);
//
//     m5.print();
//
//     m5.negate();
//
//     m5.print();
//
//     m5[1][0] = 3;
//     m5[1][1] = 3;
//     m5[1][2] = 3;
//     m5[1][3] = 3;
//     m5[1][4] = 3;
//     m5[1][5] = 3;
//
//     m5.print();
//
//     m5.print_row(1);
//
//     Matrix<int> m5_ = m5;
//
//     m5.transpose();
//
//     m5.print();
//
//     m5.print_row(1);
//
//     int trc_ = m5.trace();
//
//     std::cout << "trace: " << trc_ << std::endl;
//
//     std::cout << "eq : " << m5.equal(m5_) << " " << m5.equal(m4) << std::endl;


    Matrix<int> mat_ = Matrix<int>::upper_triangular(13, 6);


    std::cout << "Init matrix:" << std::endl;
    mat_.print();
    mat_.shuffle();
    std::cout << "Shuffled matrix:" << std::endl;
    mat_.print();
    int det = mat_.calculate_det();
    std::cout << "Det: " << det << std::endl;

    return 0;
}
