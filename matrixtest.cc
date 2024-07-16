#include<iostream>
#include"include/matrix.hpp"
#include<gtest/gtest.h>

using namespace matrix;

TEST(End2endTests, Integers150x150) {
    int true_det = 151;
    Matrix<int> mat_ = Matrix<int>::upper_triangular(150, true_det);

    mat_.shuffle();
    mat_.shuffle();
    mat_.shuffle();
    mat_.shuffle();

    clock_t start = clock();
    int det = mat_.calculate_det();
    clock_t stop = clock();

    std::cout << double(stop - start) / CLOCKS_PER_SEC << " secs \n";

    EXPECT_EQ(det, true_det);
}

TEST(End2endTests, GeneralInteger){
    double test_det, trues_count = 0.0;
    int dets_count = 42;

    for(int sz = 1; sz < 50; sz++){
        trues_count = 0;
        for(int det = -dets_count; det < dets_count; det++){
            Matrix<double> mat_ = Matrix<double>::upper_triangular(sz, det);

            mat_.shuffle();
            mat_.shuffle();

            test_det = mat_.calculate_det();
            if(std::fabs(test_det - det) < 1.0){ trues_count++; }
        }
        if(trues_count / 84.0 < 0.5){ std::cout << "Border size for doubles is " << --sz << std::endl; break; }
    }
}

TEST(End2endTests, Int3x3){
    std::vector<int> v = {48, 56, 0, -1, 23, 0, 0, 0, 1};
    Matrix<int> A(3, 3, v.begin(), v.end());
    int det = A.calculate_det();
    EXPECT_EQ(det, 1160);
}

TEST(End2endTests, Double3x3){
    std::vector<double> v = {2.09, 5.55, 4.93, 0.15, 8, 8.7, 0.87, 8.33, 4.68};
    Matrix<double> m(3, 3, v.begin(), v.end());
    double det = m.calculate_det();
    EXPECT_EQ(det, -63.255705);
}

TEST(UnitTests, Multiplication){
    std::vector<int> v = {1, 2, 1, 0};
    std::vector<int> w = {1, 2, 3, 4};
    std::vector<int> q = {7, 10, 1, 2};
    Matrix<int> A(2, 2, v.begin(), v.end());
    Matrix<int> B(2, 2, w.begin(), w.end());
    Matrix<int> C_true(2, 2, q.begin(), q.end());
    Matrix<int> C = Matrix<int>::multiply(A, B);
    EXPECT_TRUE(C_true.equal(C));
}

TEST(UnitTests, MatrixCtors){
    Matrix<int> M = Matrix<int>(2, 2, 1);
    EXPECT_EQ(M[0][0], 1);
    EXPECT_EQ(M[0][1], 1);
    EXPECT_EQ(M[1][0], 1);
    EXPECT_EQ(M[1][1], 1);
    EXPECT_EQ(M.cols(), 2);
    EXPECT_EQ(M.rows(), 2);

    Matrix<int> N = Matrix<int>::eye(3);
    std::vector<int> v{1, 0, 0, 0, 1, 0, 0, 0, 1};
    Matrix<int> Q = Matrix<int>(3, 3, v.begin(), v.end());
    EXPECT_TRUE(N.equal(Q));

    std::vector<int> w(4, 1);
    Matrix<int> S = Matrix<int>(2, 2, w.begin(), w.end());
    EXPECT_TRUE(S.equal(M));
}
