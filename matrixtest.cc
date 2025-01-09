#include<iostream>
#include"include/matrix.hpp"
#include<gtest/gtest.h>
#include<cstdlib>

namespace matrix {

    template <bool throwing, int max_allocs_count = -1> class MyInt final {

        private:
            int value;

        public:
            static int crash_cnt;
            static int allocations;
            static int deallocations;
            static int alloc_summ;

            MyInt(int val = 0) : value(val) {
                if(throwing && crash_cnt == max_allocs_count) { std::cout << "throwing\n"; throw std::bad_alloc(); }
                if(!throwing && crash_cnt == max_allocs_count) throw std::bad_weak_ptr();
                crash_cnt++;
                allocations++;
                alloc_summ++;
                std::cout << "MyInt() " << crash_cnt << "\n";
            }

            auto operator<=>(const MyInt& rhs) const = default;

            MyInt& operator+=(const MyInt& rhs) {
                std::cout << "+=\n";
                if(throwing && rhs.get_value() == 43) { throw std::bad_function_call(); }
                value += rhs.value; return *this;
            }

            MyInt& operator*=(const MyInt& rhs) {
                std::cout << "*=\n";
                if(throwing && crash_cnt == max_allocs_count) { throw std::bad_function_call(); }
                if(throwing && rhs.get_value() == 43) { throw std::bad_function_call(); }
                value *= rhs.value; return *this;
            }

            std::ostream& operator<<(std::ostream& os) const { return os << value; }

            operator int() const {
                if(throwing && crash_cnt == max_allocs_count) { throw std::bad_function_call(); }
                std::cout << "int() " << crash_cnt << "\n";
                return value;
            }

            // operator float() const {
            //     std::cout << "float()\n";
            //     return static_cast<double>(value);
            // }

            int get_value() const noexcept { return value; }

            ~MyInt() {
                deallocations++;
                alloc_summ--;
                std::cout << "~MyInt()" << std::endl;
            }
    };

    MyInt<0> operator+(const MyInt<0>& lhs, const MyInt<0>& rhs) {
        std::cout << "+\n";
        MyInt tmp(lhs); tmp += rhs; return tmp;
    }

}

using namespace matrix;

template <typename T> int MatrixBuff<T>::alloc_summ = 0;
template <typename T> int MatrixBuff<T>::allocations = 0;
template <typename T> int MatrixBuff<T>::deallocations = 0;
template <bool throwing, int max_allocs_count> int MyInt<throwing, max_allocs_count>::allocations = 0;
template <bool throwing, int max_allocs_count> int MyInt<throwing, max_allocs_count>::deallocations = 0;
template <bool throwing, int max_allocs_count> int MyInt<throwing, max_allocs_count>::alloc_summ = 0;
template <bool throwing, int max_allocs_count> int MyInt<throwing, max_allocs_count>::crash_cnt = 0;

// TEST(UnitTests, Basics){ }

TEST(UnitTests, MatrixCtors){
    Matrix<int> M = Matrix<int>(2, 2);
    M.fill(1);
    EXPECT_EQ(M[0][0], 1);
    EXPECT_EQ(M[0][1], 1);
    EXPECT_EQ(M[1][0], 1);
    EXPECT_EQ(M[1][1], 1);
    EXPECT_EQ(M.cols(), 2);
    EXPECT_EQ(M.rows(), 2);
    std::cout << "M:" << std::endl;
    M.print();

    Matrix<int> N = Matrix<int>::eye(3);
    std::vector<int> v{1, 0, 0, 0, 1, 0, 0, 0, 1};
    Matrix<int> Q = Matrix<int>(3, 3, v.begin(), v.end());
    for(auto it_ = v.begin(); it_ != v.end(); ++it_) { std::cout << *it_ << " "; }
    N.print();
    Q.print();
    EXPECT_TRUE(N.equal(Q));

    std::vector<int> w(4, 1);
    Matrix<int> S = Matrix<int>(2, 2, w.begin(), w.end());
    EXPECT_TRUE(S.equal(M));
}

