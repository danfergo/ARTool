//
// Created by danfergo on 02-01-2017.
//

#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>

#include "CameraCalibrator.h"

void CameraCalibrator::calibrate() {

    //string text = std::to_string(successes) +  "/" + std::to_string(numBoards);
    //putText(image, text, Size(10,400), FONT_HERSHEY_SIMPLEX, 1, Scalar(255,255,255), 2, LINE_AA);

    vector<Mat> rvecs;
    vector<Mat> tvecs;

    // intrinsicCoeffs.ptr<float>(0)[0] = 1;
    // intrinsicCoeffs.ptr<float>(1)[1] = 1;

    calibrateCamera(patternsCornersWorldPos, patternsCornersImagePos, frameSize, intrinsicCoeffs, distortionCoeffs, rvecs, tvecs);
}

void CameraCalibrator::collectLastFrame() {
    if (!tmpPatternCornersImagePos.empty()) {
        patternsCornersWorldPos.push_back(patternCornersWorldPos);
        patternsCornersImagePos.push_back(tmpPatternCornersImagePos);
    }
}

CameraCalibrator::CameraCalibrator(int nRows, int nCols, Size frameSize, int nSamples) : nRows(nRows), nCols(nCols),
                                                                                         frameSize(frameSize),
                                                                                         nSamples(nSamples),
                                                                                         tmpPatternCornersImagePos(),
                                                                                         patternsCornersImagePos() {

    patternCornersWorldPos = vector<Point3f>();

    for (int i = 0; i < nRows; i++) {
        for (int j = 0; j < nCols; j++) {
            patternCornersWorldPos.push_back(Point3f(j, i, 0.0f));
        }
    }

}

void CameraCalibrator::findChessboard(Image &image) {
    cvtColor(image.mat, image.mat, CV_BGR2GRAY);
    tmpPatternCornersImagePos.clear();

    Size patternSize = Size(nCols, nRows);
    bool found = findChessboardCorners(image.mat, patternSize, tmpPatternCornersImagePos,
                                       CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
    if (found) {
        cornerSubPix(image.mat, tmpPatternCornersImagePos, Size(11, 11), Size(-1, -1),
                     TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
        drawChessboardCorners(image.mat, patternSize, tmpPatternCornersImagePos, true);
    } else {
        tmpPatternCornersImagePos.clear();
    }
}

bool CameraCalibrator::hasAllSamples() {
    return patternsCornersImagePos.size() >= nSamples;
}

Image CameraCalibrator::undistort(Image image){
    Mat undistorted;
    cv::undistort(image.mat, undistorted, intrinsicCoeffs, distortionCoeffs);
    return Image(undistorted);
}

void CameraCalibrator::exportCoefficients(string filename) {
    FileStorage fs(filename, FileStorage::WRITE);
    fs << "intrinsic_coeffs" << intrinsicCoeffs;
    fs << "distortion_coeffs" << distortionCoeffs;
    fs.release();
}

int CameraCalibrator::getNumberOfCollectedSamples() {
    return (int)patternsCornersImagePos.size();
}

