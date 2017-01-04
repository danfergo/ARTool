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
#include "util/Shape.h"
#include "classes/Database.h"
#include "util/Camera.h"

const int ESC_KEY = 27;
const int SPC_KEY = 32;

string markerDatabasePath = "database";
string shapesDatabasePath = "shapes";

Database database;

void printMenuList(vector<string> fileNames, bool printIndexes) {
    for (int i = 0; i < fileNames.size(); i++) {
        cout << "    " << (printIndexes ? to_string(i + 1) + " - " : "") << Util::split(fileNames[i], '.')[0] << endl;
    }
}


void renderMarkers(Image &image, vector<MarkerDescriptor> &markers, CameraCalibrator calibrator) {
    Mat rvec, tvec;

    for (int i = 0; i < markers.size(); i++) {
        int r = 0;
        MarkerDescriptor marker = database.findSimilar(markers[i], r);
        vector<Point3f> pts = {Point3f(-1, -1, 0), Point3f(1, -1, 0), Point3f(1, 1, 0), Point3f(-1, 1, 0)};

        vector<Point3f> points = {pts[r], pts[(r+1)%4],pts[(r+2)%4],pts[(r+3)%4]};

        solvePnPRansac(points, markers[i].getCorners(), calibrator.getIntrinsicCoeffs(),
                       calibrator.getDistortionCoeffs(), rvec, tvec);

        vector<Point2f> pointProjection;
        projectPoints(points, rvec,tvec, calibrator.getIntrinsicCoeffs(),
                      calibrator.getDistortionCoeffs(), pointProjection);

        image.plotArrowedLine(pointProjection[0], pointProjection[1])
                .plotArrowedLine(pointProjection[0], pointProjection[3], Scalar(0,255,0));



        if (marker.getIdentifier() != "") { // marker found
            vector<Shape> shapes = database.findAllShapesForMarker(marker);
            for (int i = 0; i < shapes.size(); i++) {
                shapes[i].draw(image, rvec, tvec, calibrator.getIntrinsicCoeffs(), calibrator.getDistortionCoeffs());
            }
        }
    }
}


void renderAugmentedReality() {
    Camera camera(0);
    int threshold = 256;

    createTrackbar("Threshold", "Camera", &threshold, 256);

    Image image = camera.nextFrame().clone(), bw;
    vector<MarkerDescriptor> markers;
    CameraCalibrator calibrator = CameraCalibrator::importCoefficients();
    int key;
    do {
        image = camera.nextFrame();
        bw = image.clone().changeColorSpace(CV_BGR2GRAY).gaussianBlur(7).threshold(threshold);
        markers = MarkerDetector().findMarkers(bw);

        renderMarkers(image, markers, calibrator);

        image.writeText("Show markers to the camera.").show("Camera");
        key = waitKey(1);
    } while (key != SPC_KEY && key != ESC_KEY);

    destroyWindow("Camera");
    camera.release();


}


int manageAssociatedShapes() {

    int option, option2;
    vector<string> markersFileNames = Util::fileNamesAt(markerDatabasePath);
    vector<string> shapesFileNames = Util::fileNamesAt(shapesDatabasePath);

    do {
        cout << "Markers database: " << endl;
        printMenuList(markersFileNames, true);
        cout << endl;

        cout << "Markers database: " << endl;
        printMenuList(shapesFileNames, true);
        cout << endl;

        cout << "Associations: " << endl;
        for (int i = 0; i < database.markers.size(); i++) {
            cout << "    " << to_string(i + 1) << " - " << database.markers[i].getIdentifier() << " is associated to "
                 << database.shapes[i].getIdentifier() << endl;
        }

        cout << endl;

        cout << "Actions: " << endl;
        cout << "      - Insert a negative index to remove an existing association. (TODO)" << endl;
        cout << "      - Insert a marker index followed by an shape index to associate them" << endl;
        cout << endl;
        cout << "    0 - Go back" << endl;


        do {
            scanf("%d", &option);
        } while (option < 0 || (option != 0 && option > markersFileNames.size()));

        if (option == 0) {
            return 0;
        }


        scanf("%d", &option2);

        MarkerDescriptor markerDescriptor = database.findMarker(markersFileNames[option - 1]);
        if (markerDescriptor.getIdentifier() == "") {
            markerDescriptor = MarkerDescriptor::importFromFile(markersFileNames[option - 1]);
        }



        Shape shape = database.findShape(shapesFileNames[option2 - 1]);
        if (shape.getIdentifier() == "") {
            shape = Shape::importFromFile(shapesFileNames[option2 - 1]);
        }

        database.addAssociation(markerDescriptor, shape);

        cout << endl << endl << endl;
    } while (true);

}


int showMarkerDatabase() {
    int option;
    vector<string> markersFileNames = Util::fileNamesAt(markerDatabasePath);

    do {
        cout << "Markers database: " << endl;
        printMenuList(markersFileNames, true);
        cout << endl;
        cout << "    0 - Go back" << endl;
        cout << "Choose marker:" << endl;

        do {
            scanf("%d", &option);
        } while (option < 0 || (option != 0 && option > markersFileNames.size()));

        if (option == 0) {
            return 0;
        }

        Image marker(markerDatabasePath + "/" + markersFileNames[option - 1]);

        marker.show("Marker", 0);
        cvDestroyWindow("Marker");

        cout << endl << endl << endl;
    } while (true);
}

int main() {

    int option;
    do {
        cout << "What do yo wish to do?" << endl;
        cout << "    1 - Show markers database" << endl;
        cout << "    2 - Manage markers shapes associations" << endl;
        cout << "    3 - Render augmented reality" << endl << endl;
        cout << "    0 - Exit " << endl;

        scanf("%d", &option);

        cout << endl << endl << endl;
        switch (option) {
            case 0:
                cout << "Good bye!" << endl;
                return 0;
            case 1:
                showMarkerDatabase();
                break;
            case 2:
                manageAssociatedShapes();
                break;
            case 3:
                renderAugmentedReality();
                break;
            default:
                cout << "Bad option." << endl;
        }
        cout << endl << endl << endl;
    } while (true);

    //showMarkerDatabase();
    //readPyramid(pyramidPath);
}

