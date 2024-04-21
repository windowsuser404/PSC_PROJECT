#include "Mat_ops.h"
#include "my_fft.h"
#include <opencv2/core/mat.hpp>

cv::Mat img_rotate(Matrix *&fft_mat) {
  Matrix *temp, *result;
  temp = transpose(fft_mat);
  result = fft2d(temp, 1);
  cv::Mat rotated_img = std_to_cv(result);
  return rotated_img.clone();
}
