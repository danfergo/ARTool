//
// Created by danfergo on 03-01-2017.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv/cv.hpp>
#include "MarkerDescriptor.h"
#include "../util/Util.h"

MarkerDescriptor::MarkerDescriptor(string identifier, Image bwImage, vector<Point2f> corners) : identifier(identifier),
                                                                                                corners(corners) {
    if (corners.size() == 4) {
        Point2f margin[4] = {Point2f(5, 5), Point2f(5, 105), Point2f(105, 105), Point2f(105, 5)};
        frontalView = bwImage.clone().extractQuad(&corners[0], Size(110, 110)).extractQuad(margin, Size(100, 100));
    }else{
        frontalView = bwImage;
    }
}

MarkerDescriptor::MarkerDescriptor(Image bwImage, vector<Point2f> corners) : MarkerDescriptor("", bwImage, corners) {
}

Image & MarkerDescriptor::getFrontalView() {
    return frontalView;
}

void MarkerDescriptor::setOrigin(Point3f origin) {
    this->origin = origin;
}

vector<Point2f> MarkerDescriptor::getCorners() {
    return corners;
}

void MarkerDescriptor::setIdentifier(string identifier) {
    this->identifier = identifier;
}

void MarkerDescriptor::exportToFile() {
    imwrite("database/" + identifier + ".jpg", frontalView.mat);
}

std::ostream &operator<<(std::ostream &strm, const MarkerDescriptor &a) {
    return strm << a.identifier << "(" << ")";
}

string MarkerDescriptor::getIdentifier() const {
    return identifier;
}

MarkerDescriptor MarkerDescriptor::importFromFile(std::string filename) {
    Image image = Image("database/" + filename);
    image.changeColorSpace(CV_BGR2GRAY);
    return MarkerDescriptor(Util::split(filename, '.')[0], image, std::vector<cv::Point2f>());
}

MarkerDescriptor::MarkerDescriptor() : MarkerDescriptor("", Image(), vector<Point2f>()) {

}

bool MarkerDescriptor::matches(const MarkerDescriptor &descriptor, int & rotations) {
    Mat mat = descriptor.frontalView.mat;
    int r = 0;

    for(int i = 0; i < 4; i++){
        Mat difff;
        absdiff(frontalView.mat, mat, difff);
        double diff =  sum(difff)[0];

        if((diff / (double)(100*100*3)) < 7){
            rotations = r;
            return true;
        }

        Mat rot_mat = getRotationMatrix2D(Point(50,50), 90, 1.0f);
        warpAffine(mat,mat, rot_mat, Size(100,100));
        r++;
    }


    return false;
}

