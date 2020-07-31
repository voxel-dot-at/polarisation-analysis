#include <time.h>

#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion

#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O

using namespace cv;
using namespace std;


Mat doCanny(Mat& img, string name)
{
    double min, max;
    cv::minMaxLoc(img, &min, &max);
    double lower = min + (max-min)*.20;
    double upper = min + (max-min)*.80;
    cout << name << ":\tmin " << min << " max " << max << " lower " << lower << " upper " << upper << endl;
    Mat canny;
    Canny 	( 	img, canny, lower, upper, 3);

    imshow(name, canny);
    imwrite(name + ".jpg", canny);
    return canny;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
      cerr << "Usage: " << argv[0] << " a.jpg b.jpg" << endl;
      exit(1);
    }
    char c;
    Mat a = imread(argv[1]);
    Mat b = imread(argv[2]);

    Mat plus = a/2 + b/2 ;
    Mat minus;

    absdiff(a,b,minus);

    imshow("a", a);
    imshow("b", b);

    imshow("a+b", plus);
    imshow("|a-b|", minus);

    c = waitKey();

    Mat luv, planes[3];

    cvtColor 	(  minus, luv, COLOR_BGR2Luv);
    split(luv, planes);
    //cvtColor 	(  minus, planes, COLOR_BGR2Luv);
    imshow("l", planes[0]);
    imshow("u", planes[1]);
    imshow("v", planes[2]);

    Mat lu;
    absdiff(planes[1],planes[2],lu);
    imshow("|l-u|", lu);
    imshow("l+u", planes[1]+planes[2]);
cout << "writing" << endl;
    imwrite("l.jpg", planes[0]);
    imwrite("u.jpg", planes[1]);
    imwrite("v.jpg", planes[2]);
    imwrite("lu.jpg", lu);
    // imwrite("l+u", planes[1]+planes[2]);

    if (false) { // DoG
      Mat g1, g2, result;
      GaussianBlur(lu, g1, Size(3,3), 0);
      GaussianBlur(lu, g2, Size(1,1), 0);
      result = g1 - g2;
      imshow("g-g", result);
    }

    Mat edges = doCanny(lu, "canny-lu");
    edges += doCanny(planes[0], "canny-l");
    edges += doCanny(planes[1], "canny-u");
    edges += doCanny(planes[2], "canny-v");
    imshow("edges", edges);

    if (false) { // Laplacian
    Mat abs_dst;
      Mat dst;
      int kernel_size = 7;
      int scale = 1;
      int delta = 0;
      int ddepth = CV_16S;
          GaussianBlur( lu, lu, Size(7, 7), 0, 0, BORDER_DEFAULT );

      Laplacian( lu, dst, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
      // converting back to CV_8U
      convertScaleAbs( dst, abs_dst );
    imshow("abs", abs_dst);
    }



    c = waitKey();
    return 0;
}
