#include "my_filters.h"
#include "Mat_ops.h"
#include "acc_fft.h"
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
      dist = sqrt(dist) * 100;
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
      dist = sqrt(dist) * 100.0;
      if (dist < cut_off) {
        cout << dist << " =dist" << endl;
        fft_mat->matrix[i * cols + j].real(0.0);
        fft_mat->matrix[i * cols + j].imag(0.0);
      }
    }
  }
}

void gauss(Matrix *&fft_mat, int size, double sigma) {
  Matrix *kernel = new Matrix;
  kernel->rows = size;
  kernel->cols = size;
  kernel->matrix = new Complex[size * size];

  // Calculate the center of the kernel
  int center = size / 2;

  // Pre-compute constant values
  double sigmaSquared = sigma * sigma;
  double twoSigmaSquared = 2 * sigmaSquared;
  double constant = 1 / (twoSigmaSquared * M_PI);
  // Generate the kernel values
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      int x = i - center;
      int y = j - center;
      kernel->matrix[i * size + j].real(
          constant * exp(-(x * x + y * y) / twoSigmaSquared));
      kernel->matrix[i * size + j].imag(0);
      cout << (x * x + y * y) / twoSigmaSquared << endl;
    }
  }
  double sum = 0.0;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      sum += kernel->matrix[i * size + j].real();
    }
  }
  cout << sum << " = sum" << endl;
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      kernel->matrix[i * size + j] /= sum;
      // cout << kernel->matrix[i * size + j] << endl;
    }
  }
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      if ((i + j) % 2 == 0) {
        continue;
      } else {
        kernel->matrix[i * size + j].real(kernel->matrix[i * size + j].real());
      }
    }
  }

  // Matrix *temp = fft2d(kernel, 0);
  // delete[] kernel->matrix;
  // delete kernel;
  // kernel = temp;
  Complex neg1(1.0, 0);
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      // cout << fft_mat->matrix[i * size + j] << " "
      //     << kernel->matrix[i * size + j] << endl;
      if (i + j % 2 == 0) {
        fft_mat->matrix[i * size + j] =
            fft_mat->matrix[i * size + j] * kernel->matrix[i * size + j];
      } else {
        fft_mat->matrix[i * size + j] =
            kernel->matrix[i * size + j] * kernel->matrix[i * size + j];
      }
      // cout << fft_mat->matrix[i * size + j] << endl;
    }
  }
  delete[] kernel->matrix;
  delete kernel;
}
