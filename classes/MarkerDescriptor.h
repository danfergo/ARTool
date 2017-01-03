//
// Created by danfergo on 03-01-2017.
//

#ifndef ARTOOL_MARKERDESCRIPTOR_H
#define ARTOOL_MARKERDESCRIPTOR_H


#include "../util/Image.h"

using namespace cv;

class MarkerDescriptor {
    Image frontalView;
    vector<Point2f> corners;
    Point3f origin;
    string identifier;


public:
    MarkerDescriptor(Image bwImage, vector<Point2f>);
    Image getFrontalView();
    void setOrigin(Point3f origin);
    void setIdentifier(string identifier);
    vector<Point2f> getCorners();
    void exportToFile();
    friend std::ostream& operator<<(std::ostream&, const MarkerDescriptor&);
};

std::ostream& operator<<(std::ostream &strm, const MarkerDescriptor &a);


#endif //ARTOOL_MARKERDESCRIPTOR_H
