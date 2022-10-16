#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

using namespace cv;
using namespace std;

void silu(Point3_<uint8_t> &pixel) {
    pixel.x /= static_cast<uint8_t>(1 + exp(-pixel.x));
    pixel.y /= static_cast<uint8_t>(1 + exp(-pixel.y));
    pixel.z /= static_cast<uint8_t>(1 + exp(-pixel.z));
}

int main() {
    Mat img = imread("test.jpg"), dst;
    if (img.empty()) {
        cout << "Cannot load image!" << endl;
        return -1;
    }
    float kernel_data[] = {0., -1., 0.,
                           -1., 5., -1.,
                           0., -1., 0.};
    Mat kernel = Mat(3, 3, CV_32FC1, kernel_data);
    filter2D(img, dst, -1, kernel, Point(-1, -1), 0, BORDER_DEFAULT);
    imshow("before", img);
    imshow("after", dst);

    imwrite("result.jpg", dst);

    Mat dst_silu = img.clone();
    dst_silu.forEach<Point3_<uint8_t>>([](Point3_<uint8_t> &pixel, const int *position) -> void { silu(pixel); });
    imshow("silu", dst_silu);
    waitKey(0);
    while (waitKey(33) != 33);
    return 0;
}
