#ifndef ARTOOL_MarkerDetector_H
#define ARTOOL_MarkerDetector_H


#include "Image.h"

class MarkerDetector {

    Image &image;
    std::vector<Point> contour;
    Point2f corners [4];
    bool markerDetected = false;

public:
    MarkerDetector(Image &image);

    Image plotContours(Point origin);
    Image plotMedians();

    Point2f * getCorners();

    bool isMarkerDetected();

private:

    Point findFirst(Point from, Vec3b color);

    void nextNeighbour(Point & foreground, Point & background, Vec3b color);
    Point neighbour(const Point & point, int angle);
    int mostDistantCorner(Vec4i edge, int si, int ei);
};


#endif //ARTOOL_MarkerDetector_H
