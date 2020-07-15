
#include <iostream>

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


int main() {
  VideoCapture cap(0); 
  if (!cap.isOpened()) {
      cerr << "ERROR! Unable to open camera\n";
      return -1;
  }

  // cap.set(CAP_PROP_FRAME_WIDTH, 800);
  // cap.set(CAP_PROP_FRAME_HEIGHT, 600);
  // std::cout << "conn cam " << cap.getBackendName() << std::endl;

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;

    Mat frame, a,b,diff;
    for (;;) {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        if (0 == a.rows ) {
	  cout << "init " << frame.cols << "x" << frame.rows << " " << endl;
          frame.copyTo(a); 
          frame.copyTo(b);
        }
	a.copyTo(b);
	frame.copyTo(a);
	absdiff(a,b,diff);
	// diff = a - b;
	
        Scalar mean(3), stddev(3);
        meanStdDev(diff, mean, stddev);

        // cout << "diff mean " << mean.at<double>(0) << " sigma " << stddev.at<double>(0) << endl;
        cout << "diff mean " << mean << " sigma " << stddev << endl;
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // show live and wait for a key with timeout long enough to show images
        imshow("diff", diff);
        if (waitKey(5) >= 0)
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
