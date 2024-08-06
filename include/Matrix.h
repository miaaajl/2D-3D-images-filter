#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <vector>
#include <tuple>
#include <cstdint>

/**
 * A template class for a 2D matrix that can store elements of any type.
 * It provides basic functionality such as initialization, copy, comparison, and output,
 * along with access to individual elements and matrix properties.
 *
 * @tparam T The data type of the elements stored in the matrix.
 */
template<typename T>
class Matrix {
public:
    /**
     * Default constructor. Initializes an empty Matrix object.
     */
    Matrix();

    /**
     * Constructor to initialize a matrix of given dimensions with a fill value.
     *
     * @param rows Number of rows in the matrix.
     * @param cols Number of columns in the matrix.
     * @param fill_value The value to fill the matrix with. Defaults to 0.
     */
    Matrix(unsigned int rows, unsigned int cols, T fill_value = 0);

    /**
     * Copy constructor. Initializes a Matrix object as a copy of an existing matrix.
     *
     * @param other The Matrix object to copy.
     */
    Matrix(const Matrix<T>& other);

    /**
     * Constructs a Matrix object from a 2D vector.
     *
     * @param v A 2D vector of type T from which to initialize the matrix.
     */
    explicit Matrix(const std::vector<std::vector<T>> &v);

    /**
     * Destructor. Cleans up resources allocated for the matrix.
     */
    ~Matrix();

    /**
     * Copy assignment operator. Assigns the current matrix the same values as another matrix.
     *
     * @param other The Matrix object to assign from.
     * @return Reference to the current matrix after assignment.
     */
    Matrix<T>& operator=(const Matrix<T> &other);

    /**
     * Accesses an element of the matrix.
     *
     * @param x Row index of the element.
     * @param y Column index of the element.
     * @return Reference to the element at the specified indices.
     */
    T& operator()(int x, int y) const;

    /**
     * Compares two matrices for equality.
     *
     * @param other The matrix to compare with.
     * @return True if the matrices are equal, false otherwise.
     */
    bool operator==(const Matrix<T> &other) const;

    /**
     * Compares two matrices for inequality.
     *
     * @param other The matrix to compare with.
     * @return True if the matrices are not equal, false otherwise.
     */
    bool operator!=(const Matrix<T> &other) const;

    /**
     * Overloads the << operator to print the matrix to an output stream.
     *
     * @param os The output stream.
     * @param matrix The matrix to output.
     * @return The output stream.
     */
    inline friend std::ostream& operator<<(std::ostream& os, const Matrix<T> &matrix) {
        for (int i = 0; i < matrix.m; i++) {
            for (int j = 0; j < matrix.n; j++) {
                os << matrix.buffer[i][j];
                if (j != matrix.n - 1) os << ", ";
            }
            if (i != matrix.m - 1) os << "\n";
        }
        return os;
    }


    /**
     * Returns the dimensions of the matrix as a tuple of rows and columns.
     *
     * @return A tuple where the first element is the number of rows and the second is the number of columns.
     */
    std::tuple<unsigned int, unsigned int> shape();


    /**
     * Flattens the matrix into a 1D vector.
     *
     * @return A vector containing all elements of the matrix in row-major order.
     */
    std::vector<T> flatten();

private:
    unsigned int m{0}, n{0}; // Dimensions of the matrix: m rows and n columns.
    /**
     * Allocates memory for the matrix based on its dimensions.
     */
    T** buffer{};
    void alloc();
};

#endif //MATRIX_H

