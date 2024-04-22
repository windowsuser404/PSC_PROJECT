#include "my_filters.h"
#include "Mat_ops.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <opencv2/core/mat.hpp>

void low_pass(Matrix *&fft_mat, double &cut_off) {
  int rows = fft_mat->rows;
  int cols = fft_mat->cols;
  double max = (rows * rows / (double)4 + cols * cols / (double)4);
  for (unsigned int i = 0; i < rows; i++) {
    for (unsigned int j = 0; j < cols; j++) {
      double dist = (rows / 2.0 - i) * (rows / 2.0 - i) +
                    (cols / 2.0 - j) * (cols / 2.0 - j);
      dist = dist / max;
      dist = sqrt(dist);
      if (dist > cut_off) {
        fft_mat->matrix[i * cols + j].real(0.0);
        fft_mat->matrix[i * cols + j].imag(0.0);
      }
    }
  }
}

void high_pass(Matrix *&fft_mat, double &cut_off) {
  int rows = fft_mat->rows;
  int cols = fft_mat->cols;
  double max = (rows * rows / (double)4 + cols * cols / (double)4);
  for (unsigned int i = 0; i < rows; i++) {
    for (unsigned int j = 0; j < cols; j++) {
      double dist = (rows / 2.0 - i) * (rows / 2.0 - i) +
                    (cols / 2.0 - j) * (cols / 2.0 - j);
      dist = dist / max;
      dist = sqrt(dist);
      if (dist < cut_off) {
        fft_mat->matrix[i * cols + j].real(0.0);
        fft_mat->matrix[i * cols + j].imag(0.0);
      }
    }
  }
}