TEST(UnitTests, Ctor_exceptions){
    std::cout << "--- #0 — 1st ctor" << std::endl;

    try {
        Matrix<MyInt<false, 5>> A(3, 3); // 1 x MatrixBuff() and 1 x ~MatrixBuff(), 5 x MyInt() and 5 x ~MyInt();
    } catch(std::bad_weak_ptr& e) { std::cout << "Catched!" << std::endl; } // 11.12 - 10.01

    EXPECT_EQ(int(MatrixBuff<MyInt<false, 5>>::allocations), 1);
    EXPECT_EQ(int(MatrixBuff<MyInt<false, 5>>::allocations), int(MatrixBuff<MyInt<false, 5>>::deallocations));
    EXPECT_EQ(int(MatrixBuff<MyInt<false, 5>>::alloc_summ), 0);

    EXPECT_EQ(int(MyInt<false, 5>::allocations), 5);
    EXPECT_EQ(int(MyInt<false, 5>::allocations), int(MyInt<false, 5>::deallocations));
    EXPECT_EQ(int(MyInt<false, 5>::alloc_summ), 0);

    std::cout << "--- #1 — 2nd ctor" << std::endl;

    MyInt<true>::allocations = 0;
    MyInt<true>::deallocations = 0;
    MyInt<true>::alloc_summ = 0;

    try {
        Matrix<MyInt<true, 0>> B(3); // 1 x MatrixBuff() and 1 x ~MatrixBuff(), 0 x MyInt() and 0 x ~MyInt();
    } catch(std::bad_alloc& e) { std::cout << "Catched!" << std::endl; }

    EXPECT_EQ(int(MatrixBuff<MyInt<true, 0>>::allocations), 1);
    EXPECT_EQ(int(MatrixBuff<MyInt<true, 0>>::allocations), int(MatrixBuff<MyInt<true, 0>>::deallocations));
    EXPECT_EQ(int(MatrixBuff<MyInt<true, 0>>::alloc_summ), 0);

    EXPECT_EQ(int(MyInt<true>::allocations), 0);
    EXPECT_EQ(int(MyInt<true>::allocations), int(MyInt<true>::deallocations));
    EXPECT_EQ(int(MyInt<true>::alloc_summ), 0);

    std::cout << "--- #2 — 3rd ctor" << std::endl;

    MyInt<true, 7>::allocations = 0;
    MyInt<true, 7>::deallocations = 0;
    MyInt<true, 7>::alloc_summ = 0;

    try {
        Matrix<MyInt<true, 7>> B(3, 3, MyInt<true, 7>(0)); // 2 x MatrixBuff() and 2 x ~MatrixBuff(), 7 x MyInt() and 7 x ~MyInt();
    } catch(std::bad_alloc& e) { std::cout << "Catched!" << std::endl; }

    EXPECT_EQ(int(MatrixBuff<MyInt<true, 7>>::allocations), 2);
    EXPECT_EQ(int(MatrixBuff<MyInt<true, 7>>::allocations), int(MatrixBuff<MyInt<true, 7>>::deallocations));
    EXPECT_EQ(int(MatrixBuff<MyInt<true, 7>>::alloc_summ), 0);

    EXPECT_EQ(int(MyInt<true, 7>::allocations), 7);
    EXPECT_EQ(int(MyInt<true, 7>::allocations), int(MyInt<true, 7>::deallocations));
    EXPECT_EQ(int(MyInt<true, 7>::alloc_summ), 0);

    std::cout << "--- #3 — Copy assign" << std::endl;

    try {
        Matrix<MyInt<true, 23>> A(3, 3);    // 9 x MyInt() and 1 x MatrixBuff();
        std::cout << "B(...)." << std::endl;
        Matrix<MyInt<true, 23>> B(3, 3, MyInt<true, 23>(1)); // 10 x MyInt() and 3 x MatrixBuff();
        std::cout << "B = ..." << std::endl;
        B = A;  // 4 x MyInt() and 1 x MatrixBuff();
    } catch(std::bad_alloc& e) { std::cout << "Catched!" << std::endl; }

    EXPECT_EQ(int(MatrixBuff<MyInt<true, 23>>::allocations), 5);
    EXPECT_EQ(int(MatrixBuff<MyInt<true, 23>>::allocations), int(MatrixBuff<MyInt<true, 23>>::deallocations));
    EXPECT_EQ(int(MatrixBuff<MyInt<true, 23>>::alloc_summ), 0);

    EXPECT_EQ(int(MyInt<true, 23>::allocations), 23);
    EXPECT_EQ(int(MyInt<true, 23>::allocations), int(MyInt<true, 23>::deallocations));
    EXPECT_EQ(int(MyInt<true, 23>::alloc_summ), 0);

    std::cout << "--- #3 — 4th ctor" << std::endl;

    try {
        std::vector<MyInt<false, 17>> v(9);     // 9 x MyInt();
        std::cout << "v constructed." << std::endl;
        Matrix<MyInt<false, 17>> A(3, 3, v.begin(), v.end()); // 2 x MatrixBuff() and 8 x MyInt();
    } catch(std::bad_weak_ptr& e) { std::cout << "Catched!" << std::endl; }

    EXPECT_EQ(int(MatrixBuff<MyInt<false, 17>>::allocations), 2);
    EXPECT_EQ(int(MatrixBuff<MyInt<false, 17>>::allocations), int(MatrixBuff<MyInt<false, 17>>::deallocations));
    EXPECT_EQ(int(MatrixBuff<MyInt<false, 17>>::alloc_summ), 0);

    EXPECT_EQ(int(MyInt<false, 17>::allocations), 17);
    EXPECT_EQ(int(MyInt<false, 17>::allocations), int(MyInt<false, 17>::deallocations));
    EXPECT_EQ(int(MyInt<false, 17>::alloc_summ), 0);

}

