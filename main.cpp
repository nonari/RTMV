#include <iostream>
#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <math.h>

#define PI 3.1415926535

using namespace cv;
using namespace std;

pair<Point2d, Point2d> corners(Mat& im, vector<Point2d> lower) {
    const int middle_h = im.size().height / 2;
    const int middle_w = im.size().width / 2;

    int blockSize = 4;
    int apertureSize = 3;
    double k = 0.1;
    Mat dst = Mat::zeros(im.size(), CV_32FC1);
    Mat canvas = Mat::zeros(im.size(), CV_32FC3);
    cornerHarris(im, dst, blockSize, apertureSize, k);
    Mat dst_norm, dst_norm_scaled;
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);
    vector<Point2d> corners;
    for (int y = 0; y < dst_norm.rows; y++) {
        for (int x = 0; x < dst_norm.cols; x++) {
            if ((int) dst_norm.at<float>(y, x) > 200) {
                cout << "x: " << x << ", y: " << y << endl;
                if (y < middle_h) {
                    corners.push_back(Point(x, y));
                } else {
                    lower.push_back(Point(x, y));
                }
                circle(canvas, Point(x,y), 5,  Scalar(255, 0,0), 2, 8, 0);
            }
        }
    }

    vector<Point2d> right;
    vector<Point2d> left;
    for (auto &point : corners) {
        if (point.x > middle_w) {
            right.push_back(point);
        } else {
            left.push_back(point);
        }
    }
    cout << "Corners: " << left[0] << ", " << right[0] << endl;
//    namedWindow("corners" );
//    imshow( "corners", canvas );
    return {left[0], right[0]};
}

float rotation(pair<Point2d, Point2d> &points) {
    auto x_diff = points.second.x - points.first.x;
    auto y_diff = points.second.y - points.first.y;
    auto angle = atan(y_diff / x_diff) * 180 / PI;
    std::cout << "Angle: " << angle << std::endl;
    return angle;
}


pair<Point2d, Point2d> corners_b(pair<Point2d, Point2d> top, vector<Point2d> corners) {

}


int main() {
    std::cout << "Hello, World!" << std::endl;
    auto im1 = cv::imread("/home/nonari/frames/frame106.jpg");

    Mat gray;
    cvtColor(im1, gray, COLOR_BGR2GRAY);

    vector<Point2d> lower_corners;
    auto top_corners = corners(gray, lower_corners);
    auto angle = rotation(top_corners);
    const int middle_h = im1.size().height / 2;
    const int middle_w = im1.size().width / 2;
    auto rot_mat = getRotationMatrix2D(Point(middle_w, middle_h), angle, 1.0);
    Mat rotated;
    warpAffine(im1, rotated, rot_mat, im1.size(), INTER_LINEAR);

    auto left_top = top_corners.first;
    auto left_top_rot = (rot_mat * Mat(Point3d(left_top.x, left_top.y, 1))).t();
    auto right_top = top_corners.second;
    auto right_top_rot = (rot_mat * Mat(Point3d(right_top.x, right_top.y, 1))).t();
    std::cout << "Orig:" << left_top << " Corr:" << left_top_rot << std::endl;

    vector<Point2d> lower_corners_rotated;
    for (auto & lower_corner : lower_corners) {
        Mat tmp = (rot_mat * Mat(Point3d(lower_corner.x, lower_corner.y, 1))).t();
        std::cout << "Orig:" << tmp.at<double>(0,0) << std::endl;
        lower_corners_rotated.push_back(Point(tmp.at<double>(0,0), tmp.at<double>(0,0)));
    }


    Mat canny;
    Canny(gray, canny, 50, 200);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    drawContours(im1, contours, 0, Scalar(255,0,0), 4);
    vector<Mat> channels;
    split(im1, channels);
    Mat thresh_blue;
    threshold(channels[0], thresh_blue, 100., 255., THRESH_BINARY_INV);
    Mat thresh_green;
    threshold(channels[1], thresh_green, 100., 255., THRESH_BINARY_INV);
    Mat thresh_red;
    threshold(channels[2], thresh_red, 100., 255., THRESH_BINARY_INV);

    Mat gr;
    bitwise_and(thresh_green, thresh_red, gr);

    Mat dark;
    bitwise_and(gr, thresh_blue, dark);

//    imshow("f", thresh_blue);
//    waitKey(4000);
//    imshow("f", thresh_green);
//    waitKey(4000);
    imwrite("/home/nonari/f.png", rotated);
    waitKey(4000);
    return 0;
}
