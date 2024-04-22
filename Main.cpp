#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <opencv2/core/types.hpp>
#ifndef OPENCV
#define OPENCV
#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#endif

#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>

#include "Mat_ops.h"
#include "my_fft.h"
#include "my_filters.h"

using namespace cv;

void printmatrix(Matrix *&mat) {
  std::cout.precision(3);
  for (int i = 0; i < mat->rows / 50; i++) {
    for (int j = 0; j < mat->cols / 50; j++) {
      cout << mat->matrix[i * mat->cols + j].real() << " ";
    }
    printf("\n");
  }
  printf("\n\n");
}

void generate_mat(Matrix *&ret) {
  unsigned int rows = 32;
  unsigned int cols = 64;
  ret->rows = rows;
  ret->cols = cols;
  cout << "gonna fail" << endl;
  ret->matrix = new std::complex<double>[rows * cols];
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++) {
      ret->matrix[i * cols + j].real(rand() % 255);
      ret->matrix[i * cols + j].imag(0);
    }
}

int main(int argc, char *argv[]) {
  printf("Hello\n");
  if (argc == 1) {
    printf("Given image path as first argument\n");
    exit(0);
  }
  std::string img_path(argv[2]);
  std::string window_name = "Original image";
  Mat img = imread(img_path, IMREAD_GRAYSCALE);

  if (img.empty()) {
    printf("Couldnt read image\n");
    return 1;
  }

  namedWindow(window_name, WINDOW_NORMAL);
  // imshow(window_name, img);
  // waitKey(0);
  //      for (unsigned int i = 0; i < img.rows; i++) {
  //       for (unsigned int j = 0; j < img.cols; j++) {
  //         printf("%d ", static_cast<int>(img.at<uchar>(i, j)));
  //       }
  //       printf("\n");
  //     }
  Matrix *img_matrix = cv_to_std(img);
  cout << img_matrix->cols << " is new cols" << endl;
  cout << img_matrix->rows << " is new rows" << endl;
  // Matrix *img_matrix = new Matrix;
  // generate_mat(img_matrix);
  // printmatrix(img_matrix);
  // cout << "works till displaying" << endl;
  Matrix *fft_matrix = fft2d(img_matrix, 0);
  // cout << "generated matrix" << endl;
  //////////////////////////////
  double cut_off = atof(argv[1]) / (double)100;
  //  low_pass(fft_matrix, cut_off);
  //////////////////////////////
  // cout << "ep ep teee" << endl;
  // printmatrix(fft_matrix);
  Matrix *new_mat = fft2d(fft_matrix, 1);
  // cout << "printing them results" << endl;
  // printmatrix(new_mat);
  cv::Mat new_cv = std_to_cv(new_mat);
  imshow(window_name, new_cv);
  waitKey(0);
  printf("finished showing image\n");
}
