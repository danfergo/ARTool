#include "util/Util.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;

#include "util/Image.h"
#include "classes/MarkerDetector.h"
#include "classes/CameraCalibrator.h"

string markerDatabasePath = "assets/database/";
string pyramidPath = "assets/shapes/pyramid.txt";

Image extractedQuad;

vector<Point3i> pyramidPoints;

bool drawn = false;

void onClick(int event, int x, int y, int flags, void *userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        markerOrigin = Point(x, y);
        drawn = false;
    } else if (event == EVENT_RBUTTONDOWN) {
        markerOrigin = Point(-1, -1);
    }
}

void onClickSaveImage(int event, int x, int y, int flags, void *userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		cout << "Marker name: " << endl;
		string markerName;
		cin >> markerName;
		string path = markerDatabasePath + markerName + ".jpg";
		imwrite(path, extractedQuad.mat);
	}
}

void readPyramid(string filePath) {
	ifstream infile;
	infile.open(filePath.c_str());

	vector <string> record, record2;
	while (infile)
	{
		string s;
		if (!getline(infile, s)) break;

		istringstream ss(s);
		

		while (ss)
		{
			string s;
			if (!getline(ss, s, '/')) break;
			record.push_back(s);
			istringstream ss(s);
		}
	}
	if (!infile.eof())
	{
		cerr << "Fooey!\n";
	}

	for (int i = 0; i < record.size(); i++) {

		istringstream ss2(record[i]);
		while (ss2) {
			string s2;
			if (!getline(ss2, s2, ',')) break;
			record2.push_back(s2);
		}
	}

	for (int i = 0; i < record2.size(); i+=3) {
		istringstream buffer(record2[i]);
		int x;
		buffer >> x;
		istringstream buffer2(record2[i+1]);
		int y;
		buffer2 >> y;
		istringstream buffer3(record2[i+2]);
		int z;
		buffer3 >> z;
		Point3i p(x, y, z);
		pyramidPoints.push_back(p);
	}
	


}

void detectMarkers() {
	VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.

	if (!stream1.isOpened()) { //check if video device has been initialised
		cout << "cannot open camera";
	}

	//SimpleBlobDetector::Params blobParams;
	//blobParams.filterByCircularity = true;
	//blobParams.minCircularity = 0;
	//blobParams.maxCircularity = 0.65;
	//blobParams.filterByConvexity = false;
	//blobParams.filterByColor = false;
	////blobParams.blobColor = 255;
	//blobParams.filterByArea = true;
	//blobParams.minArea = 2000;
	//blobParams.maxArea = 10000;
	//blobParams.minDistBetweenBlobs = 400;
	//Ptr<SimpleBlobDetector> blobDetector = SimpleBlobDetector::create(blobParams);
	//vector<KeyPoint> blobKeypoints;
	//DrawMatchesFlags blobKeypointsFlag;

	vector<vector<Point2f> > contours;
	vector<Vec4i> hierarchy;

	//unconditional loop
	int threshold = 80;

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



		/*blobDetector->detect(clone.mat, blobKeypoints);
		drawKeypoints(clone.mat, blobKeypoints, clone.mat, Scalar(0, 0, 255), blobKeypointsFlag.DRAW_RICH_KEYPOINTS);
		cout << blobKeypoints.size() << endl;*/

		Image imageForCanny = image.clone().changeColorSpace(CV_BGR2GRAY).gaussianBlur(7);
		Canny(imageForCanny.mat, imageForCanny.mat, threshold, 255);

        double totalArea =  image.width() * image.height();


		//find Contours
		findContours(imageForCanny.mat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		/// Draw contours
		Scalar color = Scalar(0, 255, 0);
		vector<vector<Point2> > contoursDrawn;
		for (int i = 0; i < contours.size(); i++) {
			if ( 0.005f*totalArea < contourArea(contours[i])  )
			{
				contoursDrawn.push_back(contours[i]);
				drawContours(clone.mat, contours, i, color, 2, 8, hierarchy, 0, Point());
			}
		}

		//Filter contours that were detected very near each other
		for (int i = 0; i < contoursDrawn.size(); i++) {
			if (i + 1 < contoursDrawn.size()) {
				//if the X coordinate of the first point of the current contour is very near the X coordinate of the first point of the next contour then we eliminate the next contour since
				//the first point of a counter is always it's up left corner
				if (contoursDrawn[i][0].x > contoursDrawn[i + 1][0].x - 5 && contoursDrawn[i][0].x < contoursDrawn[i + 1][0].x + 5) {
					contoursDrawn.erase(contoursDrawn.begin() + (i + 1));
				}
                else {

                }
			}



        }

		cout << "Contours obtained: " << contoursDrawn.size() << endl;
        for (int i = 0; i < contoursDrawn.size(); i++) {
            MarkerDetector markerDetector(clone);
            markerDetector.setContour(contoursDrawn[i]);
			markerDetector.calcCorners();
			markerDetector.validateStraightEdges(nullptr, nullptr);
        }

		//extract the markers from the image
		//for (int i = 0; i < contoursDrawn.size(); i++) {

		//	//Find the marker corners

		//	Point2f upLeftCorner, upRightCorner, downLeftCorner, downRightCorner;

		//	Point2f corners[4];
		//	string windowName = "Marker " + i;
		//	clone.clone().extractQuad(corners, Size(300, 300)).show(windowName);
		//}

		//markerDetector.plotContours(markerOrigin);
		clone.show("image");

		//        Point2f  corners [4] = { Point2f(0,0), Point2f(,2), Point2f(1,2), Point2f(1,2)};

//		if (markerDetector.isMarkerDetected()) {
//			clone.clone().extractQuad(markerDetector.getCorners(), Size(300, 300)).show("Marker");
//		}

		/*if (image.contains(markerOrigin)) {
		waitKey(0);
		}*/

		if (waitKey(30) == 0)
			break;
	}
}

void showMarker(string markerPath) {
	Image marker(markerPath);
	marker.show("Marker", 0);
	cvDestroyWindow("Marker");
}

void showMarkerDatabase() {
	vector<string> markerPaths = Util::fileNamesAt(markerDatabasePath);
	int option = -1;
	while (option != 0) {


		int internalOption = -1;
		if (option != 0) {
			while (internalOption != 0) {
				cout << "Action: " << endl;
				cout << "1 - Show Marker: " << endl;
				cout << "2 - Associate Pyramid to Marker: " << endl << endl;

				cout << "0 - Leave" << endl;
				cin >> internalOption;

				switch (internalOption) {
				case 1:
					showMarker(markerPaths[option - 1]);
				case 2:

				default:
					break;
				}
				
			}
		}
	}
}

int main() {
	detectMarkers();
	//showMarkerDatabase();
	//readPyramid(pyramidPath);
    return 0;
}
