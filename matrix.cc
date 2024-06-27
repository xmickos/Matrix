#include"matrix.hpp"
#include<ctime>

using namespace matrix;

int main(int argc, char** argv){

    // g++ -std=c++17 -DDEBUG_ -fsanitize=address,bool,bounds matrix.cc -o matrix

//     int dets[100][184], goods = 0;
//
//     for(int sz = 1; sz < 100; sz++){
//         for(int det = -42; det < 142; det++){
//             Matrix<long> mat_ = Matrix<long>::upper_triangular(det, sz);
//
//             mat_.shuffle();
//             mat_.shuffle();
//
//             dets[sz][det] = mat_.calculate_det();
//             std::cout << "SZ: " << sz << ", DET: " << det << ", calc_det: " << dets[sz][det];
//             if(dets[sz][det] == det){ std::cout << "\t\t\t\t\t\t\t\t\t+"; goods++; }
//             std::cout << std::endl;
//         }
//     }
//     std::cout << "total goods: " << goods;


    Matrix<int> mat_ = Matrix<int>::upper_triangular(15, 100);


    std::cout << "Init matrix:" << std::endl;
    mat_.python_print();
    mat_.shuffle();
    mat_.shuffle();
    mat_.shuffle();
    mat_.shuffle();
    std::cout << "Shuffled matrix:" << std::endl;
    mat_.python_print();

    clock_t start = clock();
    Matrix<int> Fn = mat_.det_bird();
    clock_t stop = clock();
    std::cout << "Det: " << std::endl;
    Fn.print();


    std::cout << double(stop - start) / CLOCKS_PER_SEC << " secs \n";

    // std::vector<int> v = {48, 56, 0, -1, 23, 0, 0, 0, 1};
    // Matrix<int> A(3, 3, v.begin(), v.end());

//     Matrix<int> Fn = A.det_bird();
//
//     Fn.print();


    return 0;
}
