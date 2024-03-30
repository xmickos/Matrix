#include<vector>
#include<iostream>


template <typename T>       // T = {int, double, float}
class Matrix{
    private:

        T** data = nullptr;
        int cols, rows;

    public:

        Matrix(int cols_, int rows_, T val = T{}) : cols(cols_), rows(rows_) {
            data = new T*[cols];
            for(int i = 0; i < cols; ++i){
                data[i] = new T[rows]{val};
            }
            std::cout << "Matrix ctor" << std::endl;
        }


        void print() const {
            std::cout << "Matrix output" << std::endl;
            for(int i = 0; i < cols; ++i){
                for(int j = 0; j < rows; ++j){
                    std::cout << data[i][j] << " ";
                }
                std::cout << std::endl;
            }
        }



        ~Matrix() {
            for(int i = 0; i < cols; ++i){
                delete [] data[i];
            }
            delete [] data;

            std::cout << "Matrix dtor" << std::endl;
        }
};
