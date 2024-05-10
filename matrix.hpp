#include<vector>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<cstdlib>
#include<execution>
#include<cmath>

namespace matrix {

    template <typename T> class Matrix;

    template <typename T>
    struct lpu_decomposition{
        Matrix<T> L, U;
        bool sign = false;
    };

    template <typename T>       // T = {int, char, double}
    class Matrix{
        private:
            T** data = nullptr;
            int cols_, rows_;
            const int rand_actions_count = 4;

            struct ProxyRow {
                T* row;

                T& operator[](int j) { return row[j]; }
                const T& operator[](int j) const { return row[j]; }
            };

        public:

            Matrix(int cls, int rws, T val = T{}) : cols_(cls), rows_(rws) {
                data = new T*[cols_];
                for(int i = 0; i < cols_; ++i){
                    data[i] = new T[rows_];
                    std::fill_n(data[i], rows_, val);
                }

                #ifdef DEBUG_
                std::cout << "Matrix base ctor" << std::endl;
                #endif
            }

            static Matrix eye(int sz_) {
                if(sz_ <= 0){
                    std::cout << "Matrix size must be positive integer." << std::endl;
                    abort();
                }

                int* seq_ = new int[sz_ * sz_]{};

                for(int i = 0; i < sz_; ++i){
                    seq_[ i * (sz_ + 1) ] = static_cast<T>(1);
                }

                Matrix<T> m(sz_, sz_, seq_, seq_ + sz_ * sz_);
                delete[] seq_;

                return m;
            }

            static Matrix zeros(int cols, int rows) {
                int* seq_ = new int [cols * rows]{};

                Matrix<T> m(cols, rows, seq_, seq_ + cols * rows);
                delete [] seq_;

                return m;
            }

            template<typename Iterator>
            Matrix(int cls, int rws, Iterator it, Iterator et) : cols_(cls), rows_(rws) {
                if(cols_ <= 0 || rows_ <= 0){
                    std::cout << "Matrix sizes must be positive integers." << std::endl;
                    abort();
                }

                if(std::distance(it, et) != cols_ * rows_){
                    std::cout << "Wrong sequence length." << std::endl;
                    #ifdef DEBUG_
                    std::cout << "Sequence length is " << std::distance(it, et) << std::endl;
                    #endif
                    abort();
                }

                data = new T*[cols_];

                for(int i = 0; i < cols_; ++i){
                    data[i] = new T[rows_];
                    std::copy(it, std::next(it, rows_), data[i]);
                    it = std::next(it, rows_);
                }

                #ifdef DEBUG_
                std::cout << "Matrix iterator ctor" << std::endl;
                #endif
            }

            static Matrix Matrix_add(Matrix<T>& m1, Matrix<T>& m2){
                if(m1.cols_ != m2.cols_ || m1.rows_ != m2.rows_){
                    std::cout << "Matrices are incompatible." << std::endl;
                    abort();
                }

                Matrix<T> m_out(m1.cols_, m1.rows_);

                for(int i = 0; i < m1.cols_; ++i){
                    for(int j = 0; j < m1.rows_; ++j){
                        m_out.data[i][j] = m1.data[i][j] + m2.data[i][j];
                    }
                }

                return m_out;
            }

            void swap_rows(int index1, int index2 ) {
                if(index1 > rows_) {
                    std::cout << "index1 is out of matrix rows range." << std::endl;
                    abort();
                }
                if(index2 > rows_) {
                    std::cout << "index2 is out of matrix rows range." << std::endl;
                    abort();
                }

                std::swap(data[index1], data[index2]);
            }

            T trace() const {
                T trace_{};

                for(int i = 0; i < cols_; ++i) {
                    trace_ += data[i][i];
                }

                return trace_;
            }

            bool equal(const Matrix& other) const {         // to be refactored
                for(int i = 0; i < cols_; ++i){
                    for(int j = 0; j < rows_; ++j){
                        if(data[i][j] != other.data[i][j]) { return false; }
                    }
                }

                return true;
            }

            bool less(const Matrix& other) const {         // to be refactored
                for(int i = 0; i < cols_; ++i){
                    for(int j = 0; j < rows_; ++j){
                        if(data[i][j] >= other.data[i][j]) { return false; }
                    }
                }

                return true;
            }

            Matrix& negate() & { // to be refactored
                for(int i = 0; i < cols_; ++i){
                    for(int j = 0; j < rows_; ++j){
                        data[i][j] = -data[i][j];
                    }
                }

                return *this;
            }

            Matrix& transpose() & { // to be refactored

                for(int i = 0; i < cols_; ++i){
                    for(int j = 0; j <= i; ++j){
                        std::swap(data[i][j], data[j][i]);
                    }
                }

                return *this;
            }

