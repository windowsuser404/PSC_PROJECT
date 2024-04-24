#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <openacc.h>
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
#include "acc_fft.h"
#include "my_filters.h"

using namespace cv;

int main(int argc, char *argv[]) {
  acc_device_t dev_type = acc_get_device_type();
  if (dev_type == acc_device_none) {
    printf("No accelerator device is being used.\n");
  } else if (dev_type == acc_device_host) {
    printf("The CPU is being used as the accelerator.\n");
  } else if (dev_type == acc_device_not_host) {
    printf("An accelerator device other than the CPU is being used.\n");
  } else if (dev_type == acc_device_nvidia) {
    printf("An NVIDIA GPU is being used as the accelerator.\n");
  } else {
    printf("Unknown accelerator device is being used.\n");
  }

  printf("Hello\n");
  if (argc == 1) {
    printf("Given image path as first argument\n");
    exit(0);
  }
  std::string img_path(argv[1]);
  std::string window_name = "Original image";
  Mat img = imread(img_path, IMREAD_GRAYSCALE);

  if (img.empty()) {
    printf("Couldnt read image\n");
    return 1;
  }

  namedWindow(window_name, WINDOW_NORMAL);
  Matrix *img_matrix = cv_to_std(img);
  cout << img_matrix->cols << " is new cols" << endl;
  cout << img_matrix->rows << " is new rows" << endl;
  Matrix *fft_matrix = fft2d(img_matrix, 0);
  // gauss(fft_matrix, fft_matrix->rows, 0.05);
  Matrix *new_mat = fft2d(fft_matrix, 1);
  cv::Mat new_cv = std_to_cv(new_mat);
  imshow(window_name, new_cv);
  waitKey(0);
  printf("finished showing image\n");
}
