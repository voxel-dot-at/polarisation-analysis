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


int main() {
  VideoCapture cap(0); 
  if (!cap.isOpened()) {
      cerr << "ERROR! Unable to open camera\n";
      return -1;
  }

  cap.set(CAP_PROP_FRAME_WIDTH, 1280);
  cap.set(CAP_PROP_FRAME_HEIGHT, 720);
  // std::cout << "conn cam " << cap.getBackendName() << std::endl;

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;

    Mat frame, a,b,diff;
    Mat display, pb;
    double sigma;
    double thresh = 15.0;
    bool changing=false;
    int iter=0;
    for (;;) {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        if (0 == a.rows ) {
	  cout << "init " << frame.cols << "x" << frame.rows << " " << endl;
          frame.copyTo(a); 
          frame.copyTo(b);
          frame.copyTo(pb);
        }
	a.copyTo(b);
	frame.copyTo(a);
	absdiff(a,b,diff);
	// diff = a - b;
	
        Scalar mean(3), stddev(3);
        meanStdDev(diff, mean, stddev);
	sigma = (stddev[0] + stddev[1] + stddev[2] ) / 3.;

        cout << "diff mean " << mean << "\tsigma " << stddev << "\t\t" << sigma << endl;
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // show live and wait for a key with timeout long enough to show images
	//        imshow("diff", diff);
	//        if (waitKey(5) >= 0)
        //    break;

	// state machine:
	if (changing) {
	  if (sigma <= thresh*0.9 ) { // - 10% hysteresis
	    changing = false;
	    cout << "changed! " << endl;

	    // save current state:
	    stringstream s;
	    s << "/tmp/img_" << iter++;
	    string fname_a = s.str() + "_a.jpg";
	    string fname_b = s.str() + "_b.jpg";
	    vector<int> compression_params;
	    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	    compression_params.push_back(90);
	    imwrite(fname_a, frame, compression_params);
	    imwrite(fname_b, pb, compression_params);
	  }
	  cout << "changing! " << endl;
	} else {
	  if (sigma > thresh) {
	    changing = true;
	    b.copyTo(pb); // keep pre-change image
	  } else {
	    absdiff(frame, pb, diff);
	    resize(diff, display, Size(640,480));
	    imshow("diff", display);
	    if (waitKey(5) >= 0)
	      break;
	  }
	}
	
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
