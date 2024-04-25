#include "acc_fft.h"
#include "Mat_ops.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>

using namespace std;

void printmat_in_acc(Matrix *&mat) {
  printf("going to print matrix\n");
  std::cout.precision(3);
  for (int i = 0; i < mat->rows / 50; i++) {
    for (int j = 0; j < mat->cols / 50; j++) {
      cout << mat->matrix[i * mat->cols + j].real() << " ";
    }
    printf("\n");
  }
  printf("\n\n");
}

// #pragma acc routine
// void cooley_turkey(Complex *matrix, unsigned int &n, int invert) {
//
//   for (unsigned int i = 1, j = 0; i < n; i++) {
//     int bit = n >> 1;
//     for (; j & bit; bit >>= 1)
//       j ^= bit;
//     j ^= bit;
//
//     if (i < j)
//       std::swap(matrix[i], matrix[j]);
//   }
//
//   for (unsigned int len = 2; len <= n; len <<= 1) {
//     double ang = 2 * PI / len * (invert ? -1 : 1);
//     Complex wlen(cos(ang), sin(ang));
//     for (unsigned int i = 0; i < n; i += len) {
//       Complex w(1);
//       for (unsigned int j = 0; j < len / 2; j++) {
//         Complex u = matrix[i + j], v = matrix[i + j + len / 2] * w;
//         matrix[i + j] = u + v;
//         matrix[i + j + len / 2] = u - v;
//         w = w * wlen;
//       }
//     }
//   }
//
//   if (invert) {
//     for (unsigned int i = 0; i < n; i++) {
//       matrix[i] = matrix[i] / (Complex)n;
//     }
//   }
// }

// will use standard cooley-turkey algorithm
Matrix *fft2d(Matrix *padded_matrix, const int to_invert) {
  // to parallelise
  unsigned Rows = padded_matrix->rows;
  unsigned Cols = padded_matrix->cols;
  double PI = acos(-1);
  Matrix *transposed_matrix;
#pragma acc data copy(padded_matrix[0 : 1],                                    \
                      padded_matrix->matrix[0 : Rows * Cols])
  {
#pragma acc parallel
    {
#pragma acc loop
      for (unsigned int i = 0; i < Rows; i++) {
        for (unsigned int k = 1, j = 0; k < Cols; k++) {
          int bit = Cols >> 1;
          for (; j & bit; bit >>= 1)
            j ^= bit;
          j ^= bit;

          if (k < j) {
            std::swap(padded_matrix->matrix[i * Cols + k],
                      padded_matrix->matrix[i * Cols + j]);
          }
        }

        for (unsigned int len = 2; len <= Cols; len <<= 1) {
          double ang = 2 * PI / len * (to_invert ? -1 : 1);
          Complex wlen(cos(ang), sin(ang));
          for (unsigned int k = 0; k < Cols; k += len) {
            Complex w(1);
            for (unsigned int j = 0; j < len / 2; j++) {
              Complex u = padded_matrix->matrix[i * Cols + k + j],
                      v = padded_matrix->matrix[i * Cols + k + j + len / 2] * w;
              padded_matrix->matrix[i * Cols + k + j] = u + v;
              padded_matrix->matrix[i * Cols + k + j + len / 2] = u - v;
              w = w * wlen;
            }
          }
        }

        if (to_invert) {
          for (unsigned int k = 0; k < Cols; k++) {
            padded_matrix->matrix[i * Cols + k] =
                padded_matrix->matrix[i * Cols + k] / (Complex)Cols;
          }
        }
      }
    }
  }
  transposed_matrix = transpose(padded_matrix);
  Rows = transposed_matrix->rows;
  Cols = transposed_matrix->cols;
#pragma acc data copyin(PI) copy(transposed_matrix[0 : 1],                     \
                                 transposed_matrix -> matrix[0 : Rows * Cols])
  {
#pragma acc parallel
    {
#pragma acc loop
      for (unsigned int i = 0; i < Rows; i++) {
        for (unsigned int k = 1, j = 0; k < Cols; k++) {
          int bit = Cols >> 1;
          for (; j & bit; bit >>= 1)
            j ^= bit;
          j ^= bit;

          if (k < j)
            std::swap(transposed_matrix->matrix[i * Cols + k],
                      transposed_matrix->matrix[i * Cols + j]);
        }

        for (unsigned int len = 2; len <= Cols; len <<= 1) {
          double ang = 2 * PI / len * (to_invert ? -1 : 1);
          Complex wlen(cos(ang), sin(ang));
          for (unsigned int k = 0; k < Cols; k += len) {
            Complex w(1);
            for (unsigned int j = 0; j < len / 2; j++) {
              Complex u = transposed_matrix->matrix[i * Cols + k + j],
                      v = transposed_matrix
                              ->matrix[i * Cols + k + j + len / 2] *
                          w;
              transposed_matrix->matrix[i * Cols + k + j] = u + v;
              transposed_matrix->matrix[i * Cols + k + j + len / 2] = u - v;
              w = w * wlen;
            }
          }
        }

        if (to_invert) {
          for (unsigned int k = 0; k < Cols; k++) {
            transposed_matrix->matrix[i * Cols + k] =
                transposed_matrix->matrix[i * Cols + k] / (Complex)Cols;
          }
        }
      }
    }
  }
  Matrix *result = transpose(transposed_matrix);
  delete[] transposed_matrix->matrix;
  delete[] transposed_matrix;
  return result;
}
