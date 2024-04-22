#include "my_fft.h"
#include "Mat_ops.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>

using namespace std;

const float PI = acos(-1);

void test_print() { printf("Hello from fft.h"); }

void cooley_turkey(Complex *matrix, unsigned int &n, int invert) {

  for (unsigned int i = 1, j = 0; i < n; i++) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1)
      j ^= bit;
    j ^= bit;

    if (i < j)
      std::swap(matrix[i], matrix[j]);
  }

  for (unsigned int len = 2; len <= n; len <<= 1) {
    double ang = 2 * PI / len * (invert ? -1 : 1);
    Complex wlen(cos(ang), sin(ang));
    for (unsigned int i = 0; i < n; i += len) {
      Complex w(1);
      for (unsigned int j = 0; j < len / 2; j++) {
        Complex u = matrix[i + j], v = matrix[i + j + len / 2] * w;
        matrix[i + j] = u + v;
        matrix[i + j + len / 2] = u - v;
        w = w * wlen;
      }
    }
  }

  if (invert) {
    for (unsigned int i = 0; i < n; i++) {
      matrix[i] = matrix[i] / (Complex)n;
    }
  }
}

void center_ffts(Matrix *&fft_mat) {
  int Rows = fft_mat->rows;
  int Cols = fft_mat->cols;
  int cx = Cols / 2;
  int cy = Rows / 2;
  for (unsigned int i = 0; i < cx; i++) {
    for (unsigned int j = 0; j < cy; j++) {
      // Calculate indices for quadrant swapping
      int index_q0 = j * Cols + i;
      int index_q1 = j * Cols + (i + cx);
      int index_q2 = (j + cy) * Cols + i;
      int index_q3 = (j + cy) * Cols + (i + cx);

      // Swap quadrants (q0 with q3, q1 with q2)
      std::swap(fft_mat->matrix[index_q0], fft_mat->matrix[index_q3]);
      std::swap(fft_mat->matrix[index_q1], fft_mat->matrix[index_q2]);
    }
  }
}

// will use standard cooley-turkey algorithm
Matrix *fft2d(Matrix *&padded_matrix, const int &to_invert) {
  // to parallelise
  unsigned Rows = padded_matrix->rows;
  unsigned Cols = padded_matrix->cols;

  for (unsigned int i = 0; i < Rows; i++) {
    cooley_turkey(&(padded_matrix->matrix[i * Cols]), Cols, to_invert);
  }

  Matrix *transposed_matrix = transpose(padded_matrix);
  Rows = transposed_matrix->rows;
  Cols = transposed_matrix->cols;

  cout << Rows << " " << Cols << endl;

  for (unsigned int i = 0; i < Rows; i++) {
    cooley_turkey(&(transposed_matrix->matrix[i * Cols]), Cols, to_invert);
  }
  Matrix *result = transpose(transposed_matrix);
  delete[] transposed_matrix->matrix;
  delete[] transposed_matrix;
  return result;
}
