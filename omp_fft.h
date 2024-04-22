#ifndef MY_FFT
#define MY_FFT
#include <omp.h>

#include <opencv2/core/base.hpp>
#ifndef OPENCV
#define OPENCV
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#endif

#include <complex.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>

#include "Mat_ops.h"

void test_print();
void center_ffts(Matrix *&fft_mat);
Matrix *fft2d(Matrix *&padded_matrix, const int &to_invert);

#endif
