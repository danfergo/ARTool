#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#include "classes/Image.h"
#include "classes/MarkerDetector.h"
#include <GL/glut.h>


Point markerOrigin = Point(-1, -1);

bool drawn = false;

void onClick(int event, int x, int y, int flags, void *userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        markerOrigin = Point(x, y);
        drawn = false;
    } else if (event == EVENT_RBUTTONDOWN) {
        markerOrigin = Point(-1, -1);
    }
}

int main() {
    VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.

    if (!stream1.isOpened()) { //check if video device has been initialised
        cout << "cannot open camera";
    }

//unconditional loop
    int threshold = 100;

    namedWindow("image", 0);
    setMouseCallback("image", onClick, NULL);

    Image image;

    while (true) {
        //      if (!image.contains(markerOrigin)) {
        stream1.read(image.mat);
        //   cv::Rect myROI(50, 50, 100, 100);
        //image.mat = image.mat(myROI);
/*
        }*/

        image.clone().changeColorSpace(CV_BGR2GRAY).plotNthChannelHistogram(0).show("cam hist");


        Image clone = image.clone().changeColorSpace(CV_BGR2GRAY).gaussianBlur(7).threshold(
                threshold).changeColorSpace(CV_GRAY2BGR);


        createTrackbar("Threshold", "image", &threshold, 255);


        MarkerDetector markerDetector(clone);

        markerDetector.plotContours(markerOrigin);
        markerDetector.plotMedians();
        clone.show("image");

//        Point2f  corners [4] = { Point2f(0,0), Point2f(,2), Point2f(1,2), Point2f(1,2)};

        if (markerDetector.isMarkerDetected()) {
            clone.clone().extractQuad(markerDetector.getCorners(), Size(300, 300)).show("Marker");
        }

        /*if (image.contains(markerOrigin)) {
            waitKey(0);
        }*/

        if (waitKey(30) == 0)
            break;
    }
    return 0;
}
