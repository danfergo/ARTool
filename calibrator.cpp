#include <iostream>
#include <opencv/cv.h>
#include <opencv2/videoio.hpp>
#include <opencv/cv.hpp>
#include "classes/CameraCalibrator.h"
#include "util/Camera.h"
#include "classes/MarkerDetector.h"

using namespace std;
using namespace cv;

const int ESC_KEY = 27;
const int SPC_KEY = 32;
const int NO_KEY = 255;

const Point NULLP = Point(-1, -1);


Point markerOrigin = NULLP;


void onSelectOrigin(int event, int x, int y, int flags, void *userdata) {
    if (event == EVENT_LBUTTONDOWN) {
        markerOrigin = Point(x, y);
    } else if (event == EVENT_RBUTTONDOWN) {
        markerOrigin = NULLP;
    }
}

void detectOneMarker() {

    Camera camera(0);


    int threshold = 256;

    namedWindow("Camera", 0);
    setMouseCallback("Camera", onSelectOrigin, NULL);
    createTrackbar("Threshold", "Camera", &threshold, 256);

    Image image = camera.nextFrame().clone(), bw;
    MarkerDetector markerDetector;
    vector<vector<Point2f> > markers;


    // BUG WTF a mostrar os marcadores no fim .
    // a ideia era mostrar marcador a marcador, e perguntar ao user as coordenadas da origem


    int key;
    do {
        image = camera.nextFrame().clone();

        // image.clone().changeColorSpace(CV_BGR2GRAY).plotNthChannelHistogram(0).show("Camera histogram"); - aqui aparece
        bw = image.clone().changeColorSpace(CV_BGR2GRAY).gaussianBlur(7).threshold(threshold);
        bw.show("bw");

        markers = markerDetector.findMarkers(bw);

        for (int i = 0; i < markers.size(); i++) {
            image.plotPolygon(markers[i]);
            //bw.clone().extractQuad(&markers[i][0], Size(320, 320)).show("Marker "); - aqui aparece

        }

        image.writeText("Press space to save the current markers.").show("Camera");

        key = waitKey(1);


    } while (key != SPC_KEY && key != ESC_KEY);

    //destroyWindow("Camera");
    //camera.release();

    cout << markers.size() << " " << key;

    if (key == SPC_KEY) {

        for (int i = 0; i < markers.size(); i++) {
            bw.clone().extractQuad(&markers[i][0], Size(320, 320)).show("Marker "); //  - aqui nao aparece

            bw.show("bw2");
            key = waitKey(30);

            string identifier;

            printf("Whats the number of corners along width (number of columns  - 1): ");
            cin >> identifier;

            //scanf("%s", &identifier);
            int x, y, z;
            printf("x y z?");
            scanf("%d %d %d", &x, &y, &z);

            cout << identifier << ": " << to_string(x) << " " << to_string(y) << " " << to_string(z) << " " << endl;

            cv::destroyWindow("Marker " + to_string(i + 1));
        }
    }
/*
    for (int i = 0; i < markers.size(); i++) {

        clone.changeColorSpace(CV_GRAY2BGR);
        image.plotLine(markers[i][1], markers[i][3], Scalar(0, 0, 255));
        image.plotLine(markers[i][0], markers[i][2], Scalar(0, 255, 255));

    }*/

}

void calibrateCamera(int nRows, int nCols, int nSamples) {
    Camera camera(0);
    CameraCalibrator cameraCalibrator = CameraCalibrator(nRows, nCols, camera.nextFrame().size(), nSamples);

    int key;

    do {
        Image frame = camera.nextFrame();
        cameraCalibrator.findChessboard(frame);
        string legend = "Press space to save the current frame. Saved " +
                        to_string(cameraCalibrator.getNumberOfCollectedSamples()) + " of " + to_string(nSamples) + ".";
        frame.writeText(legend).show("Camera");
        key = waitKey(1);

        if (key == SPC_KEY) {
            cameraCalibrator.collectLastFrame();
        }

    } while (!cameraCalibrator.hasAllSamples() && key != ESC_KEY);

    cameraCalibrator.calibrate();
    cameraCalibrator.exportCoefficients();
    destroyWindow("Camera");
    std::cout << "Calibration performed and saved at camera_calibration.xml" << endl;
}

void calibrateCameraDefineCustomConfigs(int &nRows, int &nCols, int &nSamples) {
    printf("Whats the number of corners along width (number of columns  - 1): ");
    scanf("%d", &nCols);

    printf("Whats the number of corner along height (number of rows - 1): ");
    scanf("%d", &nRows);

    printf("Number of samples to use: ");
    scanf("%d", &nSamples);
}


int calibrateCameraChooseConfigs() {
    int stay, option;
    do {
        stay = false;

        cout << "Lets define the calibration dimensions:" << endl;
        cout << "    1 - It is the default one 7x9 23mm 10 samples" << endl;
        cout << "    2 - Custom configuration " << endl;
        cout << "    0 - Go back " << endl;

        scanf("%d", &option);

        cout << endl << endl << endl;
        switch (option) {
            case 0:
                return 0;
            case 1:
                calibrateCamera(9, 7, 10);
                break;
            case 2:
                int nCols, nRows, nSamples;
                calibrateCameraDefineCustomConfigs(nRows, nCols, nSamples);
                calibrateCamera(nCols, nRows, nSamples);
                break;
            default:
                cout << "Bad option." << endl;
        }
        cout << endl << endl << endl;
    } while (stay);
}


int main(int argc, char *argv[]) {

    int option;
    do {
        cout << "What do yo wish to do?" << endl;
        cout << "    1 - Calibrate camera" << endl;
        cout << "    2 - Add marker" << endl;
        cout << "    0 - Exit " << endl;

        scanf("%d", &option);

        cout << endl << endl << endl;
        switch (option) {
            case 0:
                cout << "Good bye!" << endl;
                return 0;
            case 1:
                calibrateCameraChooseConfigs();
                break;
            case 2:
                detectOneMarker();
                break;
            default:
                cout << "Bad option." << endl;
        }
        cout << endl << endl << endl;
    } while (true);
}