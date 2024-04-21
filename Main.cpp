#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
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
  for (int i = 0; i < mat->rows; i++) {
    for (int j = 0; j < mat->cols; j++) {
      printf("%0.2f ", mat->matrix[i * mat->cols + j].real());
    }
    printf("\n");
  }
  printf("\n\n");
}

int main(int argc, char *argv[]) {
  printf("Hello\n");
  if (argc == 1) {
    printf("Given image path as first argument\n");
    exit(0);
  }
  std::string img_path(argv[1]);
  std::string window_name = "Original image";
  Mat img = imread(img_path, IMREAD_GRAYSCALE);
  printf("rows=%d, cols=%d\n", img.rows, img.cols);

  if (img.empty()) {
    printf("Couldnt read image\n");
    return 1;
  }

  namedWindow(window_name, WINDOW_NORMAL);
  // imshow(window_name, img);
  // waitKey(0);
  //   for (unsigned int i = 0; i < img.rows; i++) {
  //    for (unsigned int j = 0; j < img.cols; j++) {
  //      printf("%d ", static_cast<int>(img.at<uchar>(i, j)));
  //    }
  //    printf("\n");
  //  }
  printf("\n\n\n");
  Matrix *img_matrix = cv_to_std(img);
  Matrix *fft_matrix = fft2d(img_matrix, 0);
  //////////////////////////////
  Matrix *turnfft = transpose(fft_matrix);
  //////////////////////////////
  Matrix *turn = fft2d(turnfft, 1);
  cv::Mat rotated = std_to_cv(turn);
  imshow(window_name, rotated);
  waitKey(0);
  printf("finished showing image\n");
}
