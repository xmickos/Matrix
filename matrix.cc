#include"include/matrix.hpp"
#undef DEBUG_
#include<ctime>
#include<fstream>

using namespace matrix;

int main(int argc, char** argv) {

//     double dets[100][84], goods = 0;
//
//     for(int sz = 25; sz < 100; sz++){
//         for(int det = -42; det < 42; det++){
//             Matrix<double> mat_ = Matrix<double>::upper_triangular(det, sz);
//
//             mat_.shuffle();
//             mat_.shuffle();
//
//             dets[sz][det + 42] = mat_.calculate_det();
//             std::cout << "SZ: " << sz << ", DET: " << det << " vs " << dets[sz][det + 42];
//             if(std::abs(dets[sz][det + 42] - det) < 1.0){ std::cout << "\t\t\t\t\t\t\t\t\t+"; goods++; }
//             std::cout << std::endl;
//             std::cin.get();
//         }
//     }
//     std::cout << "total goods: " << goods;


    Matrix<int> mat_ = Matrix<int>::upper_triangular(151, 100);


    std::cout << "Init matrix:" << std::endl;
    // mat_.python_print();
    mat_.shuffle();
    mat_.shuffle();
    mat_.shuffle();
    mat_.shuffle();
    std::cout << "Shuffled matrix:" << std::endl;
    // mat_.python_print();

    clock_t start = clock();
    int det = mat_.calculate_det();
    clock_t stop = clock();
    std::cout << "Det: " << det << std::endl;


    std::cout << double(stop - start) / CLOCKS_PER_SEC << " secs \n";



//     std::vector<int> v = {48, 56, 0, -1, 23, 0, 0, 0, 1};
//     Matrix<int> A(3, 3, v.begin(), v.end());
//     std::cout << "init: " << std::endl;
//     A.print();
//
//     std::cout << "\ndet: " <<  A.calculate_det() << std::endl;

// ––Multiplication speed test––

//     std::ofstream output;
//     output.open("multiplication_02_reversed_classic.txt");
//     std::vector<int> vec(100 * 100);
//
//     for(int i = 0; i < 100 * 100; i++){
//         vec.push_back(rand() % 100);
//     }
//
//     for(int i = 1; i < 100; i++){
//         Matrix<int> A(i, i, vec.begin(), std::next(vec.begin(), i * i));
//
//         clock_t start = clock();
//         Matrix<int> C = Matrix<int>::multiply(A, A);
//         clock_t stop = clock();
//         output << i << ", " << double(stop - start) / CLOCKS_PER_SEC << std::endl;
//     }

    // ––Multiplication single test––

//     std::vector<int> v = {1, 2, 1, 0};
//     std::vector<int> w = {1, 2, 3, 4};
//     std::vector<int> q = {7, 10, 1, 2};
//     Matrix<int> A(2, 2, v.begin(), v.end());
//     Matrix<int> B(2, 2, w.begin(), w.end());
//     Matrix<int> C_true(2, 2, q.begin(), q.end());
//     Matrix<int> copied = Matrix<int>::zeros(2, 2);
//
//     A.print();
//     B.print();
//     Matrix<int> C = Matrix<int>::multiply(A, B);
//     C.print();
//     std::copy(C_true[0].row, C_true[0].row + C_true.cols(), copied[0].row);
//     std::copy(C_true[1].row, C_true[1].row + C_true.cols() - 1, copied[1].row);
//
//     std::cout << std::endl << "Equality: " << C.equal(C_true) << std::endl;
//     std::cout << std::endl << "Copied: " << std::endl;
//     copied.print();
//
//     ––Multiplication single test––
//
//     std::vector<double> v = {2.09, 5.55, 4.93, 0.15, 8, 8.7, 0.87, 8.33, 4.68};
//     Matrix<double> m(3, 3, v.begin(), v.end());
//     std::cout.precision(10);
//     std::cout << m.calculate_det() << std::endl;

    return 0;
}
