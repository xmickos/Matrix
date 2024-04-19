#include<vector>
#include<iostream>
#include<cstdlib>
#include<algorithm>

namespace matrix {

    template <typename T>       // T = {int, double, float}
    class Matrix{
        private:
            T** data = nullptr;
            int cols_, rows_;

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



            template <typename Iterator>
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

            static Matrix eye(int sz_) {
                if(sz_ <= 0){
                    std::cout << "Matrix size must be positive integer." << std::endl;
                    abort();
                }

                int* seq_ = new int[sz_ * sz_]{};

                for(int i = 0; i < sz_; ++i){
                    seq_[ i * (sz_ + 1) ] = (T)1.;
                }

                Matrix<T> m(sz_, sz_, seq_, seq_ + sz_ * sz_);
                delete[] seq_;

                return m;
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


            void print() const noexcept {
                #ifdef DEBUG_
                std::cout << "Matrix output" << std::endl;
                #endif

                for(int i = 0; i < cols_; ++i){
                    for(int j = 0; j < rows_; ++j){
                        std::cout << (data[i])[j] << " ";
                    }
                    std::cout << std::endl;
                }
            }

            int cols() const noexcept {
                return cols_;
            }

            int rows() const noexcept {
                return rows_;
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