            void swap_columns(int i, int j) {
                transpose();
                swap_rows(i, j);
                transpose();
            }

            void add_rows(int i_, int j_) {     // to be refactored
                for(int j = 0; j < rows_; ++j){
                    data[i_][j] += data[j_][j];
                }
            }

            void add_columns(int i_, int j_) {
                transpose();
                add_rows(i_, j_);
                transpose();
            }

            void multiply_row(int i_, const T& multiplier){
                for(int j = 0; j < rows_; ++j){
                    data[i_][j] *= multiplier;
                }
            }

            void multipliy_column(int j_, const T& multiplier){
                transpose();
                multiply_row(j_, multiplier);
                transpose();
            }

            static Matrix upper_triangular(std::vector<T> vec_) {       // to be refactored
                Matrix<T> m_(vec_.size(), vec_.size(), static_cast<T>(0));

                for(int i = 0; i < m_.cols_; ++i){
                    m_[i][i] = vec_[i];
                }

                for(int i = 0; i < m_.cols_; ++i){
                    for(int j = i + 1; j < m_.rows_; ++j){
                        m_[i][j] = static_cast<T>(1);
                    }
                }

                return m_;
            }

            static Matrix upper_triangular(const T& det_, int cols) {       // to be refactored
                Matrix<T> m_ = eye(cols);

                int rand_pos = std::rand() % cols;
                m_[rand_pos][rand_pos] = det_;

                for(int i = 0; i < m_.cols_; ++i){
                    for(int j = i + 1; j < m_.rows_; ++j){
                        m_[i][j] = static_cast<T>(std::rand() % 10);
                    }
                }

                return m_;
            }

            void shuffle() {        // to be refactored
                std::vector<int> actions(cols_ * 2);
                int row_ind;
                int col_ind = std::rand() % cols_;

                for(auto it = actions.begin(), et = actions.end(); it != et; ++it){
                    *it = std::rand() % rand_actions_count;
                }

                for(auto it = actions.begin(), et = actions.end(); it != et; ++it){
                    row_ind = std::rand() % cols_;
                    col_ind = std::rand() % cols_;
                    if(row_ind == col_ind){ row_ind = (row_ind + 1) % cols_; }
                    switch(*it){
                    case 1:
                        transpose();
                        break;
                    case 2:
                        swap_columns(col_ind, row_ind);
                        swap_rows(row_ind, col_ind);
                        break;
                    case 3:
                        add_rows(col_ind, row_ind);
                        break;
                    case 4:
                        add_columns(col_ind, row_ind);
                        break;
                    }
                }

            }


            T calculate_det() const {       // to be continued
                const double float_tolerance = 1e-10;

                if(cols_ != rows_){
                    std::cout << "Given matrix is not square." << std::endl;
                    abort();
                }
                if(cols_ == 2){
                    return data[0][0] * data[1][1] - data[1][0] * data[0][1];
                }

                lpu_decomposition<double> lpu = lpu_decompose();

                double det = lpu.L.triangle_det() * lpu.U.triangle_det();
                if(lpu.sign) det *= -1.0;

                int det_sign = det > 0 ? 1 : -1;

                std::cout << "pre-final det: " << det << std::endl;
                if(det - static_cast<T>(det) > float_tolerance){
                    det += 1.0 * det_sign;
                }

                return static_cast<T>(det);
            }

            lpu_decomposition<double> lpu_decompose() const {
                int permutations_count = 0;
                const double float_tolerance = 1e-10;

                Matrix<double> U = *this;
                Matrix<double> L = Matrix<double>::eye(cols_);

                for(int i = 0; i < cols_ - 1; ++i){

                    if(std::fabs(U[i][i]) < float_tolerance){

                        std::cout << "U[" << i << "][" << i << "] = " << U[i][i] << std::endl;

                        int trash_finded = 0;
                        // найти ненулевой элемент в столбце, поменять строки местами, докинуть +1 в счётчик
                        // перестановок

                        for(int k = i + 1; k < rows_; ++k){                 // refactor -> std::find_if
                            if(std::fabs(U[k][i]) < float_tolerance){

                                std::cout << "data[" << i << "][" << i << "] == " << data[i][i] << std::endl;
                                std::cout << "before permutating " << i << " <–> " << k << ":\n";
                                U.print();

                                U.swap_rows(i, k);

                                trash_finded = 1;

                                std::cout << "\nafter:\n";
                                U.print();
                                std::cout << " now U[" << i << "][" << i << "] == " << U[i][i] << std::endl;

                                permutations_count++;
                                break;
                            }
                        }

                        if(trash_finded == 0){
                            std::cout << "here\n";

                            return lpu_decomposition<double>{
                                Matrix<double>::zeros(U.cols(), U.rows()), Matrix<double>::zeros(L.cols(), L.rows()), 0
                            };
                        }
                    }

                    for(int j = i + 1; j < rows_; ++j){

                        L[j][i] = U[j][i] / U[i][i];

                        std::cout << "L[" << j << "][" << i << "] = " << L[j][i] << " = " << U[j][i] << " / "
                            << U[i][i] << "\n";

                        for(int k = 0; k < rows_; ++k){
                            U[j][k] -= L[j][i] * U[i][k];
                        }
                        std::cout << std::endl;

                    }

                    std::cout << "Current U:" << std::endl;
                    U.print();

                }

                std::cout << permutations_count << " permutations\n";


                #ifdef DEBUG_
                std::cout << "L:" << std::endl;
                L.print();
                std::cout << "U:" << std::endl;
                U.print();
                #endif

                return lpu_decomposition<double>{L, U, static_cast<bool>(permutations_count % 2)};

            }

