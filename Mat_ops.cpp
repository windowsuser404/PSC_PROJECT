#include "Mat_ops.h"
#include <algorithm>
#include <cstdio>
#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>

cv::Mat std_to_cv(Matrix *&input_matrix) {
  unsigned rows = input_matrix->rows;
  unsigned cols = input_matrix->cols;
  cv::Mat result(rows, cols, CV_8U);
  for (unsigned int i = 0; i < cols; i++) {
    for (unsigned int j = 0; j < rows; j++) {
      double real_part = (input_matrix->matrix[j * cols + i].real());
      if ((i + j) % 2 == 0)
        result.at<uchar>(j, i) = static_cast<uchar>(real_part);
      else
        result.at<uchar>(j, i) = -1 * static_cast<uchar>(real_part);
    }
  }
  return result.clone();
}

Matrix *zero_padder(cv::Mat &img, int new_rows, int new_cols) {
  Matrix *padded_matrix = new Matrix;
  padded_matrix->matrix = new Complex[new_rows * new_cols];
  padded_matrix->rows = new_rows;
  padded_matrix->cols = new_cols;

  for (int i = 0; i < new_rows; i++) {
    for (int j = 0; j < new_cols; j++) {
      padded_matrix->matrix[i * new_rows + j].real(0);
      padded_matrix->matrix[i * new_rows + j].imag(0);
    }
  }
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      if ((i + j) % 2 == 0) {
        padded_matrix->matrix[i * new_rows + j].real(
            static_cast<double>(img.at<uchar>(i, j)));
        padded_matrix->matrix[i * new_rows + j].imag(0);
      } else {
        padded_matrix->matrix[i * new_rows + j].real(
            -1 * static_cast<double>(img.at<uchar>(i, j)));
        padded_matrix->matrix[i * new_rows + j].imag(0);
      }
    }
  }
  return padded_matrix;
}

Matrix *cv_to_std(cv::Mat &img) {
  int rows = img.rows;
  int cols = img.cols;
  rows = std::max(rows, cols);
  cols = rows;
  int new_rows = 1 << (int)ceil(log2(rows));
  int new_cols = 1 << (int)ceil(log2(cols));
  Matrix *padded_matrix = zero_padder(img, new_rows, new_cols);
  return padded_matrix;
}

Matrix *transpose(Matrix *&org_matrix) {
  Matrix *transposed_matrix = new Matrix;
  unsigned int Rows = org_matrix->rows;
  unsigned int Cols = org_matrix->cols;
  transposed_matrix->matrix = new Complex[Cols * Rows];
  transposed_matrix->rows = Cols;
  transposed_matrix->cols = Rows;

#pragma acc parallel copyin(org_matrix[0 : 1],                                 \
                            org_matrix->matrix[0 : Rows * Cols])               \
    copyout(transposed_matrix[0 : 1],                                          \
            transposed_matrix -> matrix[0 : Rows * Cols])
  {
#pragma acc loop
    for (unsigned int i = 0; i < Rows; i++) {
#pragma acc loop
      for (unsigned int j = 0; j < Cols; j++) {
        transposed_matrix->matrix[i * Cols + j] =
            org_matrix->matrix[j * Rows + i]; // trans[i][j] = mat[j][i]
      }
    }
  }
  return transposed_matrix;
}
