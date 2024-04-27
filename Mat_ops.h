#ifndef MATOPS_H
#define MATOPS_H
#include <iostream>
#include <openacc.h>
using namespace std;

#ifndef OPENCV
#define OPENCV
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#endif

#define TYPES_H
typedef std::complex<double> Complex;

typedef struct Matrix {
  Complex *matrix;
  unsigned int rows;
  unsigned int cols;
} Matrix;

Matrix *cv_to_std(cv::Mat &img);

cv::Mat std_to_cv(Matrix *&input_matrix);

Matrix *transpose(Matrix *org_matrix);

#endif
