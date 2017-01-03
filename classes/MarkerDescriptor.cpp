//
// Created by danfergo on 03-01-2017.
//

#include <opencv2/imgcodecs.hpp>
#include "MarkerDescriptor.h"

MarkerDescriptor::MarkerDescriptor(Image bwImage, vector<Point2f> corners): corners(corners) {
    Point2f margin[4] = {Point2f(5,5), Point2f(5,105), Point2f(105,105) , Point2f(105,5)};
    frontalView = bwImage.clone().extractQuad(&corners[0], Size(110, 110)).extractQuad( margin , Size(100,100));
}

Image MarkerDescriptor::getFrontalView() {
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

std::ostream& operator<<(std::ostream &strm, const MarkerDescriptor &a) {
    return strm << a.identifier << "("  << ")";
}

