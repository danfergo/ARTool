//
// Created by danfergo on 02-01-2017.
//

#ifndef ARTOOL_CAMERACALIBRATOR_H
#define ARTOOL_CAMERACALIBRATOR_H

#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "../util/Image.h"

using namespace cv;
using namespace std;

class CameraCalibrator {
private:
    vector<Point3f> patternCornersWorldPos;
    int nCols, nRows, nSamples;
    Size frameSize;

    vector<vector<Point3f> > patternsCornersWorldPos;
    vector<vector<Point2f> > patternsCornersImagePos;

    vector<Point2f> tmpPatternCornersImagePos;

    Mat intrinsicCoeffs= Mat(3, 3, CV_32FC1);
    Mat distortionCoeffs;

public:
    CameraCalibrator(int nRows, int nCols, Size frameSize, int nSamples);
    void findChessboard(Image & image);
    void collectLastFrame();
    bool hasAllSamples();
    void calibrate();
    Image undistort(Image image);


    void exportCoefficients(string filename = "camera_calibration.xml");

    int getNumberOfCollectedSamples();
};


#endif //ARTOOL_CAMERACALIBRATOR_H
