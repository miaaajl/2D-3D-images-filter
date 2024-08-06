#ifndef MATRIX_TEST_H
#define MATRIX_TEST_H

#include "Matrix.h"

template<typename T>
bool check_shape(Matrix<T>& matrix, unsigned int expected_rows, unsigned int expected_cols);

template<typename T>
bool test_parameterised_constructor();

template<typename T>
bool test_copy_constructor();

bool test_vector_constructor();

template<typename T>
bool test_assignment_operator();

bool test_equality_operator();

bool test_shape_function();

bool run_matrix_tests();

#endif //MATRIX_TEST_H