TEST(End2EndTests, Determinant_exceptions){
    std::cout << "--- #0 — lpu_decompose." << std::endl;

    MatrixBuff<MyInt<true, 12>>::allocations = 0;
    MatrixBuff<MyInt<true, 12>>::deallocations = 0;
    MatrixBuff<MyInt<true, 12>>::alloc_summ = 0;
    MyInt<true, 12>::allocations = 0;
    MyInt<true, 12>::deallocations = 0;
    MyInt<true, 12>::alloc_summ = 0;

    try {
        Matrix<MyInt<true, 12>> A(3, 3);

        A[0][0] = MyInt<true, 12>(1);
        A[1][1] = MyInt<true, 12>(43);
        A[2][2] = MyInt<true, 12>(1);

        std::cout << "calculating" << std::endl;

        A.calculate_det();  // fail in lpu_decompose()
    } catch(std::bad_function_call& e) { std::cout << "Catched!" << std::endl; }

    EXPECT_EQ(int(MatrixBuff<MyInt<true, 12>>::allocations), int(MatrixBuff<MyInt<true, 12>>::deallocations));
    EXPECT_EQ(int(MatrixBuff<MyInt<true, 12>>::alloc_summ), 0);

    EXPECT_EQ(int(MyInt<true, 12>::allocations), int(MyInt<true, 12>::deallocations));
    EXPECT_EQ(int(MyInt<true, 12>::alloc_summ), 0);

    #if 0
        std::cout << "--- #1 — triangle_det." << std::endl;

        MyInt<true, 9>::allocations = 0;
        MyInt<true, 9>::deallocations = 0;
        MyInt<true, 9>::alloc_summ = 0;

        try {
            Matrix<MyInt<true, 9>> A(3, 3);
            A.calculate_det(); // fail in triangle_det
        } catch(std::bad_alloc& e) { std::cout << "Catched!" << std::endl; }

        EXPECT_EQ(int(MatrixBuff<MyInt<true, 9>>::allocations), int(MatrixBuff<MyInt<true, 9>>::deallocations));
        EXPECT_EQ(int(MatrixBuff<MyInt<true, 9>>::alloc_summ), 0);

        EXPECT_EQ(int(MyInt<true, 9>::allocations), int(MyInt<true, 9>::deallocations));
        EXPECT_EQ(int(MyInt<true, 9>::alloc_summ), 0);

    #endif
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