            double triangle_det() const {        // to be refactored
                double det = 1.0;

                for(int i = 0; i < cols_; ++i) {
                    det *= data[i][i];
                }

                return det;
            }

            void print() const noexcept {   // to be refactored
                #ifdef DEBUG_
                std::cout << "Matrix output" << std::endl;
                #endif

                for(int i = 0; i < cols_; ++i){
                    for(int j = 0; j < rows_; ++j){
                        std::cout << data[i][j] << " ";
                    }
                    std::cout << std::endl;
                }
            }

            void print_row(int i) const {
                std::cout << "row №" << i << ":" << std::endl;

                std::for_each(data[i], data[i] + rows_, [](int elem){ std::cout << elem << " "; });
                std::cout << std::endl;
            }

            int cols() const noexcept {
                return cols_;
            }

            int rows() const noexcept {
                return rows_;
            }

            operator Matrix<double>() const {                    // many questions... to be refactored
                std::cout << "Implicit user-defined convertion to Matrix<double>" << std::endl;

                Matrix<double> m_ = Matrix<double>::eye(cols_);

                for(int i = 0; i < cols_; ++i){
                    for(int j = 0; j < rows_; ++j){
                        m_[i][j] = static_cast<double>(data[i][j]);
                    }
                }

                return m_;
            }

            ProxyRow operator[](int i){         // RVO here ?..
                ProxyRow row{data[i]};
                return row;
            }

            const ProxyRow operator[](int i) const {
                const ProxyRow row{data[i]};
                return row;
            }

            Matrix(const Matrix& rhs) : cols_(rhs.cols_), rows_(rhs.rows_) {

                #ifdef DEBUG_
                std::cout << "Copy ctor" << std::endl;
                #endif

                data = new T*[cols_];

                for(int i = 0; i < rhs.cols_; ++i){
                    data[i] = new T[rows_];
                    std::copy(rhs.data[i], rhs.data[i] + rows_, data[i]);
                }
            }

            Matrix& operator=(const Matrix& rhs) {
                #ifdef DEBUG_
                std::cout << "Copy assign" << std::endl;
                #endif

                if(this == &rhs) return *this;

                for(int i = 0; i < cols_; ++i){
                    delete [] data[i];
                }
                delete [] data;

                cols_ = rhs.cols_;
                rows_ = rhs.rows_;
                data = new T*[cols_];

                for(int i = 0; i < cols_; ++i){
                    data[i] = new T[rows_];
                    std::copy(rhs.data[i], rhs.data[i] + rows_, data[i]);
                }

                return *this;
            }

            Matrix& operator=(Matrix&& rhs){
                #ifdef DEBUG_
                std::cout << "Move assign" << std::endl;
                #endif

                if(&rhs == this) return *this;

                if(cols_ != rhs.cols_ || rows_ != rhs.rows_) {
                    std::cout << "Matrices are incompatible." << std::endl;
                    abort();
                }

                std::swap(data, rhs.data);

                return *this;
            }

            Matrix(Matrix&& rhs) : cols_(rhs.cols_), rows_(rhs.rows_), data(rhs.data) {
                #ifdef DEBUG_
                std::cout << "Move ctor" << std::endl;
                #endif

                rhs.data = nullptr;
            }

            ~Matrix() {
                #ifdef DEBUG_
                std::cout << "Matrix dtor" << std::endl;
                #endif

                if(data != nullptr){
                    for(int i = 0; i < cols_; ++i){
                        delete [] data[i];
                    }
                    delete [] data;
                }
            }
    };

}
