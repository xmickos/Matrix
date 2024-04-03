#include<vector>
#include<iostream>
#include<cstdlib>
#include<algorithm>

template <typename T>       // T = {int, double, float}
class Matrix{
    private:

        T** data = nullptr;
        int cols, rows;

    public:

        Matrix(int cols_, int rows_, T val = T{}) : cols(cols_), rows(rows_) {
            data = new T*[cols];
            for(int i = 0; i < cols; ++i){
                data[i] = new T[rows];
                std::fill_n(data[i], rows, val);
            }

            #ifdef DEBUG_
            std::cout << "Matrix ctor" << std::endl;
            #endif
        }

        template <typename Iterator>
        Matrix(int cols_, int rows_, Iterator it, Iterator et) : cols(cols_), rows(rows_) {
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

            data = new T*[cols];

            for(int i = 0; i < cols; ++i){
                data[i] = new T[rows];
                std::copy(it, std::next(it, rows), data[i]);
                it = std::next(it, rows);
            }

            #ifdef DEBUG_
            std::cout << "Matrix ctor" << std::endl;
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


        void print() const {
            #ifdef DEBUG_
            std::cout << "Matrix output" << std::endl;
            #endif

            for(int i = 0; i < cols; ++i){
                for(int j = 0; j < rows; ++j){
                    std::cout << (data[i])[j] << " ";
                }
                std::cout << std::endl;
            }
        }



        ~Matrix() {
            for(int i = 0; i < cols; ++i){
                delete [] data[i];
            }
            delete [] data;

            #ifdef DEBUG_
            std::cout << "Matrix dtor" << std::endl;
            #endif
        }
};
