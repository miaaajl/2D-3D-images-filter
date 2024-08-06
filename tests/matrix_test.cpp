#include "matrix_test.h"
#include <iostream>
#include <vector>


template<typename T>
bool check_shape(Matrix<T>& matrix, unsigned int expected_rows, unsigned int expected_cols) {
    auto [rows, cols] = matrix.shape();
    return rows == expected_rows && cols == expected_cols;
}


template<typename T>
bool test_parameterised_constructor() {
    unsigned int rows = 2, cols = 3;
    T fillValue = static_cast<T>(42);
    Matrix<T> matrix(rows, cols, fillValue);
    bool shapeCheck = check_shape(matrix, rows, cols);
    bool fillCheck = true;
    for (unsigned int i = 0; i < rows; ++i) {
        for (unsigned int j = 0; j < cols; ++j) {
            if (matrix(i, j) != fillValue) {
                fillCheck = false;
                break;
            }
        }
    }
    return shapeCheck && fillCheck;
}

template<typename T>
bool test_copy_constructor() {
    Matrix<T> original(2, 2, static_cast<T>(7));
    Matrix<T> copy = original;
    return copy == original;
}


bool test_vector_constructor() {
    std::vector<std::vector<int>> vec = {{1, 2}, {3, 4}};
    Matrix<int> matrix(vec);
    return matrix(0, 0) == 1 && matrix(1, 1) == 4;
}

template<typename T>
bool test_assignment_operator() {
    Matrix<T> original(2, 2, static_cast<T>(5));
    Matrix<T> assigned;
    assigned = original;
    return assigned == original;
}

bool test_equality_operator() {
    Matrix<int> mat1(2, 2, 7);
    Matrix<int> mat2(2, 2, 7);
    Matrix<int> mat3(2, 2, 8);
    bool test1 = mat1 == mat2;
    bool test2 = mat1 == mat3;
    return test1 && !test2;
}


bool test_shape_function() {
    Matrix<float> matrix(4, 5, 1.1f);
    return check_shape(matrix, 4, 5);
}

bool run_matrix_tests() {
    std::cerr << "Starting unit tests for Matrix class...\n";
    bool result = true;

    if (!test_parameterised_constructor<int>()) {
        std::cerr << "Parameterised constructor test (int) failed.\n";
        return false;
    }
    if (!test_parameterised_constructor<float>()) {
        std::cerr << "Parameterised constructor test (float) failed.\n";
        return false;
    }
    if (!test_copy_constructor<int>()) {
        std::cerr << "Copy constructor test failed.\n";
        return false;
    }
    if (!test_vector_constructor()) {
        std::cerr << "Vector constructor test failed.\n";
        return false;
    }
    if (!test_assignment_operator<float>()) {
        std::cerr << "Assignment operator test (float) failed.\n";
        return false;
    }
    if (!test_equality_operator()) {
        std::cerr << "Equality operator test failed.\n";
        return false;
    }
    if (!test_shape_function()) {
        std::cerr << "Shape function test failed.\n";
        return false;
    }
    return result;
}
