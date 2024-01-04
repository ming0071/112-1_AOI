#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

Mat src, dst, output;
vector<Vec3f> circles;
// 1, 60, 70, 50, 60, 80
// DefaultMinDist = gray.rows / 8
const int DefaultMinDist = 60, DefaultParam1 = 70, DefaultParam2 = 45, DefaultMinRadius = 60,
          DefaultRadmaxius = 80, MaxValue = 255;
int minDist, param1, param2, minRadius, Radmaxius, thresholdVal;

void minDistCall(int, void *);
void param1Call(int, void *);
void param2Call(int, void *);
void minRadiusCall(int, void *);
void RadmaxiusCall(int, void *);
void thresholdCall(int, void *);

void calibration(const Mat &input);
void calcCircles(const Mat &input, vector<Vec3f> &circles);
void drawCircle(Mat &input, const vector<Vec3f> &circles);

int main(int argc, char **argv)
{
    // pre-process
    src = imread("..\\data\\image\\345.bmp");
    // -------------------------------------------------------------------------
    // -------------                 threshold                  ----------------
    // -------------------------------------------------------------------------
    namedWindow("binary", WINDOW_AUTOSIZE);
    createTrackbar("threshold", "binary", nullptr, MaxValue, thresholdCall);
    setTrackbarPos("threshold", "binary", 220);

    while (1)
    {
        calibration(src);
        medianBlur(dst, dst, 3);
        cvtColor(dst, dst, COLOR_BGR2GRAY);
        threshold(dst, dst, thresholdVal, 255, THRESH_BINARY);
        imshow("binary", dst);
        waitKey(10);
        int key = waitKey(1);
        if (key == 'q' || key == 27)
        {
            break;
        }
    }
    destroyAllWindows();

    // -------------------------------------------------------------------------
    // -------------                   Hough                    ----------------
    // -------------------------------------------------------------------------
    namedWindow("output", WINDOW_AUTOSIZE);
    createTrackbar("minDist", "output", nullptr, MaxValue, minDistCall);
    setTrackbarPos("minDist", "output", DefaultMinDist);
    createTrackbar("param1", "output", nullptr, MaxValue, param1Call);
    setTrackbarPos("param1", "output", DefaultParam1);
    createTrackbar("param2", "output", nullptr, MaxValue, param2Call);
    setTrackbarPos("param2", "output", DefaultParam2);
    createTrackbar("minRadius", "output", nullptr, MaxValue, minRadiusCall);
    setTrackbarPos("minRadius", "output", DefaultMinRadius);
    createTrackbar("Radmaxius", "output", nullptr, MaxValue, RadmaxiusCall);
    setTrackbarPos("Radmaxius", "output", DefaultRadmaxius);

    while (1)
    {
        // process
        calcCircles(dst, circles);
        cvtColor(dst, output, COLOR_GRAY2BGR);
        drawCircle(output, circles);
        imshow("output", output);

        waitKey(10);
        int key = waitKey(1);
        if (key == 'q' || key == 27)
        {
            break;
        }
    }
    imshow("output", output);
    waitKey(0);
    return 0;
}

//-- call back
void minDistCall(int, void *)
{
    minDist = getTrackbarPos("minDist", "output");
}
void param1Call(int, void *)
{
    param1 = getTrackbarPos("param1", "output");
}
void param2Call(int, void *)
{
    param2 = getTrackbarPos("param2", "output");
}
void minRadiusCall(int, void *)
{
    minRadius = getTrackbarPos("minRadius", "output");
}
void RadmaxiusCall(int, void *)
{
    Radmaxius = getTrackbarPos("Radmaxius", "output");
}
void thresholdCall(int, void *)
{
    thresholdVal = getTrackbarPos("threshold", "binary");
}
//-- VOID
void calibration(const Mat &input)
{
    FileStorage fs("..\\data\\calibration.xml", FileStorage::READ);

    Mat cameraMatrix, distCoeffs;
    fs["cameraMatrix"] >> cameraMatrix;
    fs["distCoeffs"] >> distCoeffs;

    undistort(input, dst, cameraMatrix, distCoeffs);
    fs.release();
}
void calcCircles(const Mat &input, vector<Vec3f> &circles)
{
    Mat contours;
    Mat temp = input.clone();
    Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));

    // imshow("before", temp);
    // morphologyEx(temp, temp, MORPH_OPEN, kernel);
    // morphologyEx(temp, temp, MORPH_CLOSE, kernel);
    // imshow("after", temp);

    Canny(temp, contours, 50, 150);
    HoughCircles(contours, circles, HOUGH_GRADIENT, 1, minDist, param1, param2, minRadius, Radmaxius);
}
void drawCircle(Mat &input, const vector<Vec3f> &circles)
{
    for (int i = 0; i < circles.size(); i++)
    {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        circle(input, center, radius, Scalar(0, 0, 255), 3, 8, 0);
    }
}