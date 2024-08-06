#include "Matrix.h"

template<typename T>
Matrix<T>::Matrix() = default;

template<typename T>
Matrix<T>::Matrix(unsigned int rows, unsigned int cols, T fill_value) : m(rows), n(cols) {
    alloc();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++)
            buffer[i][j] = fill_value;
    }
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T> &other) : m(other.m), n(other.n) {
    alloc();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            buffer[i][j] = other.buffer[i][j];
        }
    }
}


template<typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &v) {
    if (v.empty() || v[0].empty()) {
        m = 0, n = 0;
        buffer = nullptr;
        return;
    }

    m = v.size(), n = v[0].size();
    alloc();
    for (unsigned int i = 0; i < m; ++i) {
        if (v[i].size() != n) {
            throw std::invalid_argument("All rows must have the same size");
        }
        for (unsigned int j = 0; j < n; ++j) {
            buffer[i][j] = v[i][j];
        }
    }
}


template<typename T>
Matrix<T>::~Matrix() {
    for (int i = 0; i < m; i++)
        delete[] buffer[i];
    delete[] buffer;
}


template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> &other) {
    if (this != &other) {
        for (int i = 0; i < m; i++)  // dealloc current memory
            delete[] buffer[i];
        delete[] buffer;

        m = other.m, n = other.n;
        alloc();
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                buffer[i][j] = other.buffer[i][j];
            }
        }
    }
    return *this;
}


template<typename T>
bool Matrix<T>::operator==(const Matrix<T> &other) const {
    if (this->m != other.m || this->n != other.n) {
        return false;
    }

    for (unsigned int i = 0; i < this->m; i++) {
        for (unsigned int j = 0; j < this->n; j++) {
            if (this->buffer[i][j] != other.buffer[i][j]) {
                return false;
            }
        }
    }
    return true;
}


template<typename T>
T& Matrix<T>::operator()(int x, int y) const {
    return buffer[x][y];
}


template<typename T>
std::tuple<unsigned int, unsigned int> Matrix<T>::shape() {
    return std::make_tuple(m, n);
}


template<typename T>
std::vector<T> Matrix<T>::flatten() {
    static_assert(!std::is_same<T, Matrix<uint8_t>>::value, "Flatten not supported for Matrix<Matrix<uint8_t>>.");
    auto [h, w] = shape();
    std::vector<T> v(h * w, 0);
    for (int i = 0; i < h; ++i){
        for (int j = 0; j < w; ++j)
            v[i * w +j] = buffer[i][j];
    }
    return v;
}


template<typename T>
bool Matrix<T>::operator!=(const Matrix<T> &other) const {
    return *this != other;
}


template<>
std::vector<Matrix<uint8_t>> Matrix<Matrix<uint8_t>>::flatten() {
    throw std::logic_error("Flatten not supported for Matrix<Matrix<uint8_t>>.");
}


template<typename T>
void Matrix<T>::alloc() {
    buffer = new T*[m];
    for (int i = 0; i < m; i++)
        buffer[i] = new T[n];
}

template class Matrix<uint8_t>;
template class Matrix<int>;
template class Matrix<float>;
template class Matrix<Matrix<uint8_t>>;
