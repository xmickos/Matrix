#include<iostream>
#include"include/matrix.hpp"
#include<gtest/gtest.h>
#include<cstdlib>

namespace matrix {

    template <int Q, int C = -1> class MyInt final {

        private:
            int value;

        public:
            static int crash_cnt;
            static int allocations;
            static int deallocations;
            static int alloc_summ;

            MyInt(int val = 0) : value(val) {
                crash_cnt++;
                allocations++;
                alloc_summ++;
                std::cout << "MyInt()\n";
                if(Q == 43) throw std::bad_alloc();
                if(crash_cnt == C) throw std::bad_weak_ptr();
            }

            auto operator<=>(const MyInt& rhs) const = default;
            // bool operator==(const MyInt&) const;

            MyInt& operator+=(const MyInt& rhs) {
                std::cout << "+=\n";
                if(Q == 42 && rhs.get_value() == 43) { throw std::bad_function_call(); }
                value += rhs.value; return *this;
            }

            MyInt& operator*=(const MyInt& rhs) {
                std::cout << "*=\n";
                if(Q == 42 && rhs.get_value() == 43) { throw std::bad_function_call(); }
                if(Q == 43) { throw std::bad_function_call(); }
                value *= rhs.value; return *this;
            }

            std::ostream& operator<<(std::ostream& os) const { return os << value; }

            operator int() const {
                std::cout << "int()\n";
                if(Q == 42 && value == 43) { throw std::bad_function_call(); }
                if(Q == 43) { throw std::bad_function_call(); }
                return value;
            }

            int get_value() const noexcept { return value; }
            void crash_cnt_up() { crash_cnt++; }

            ~MyInt() {
                deallocations++;
                alloc_summ--;
            }
    };

    // std::ostream& MyInt::operator<<(std::ostream& os) { return os << rhs.get_value(); }

    MyInt<0> operator+(const MyInt<0>& lhs, const MyInt<0>& rhs) {
        std::cout << "+\n";
        MyInt tmp(lhs); tmp += rhs; return tmp;
    }

}

using namespace matrix;

template <typename T> int MatrixBuff<T>::alloc_summ = 0;
template <typename T> int MatrixBuff<T>::allocations = 0;
template <typename T> int MatrixBuff<T>::deallocations = 0;
template <int T, int C> int MyInt<T, C>::allocations = 0;
template <int T, int C> int MyInt<T, C>::deallocations = 0;
template <int T, int C> int MyInt<T, C>::alloc_summ = 0;
template <int T, int C> int MyInt<T, C>::crash_cnt = 0;

// TEST(UnitTests, Basics){ }

TEST(UnitTests, DISABLED_MatrixCtors){
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

TEST(UnitTests, Exceptions_Сtors){

    try {
        Matrix<MyInt<43>> A(3, 3); // 1 x MatrixBuff() and 1 x ~MatrixBuff();
    } catch(std::bad_alloc& e) {}

    EXPECT_EQ(MatrixBuff<MyInt<43>>::allocations, 1);
    EXPECT_EQ(MatrixBuff<MyInt<43>>::deallocations, 1);
    EXPECT_EQ(MatrixBuff<MyInt<43>>::alloc_summ, 0);

    try {
        Matrix<MyInt<42>> B(3); // 1 x MatrixBuff() and 1 x ~MatrixBuff();
    } catch(std::bad_function_call& e) {}

    EXPECT_EQ(MatrixBuff<MyInt<42>>::allocations, 1);
    EXPECT_EQ(MatrixBuff<MyInt<42>>::deallocations, 1);
    EXPECT_EQ(MatrixBuff<MyInt<42>>::alloc_summ, 0);

    try {
        Matrix<MyInt<42, 7>> B(3, 3, MyInt<42, 7>(997)); // 7 x MyInt() and 7 x ~MyInt()
    } catch(std::bad_weak_ptr& e) {}

    EXPECT_EQ(int(MyInt<42, 7>::allocations), 7);
    EXPECT_EQ(int(MyInt<42, 7>::deallocations), 7);
    EXPECT_EQ(int(MyInt<42, 7>::alloc_summ), 0);

    // A[0][0] = MyInt<0>(1);
    // A[1][1] = MyInt<0>(43);
    // A[2][2] = MyInt<0>(1);

    // MyInt a(1), b(2);

    // a += 1;
    // MyInt e = a + 1;
    // MyInt c = 1 + a;
    // b *= 2;
    // int d = 2 * b;
    // std::cout << a << " " /* << b << " " */ << c << " " /* << d << " " */ << e << " ";


    // std::cout << A.calculate_det() << std::endl;
}

TEST(UnitTests, DISABLED_Multiplication){
    std::vector<int> v = {1, 2, 1, 0};
    std::vector<int> w = {1, 2, 3, 4};
    std::vector<int> q = {7, 10, 1, 2};
    Matrix<int> A(2, 2, v.begin(), v.end());
    Matrix<int> B(2, 2, w.begin(), w.end());
    Matrix<int> C_true(2, 2, q.begin(), q.end());
    Matrix<int> C = Matrix<int>::multiply(A, B);
    EXPECT_TRUE(C_true.equal(C));
}

TEST(End2endTests, DISABLED_Int3x3){
    std::vector<int> v = {48, 56, 0, -1, 23, 0, 0, 0, 1};
    Matrix<int> A(3, 3, v.begin(), v.end());
    int det = A.calculate_det();
    EXPECT_EQ(det, 1160);
}

TEST(End2endTests, DISABLED_Double3x3){
    std::vector<double> v = {2.09, 5.55, 4.93, 0.15, 8, 8.7, 0.87, 8.33, 4.68};
    Matrix<double> m(3, 3, v.begin(), v.end());
    double det = m.calculate_det();
    EXPECT_EQ(det, -63.255705);
}

TEST(End2endTests, DISABLED_Integers150x150) {
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

TEST(End2endTests, DISABLED_GeneralInteger){
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
