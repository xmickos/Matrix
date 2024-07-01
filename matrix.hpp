#include<vector>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include<cstdlib>
#include<execution>
#include<climits>
#include<cstdlib>
#include<cmath>
#include<numeric>

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

            static Matrix zeros_like(const Matrix<T>& m){
                return zeros(m.cols_, m.rows_);
            }

            static Matrix Matrix_add(Matrix<T>& m1, Matrix<T>& m2){
                if(m1.cols_ != m2.cols_ || m1.rows_ != m2.rows_){
                    std::cout << "Matrices are incompatible." << std::endl;
                    abort();
                }

                Matrix<T> m_out(m1.cols_, m1.rows_);

                for(int i = 0; i < m1.cols_; ++i){
                    std::transform(
                        m1.data[i], m1.data[i] + m1.rows_, m2.data[i],
                        m_out.data[i], [](const T& a, const T& b){ return a + b; }
                    );
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

            bool equal(const Matrix& other) const {
                for(int i = 0; i < cols_; ++i){
                    if(!std::equal(data[i], data[i] + cols_, other.data[i])){ return false; }
                }

                return true;
            }

            bool less(const Matrix& other) const {
                auto f = [&](const T& a, const T& b){ return a < b; };

                for(int i = 0; i < cols_; ++i){
                    if(!std::equal(data[i], data[i] + cols_, other.data[i], f)){ return false; }
                }

                return true;
            }

            Matrix& negate() & {
                for(int i = 0; i < cols_; ++i){
                    std::transform(data[i], data[i] + cols_, data[i], [](const T& x){ return -x; });
                }

                return *this;
            }

            Matrix& transpose() & {

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

            void add_rows(int i, int j, const T& coefficient = static_cast<T>(1)) {
                auto f = [&](const T& x, const T& y){ return x + y * coefficient; };
                std::transform(data[i], data[i] + rows_, data[j], data[i], f);
            }

            void add_columns(int i, int j, const T& coefficient) {
                transpose();
                add_rows(i, j, coefficient);
                transpose();
            }

            void multiply_row(int i, const T& multiplier) {
                auto f = [&](const T& arg){ return arg * multiplier; };
                std::transform(data[i], data[i] + rows_, data[i], f);
            }

            void multiply_column(int j, const T& multiplier) {
                transpose();
                multiply_row(j, multiplier);
                transpose();
            }

            static Matrix multiply(const Matrix& A, const Matrix& B) {
                // Multiplies A * B and stores result in B
                // A [M x N] * B [N x K]

                Matrix<T> C = zeros_like(A);

                int M = A.cols_, N = A.rows_, K = B.rows_;

                #ifdef TRIVIAL_MATMUL
                for(int i = 0; i < M; i++){
                    for(int j = 0; j < K; j++){
                        for(int k = 0; k < N; k++){
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
                #endif

                #ifdef REVERSED_TRIVIAL_MATMUL
                for(int i = 0; i < M; i++){
                    for(int k = 0; k < N; k++){
                        for(int j = 0; j < K; j++){
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
                #endif

                #ifdef TRANSF_MATMUL
                std::vector<T> fixed_col(K);

                for(int i = 0; i < B.cols_; ++i){
                    for(int q = 0; q < B.rows_; ++q){
                        fixed_col[q] = B[q][i];
                    }
                    for(int j = 0; j < A.rows_; ++j){
                        C[j][i] = std::transform_reduce(A[j].row, A[j].row + A.cols_, fixed_col.begin(), static_cast<T>(0));
                    }
                }
                #endif

                #if 0
                #ifdef DEBUG_
                std::cout << "print\nA:\n";
                A.print();
                std::cout << "\nB:\n";
                B.print();
                std::cout << "C:\n";
                C.print();
                #endif
                #endif

                return C;
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
                    std::generate(m_[i].row + i + 1, m_[i].row + m_.rows_, [](){ return static_cast<T>(std::rand() % 10); });
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
                        add_rows(col_ind, row_ind, 1);
                        break;
                    case 4:
                        add_columns(col_ind, row_ind, 1);
                        break;
                    }
                }
            }

            Matrix<int> get_mu(const Matrix& A) const {
                Matrix<int> mu = zeros(A.rows_, A.cols_);

                for(int i = 0; i < A.cols_; i++){
                    std::copy(A[i].row, A[i].row + i + 1, mu[i].row);
                }

                for(int i = 0; i < A.cols_; i++){
                    for(int j = i + 1; j < A.rows_; j++){
                        mu[i][i] -= A[j][j];
                    }
                }
                mu[cols_ - 1][cols_ - 1] = 0;

                return mu;
            }

            T calculate_det() const {       // to be continued
                const long double float_tolerance = 1e-15;

                #ifdef DEBUG_
                    std::cout << "General algorithm" << std::endl;
                #endif

                if(cols_ != rows_){
                    std::cout << "Given matrix is not square." << std::endl;
                    abort();
                }

                if(cols_ == 2){
                    return data[0][0] * data[1][1] - data[1][0] * data[0][1];
                }

                lpu_decomposition<long double> lpu = lpu_decompose();

                long double det = lpu.L.triangle_det() * lpu.U.triangle_det();

                if(lpu.sign) det *= -1.0;

                int det_sign = det > 0 ? 1 : -1;

                if(det - static_cast<T>(det) > 0.5){
                    det += 1.0 * det_sign;
                }

                return static_cast<T>(det);
            }

            lpu_decomposition<long double> lpu_decompose() const {
                int permutations_count = 0;
                const long double float_tolerance = 1e-20;

                Matrix<long double> U = *this;
                Matrix<long double> L = Matrix<long double>::eye(cols_);

                for(int i = 0; i < cols_ - 1; ++i){

                    if(std::fabs(U[i][i]) < float_tolerance){

                        int trash_finded = 0;
                        // найти ненулевой элемент в столбце, поменять строки местами, докинуть +1 в счётчик
                        // перестановок

                        for(int k = i + 1; k < rows_; ++k){                 // refactor -> std::find_if
                            if(std::fabs(U[k][i]) > float_tolerance){
                                U.swap_rows(i, k);

                                trash_finded = 1;
                                permutations_count++;
                                break;
                            }
                        }



                        if(trash_finded == 0){
                            std::cout << "det is 0.\n";

                            return lpu_decomposition<long double>{
                                Matrix<long double>::zeros(U.cols(), U.rows()),
                                Matrix<long double>::zeros(L.cols(), L.rows()),
                                0
                            };
                        }
                    }

                    for(int j = i + 1; j < rows_; ++j){
                        L[j][i] = U[j][i] / U[i][i];
                        U.add_rows(j, i, -L[j][i]);
                    }
                }

                #if 0
                std::cout << permutations_count << " permutations\n";

                #ifdef DEBUG_
                std::cout << "Final L:" << std::endl;
                L.python_print();
                std::cout << "Final U:" << std::endl;
                U.python_print();
                #endif
                #endif

                return lpu_decomposition<long double>{L, U, static_cast<bool>(permutations_count % 2)};

            }

            T triangle_det() const {
                T det = 1.0;

                for(int i = 0; i < cols_; ++i) {
                    det *= data[i][i];
                }

                return det;
            }

            void print() const noexcept {
                #ifdef DEBUG_
                std::cout << "Matrix output" << std::endl;
                #endif

                for(int i = 0; i < cols_; ++i){
                    for(int j = 0; j < rows_; ++j){
                        #ifndef PYTHON_DEBUG
                        if(i == j) std::cout << '[';
                        #endif

                        std::cout << data[i][j] << " ";

                        #ifndef PYTHON_DEBUG
                        if(i == j) std::cout << ']';
                        #endif
                    }
                    std::cout << std::endl;
                }
            }

            void python_print() const noexcept {
                #ifdef DEBUG_
                std::cout << "Matrix output" << std::endl;
                #endif

                std::cout << "[";
                for(int i = 0; i < cols_ - 1; ++i){
                    std::cout << "[";

                    for(int j = 0; j < rows_ - 1; ++j){
                        #ifndef PYTHON_DEBUG
                        if(i == j) std::cout << '[';
                        #endif

                        std::cout << data[i][j];

                        #ifndef PYTHON_DEBUG
                        if(i == j) std::cout << ']';
                        #endif

                        std::cout << ", ";
                    }
                    std::cout << data[i][rows_ - 1] << "]," << std::endl;
                }

                std::cout << "[";

                for(int j = 0; j < rows_ - 1; ++j){
                    std::cout << data[cols_ - 1][j] << ", ";
                }

                std::cout << data[cols_ - 1][rows_ - 1] << "] ]" << std::endl;
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

            operator Matrix<long double>() const {
                #ifdef DEBUG_
                std::cout << "Implicit user-defined convertion to Matrix<double>" << std::endl;
                #endif

                Matrix<long double> m_ = Matrix<long double>::eye(cols_);

                for(int i = 0; i < cols_; ++i){
                    for(int j = 0; j < rows_; ++j){
                        m_[i][j] = static_cast<long double>(data[i][j]);
                    }
                }

                return m_;
            }

            ProxyRow operator[](int i){
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

template <> int matrix::Matrix<int>::calculate_det() const {
    #ifdef DEBUG_
        std::cout << "Specialized for int's algorithm" << std::endl;
    #endif

    matrix::Matrix<int> A = *this;
    matrix::Matrix<int> Fn = A;

    for(int n = 1; n < rows_; n++){
        const matrix::Matrix<int>& mu = get_mu(Fn);
        Fn = multiply(mu, A);
    }

    return Fn[0][0];
}
