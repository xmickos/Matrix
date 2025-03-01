#include "matrix.hpp"
#include<climits>

namespace matrix{

template <typename T> class MatrixChain final {
    private:
        std::vector<Matrix<T>> buff_;
        std::vector<int> p_;

    public:
        MatrixChain() : buff_(), p_() {}

        MatrixChain(const Matrix<T>& m) : buff_() {
            buff_.push_back(m);
            p_.push_back(m.cols());
            p_.push_back(m.rows());
        }

        MatrixChain(Matrix<T>&& m) : buff_() {
            buff_.push_back(m);
            p_.push_back(m.cols());
            p_.push_back(m.rows());
        }

        void append(Matrix<T>&& rhs) {
            buff_.push_back(rhs);
            if(buff_.empty()) {
                p_.push_back(rhs.cols());
                p_.push_back(rhs.rows());
            } else {
                p_.push_back(rhs.rows());
            }
        }
        void append(const Matrix<T>& rhs) {
            buff_.push_back(rhs);
            if(buff_.empty()) {
                p_.push_back(rhs.cols());
            } else {
                p_.push_back(rhs.rows());
            }
        }

        void print_content_sizes() const {
            const Matrix<T>& back_ = buff_.back();
            std::for_each(buff_.begin(), std::next(buff_.end(), -1),
                [](auto it){ std::cout << "(" << it.cols() << ", " << it.rows() << "), ";}
            );

            std::cout << "(" << back_.cols() << ", " << back_.rows() << ")" << std::endl;
        }

        void print_content() const {
            std::for_each(buff_.begin(), buff_.end(),
                [](auto it){ it.print(); std::cout << std::endl; }
            );
        }

        void print_optimal_order(const Matrix<int>& s, int i, int j) const {
            if(i == j) {
                std::cout << "A_" << i;
            } else {
                std::cout << "(";
                print_optimal_order(s, i, s[i][j]);
                print_optimal_order(s, s[i][j] + 1, j);
                std::cout << ")";
            }
        }

        void matrix_chain_order() const {
            std::cout << buff_.size() << std::endl;
            Matrix<int> m = Matrix<int>::zeros(buff_.size(), buff_.size());
            Matrix<int> s = Matrix<int>::zeros(buff_.size(), buff_.size());

            int n = p_.size() - 1;
            int q = 0;
            for(int l = 2; l < n; ++l) {
                for(int i = 1; i < n - l + 1; ++i) {
                    int j = i + l - 1;
                    m[i][j] = INT_MAX;
                    for(int k = i; i < j - 1; ++k) {
                        q = m[i][k] + m[k + 1][j] + p_[i - 1] * p_[k] * p_[j];
                        if(q < m[i][j]) {
                            m[i][j] = q;
                            s[i][j] = k;
                        }
                    }
                }
            }
            s.print();
            print_optimal_order(s, 1, n);
        }

};


}
