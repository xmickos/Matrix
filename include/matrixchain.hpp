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
            if(p_.empty()) {
                p_.push_back(rhs.cols());
                p_.push_back(rhs.rows());
            } else {
                p_.push_back(rhs.rows());
            }
        }

        void append(const Matrix<T>& rhs) {
            buff_.push_back(rhs);
            if(p_.empty()) {
                p_.push_back(rhs.cols());
                p_.push_back(rhs.rows());
            } else {
                p_.push_back(rhs.rows());
            }
        }

        void print_content_sizes() const {
            if(buff_.empty()) {
                throw std::out_of_range("MatrixBuffer is empty.");
            }

            const Matrix<T>& back_ = buff_.back();
            std::for_each(buff_.begin(), std::prev(buff_.end()),
                [](auto it){ std::cout << "(" << it.cols() << ", " << it.rows() << "), ";}
            );

            std::cout << "(" << back_.cols() << ", " << back_.rows() << ")" << std::endl;
        }

        void print_content() const {
            if(buff_.empty()) {
                throw std::out_of_range("MatrixBuffer is empty.");
            }

            std::for_each(buff_.begin(), buff_.end(),
                [](auto it){ it.print(); std::cout << std::endl; }
            );
        }

    #if 0
    private:
        void print_optimal_order(const Matrix<int>& s, int i, int j) const {
            if(i == s[i][j] && s[i][j] + 1 == j) {
                std::cout << ((i < j) ? i : j) << " ";
                return;
            }
            if(i == j) {
                std::cout << i << " ";
            } else {
                if(i < s[i][j]) {
                    print_optimal_order(s, i, s[i][j]);
                    print_optimal_order(s, s[i][j] + 1, j);
                } else {
                    print_optimal_order(s, s[i][j] + 1, j);
                    print_optimal_order(s, i, s[i][j]);
                }
            }
        }
    #endif

        void enumerate_optimal_order(Matrix<std::pair<int, int>>& s, int i, int j, int& counter) const {
            if(i == j) {
                return;
            } else {
                enumerate_optimal_order(s, i, s[i][j].first, counter);
                enumerate_optimal_order(s, s[i][j].first + 1, j, counter);
                s[i][j].second = counter;
                counter++;
            }
        }

        void print_optimal_order(const Matrix<std::pair<int, int>>& s, int i, int j) const {
            if(i == j) {
                return;
            } else {
                print_optimal_order(s, i, s[i][j].first);
                if(s[i][j].second != -1) { std::cout << s[i][j].second << " "; }
                print_optimal_order(s, s[i][j].first + 1, j);
            }
        }

    public:

        long long naive_multiply_cost() const {
            if(p_.empty()) {
                throw std::out_of_range("MatrixBuffer is empty");
            }

            return p_[0] * std::inner_product(std::next(p_.begin()), std::prev(p_.end()), std::next(p_.begin(), 2), 0LL);
        }

        long long matrix_chain_order() const {
            Matrix<long long> m = Matrix<long long>::zeros(buff_.size(), buff_.size());
            Matrix<std::pair<int, int>> s = Matrix<std::pair<int, int>>::zeros(buff_.size(), buff_.size());
            int counter = 0;

            int n = p_.size() - 1;
            long long q = 0;
            for(int l = 2; l <= n; ++l) {
                for(int i = 0; i < n - l + 1; ++i) {
                    int j = i + l - 1;
                    m[i][j] = INT_MAX;
                    for(int k = i; k < j; ++k) {
                        q = m[i][k] + m[k + 1][j] + p_[i] * p_[k + 1] * p_[j + 1];
                        if(q < m[i][j]) {
                            m[i][j] = q;
                            s[i][j].first = k;
                        }
                    }
                }
            }
            enumerate_optimal_order(s, 0, n - 1, counter);
            print_optimal_order(s, 0, n - 1);
            std::cout << std::endl;

            return m[0][n - 1];
        }

};

}
