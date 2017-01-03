#ifndef ARTOOL_MarkerDetector_H
#define ARTOOL_MarkerDetector_H


#include "../util/Image.h"

class MarkerDetector {

   // Image &image;
    //std::vector<Point> contour;
    //Point2f corners [4];
    //int cornersi [4];
    // bool markerDetected = false;

public:
    MarkerDetector();

    //Image plotContours(Image image, Point origin);
    bool calcCorners( vector<Point> contour, Point2f (& corners) [4], int (& cornersi) [4]);

   // Point2f * getCorners();

   // bool isMarkerDetected();
   // void setContour(std::vector<Point> contour);


    bool validateStraightEdges(vector<Point> contour, Point2f corners[4], int cornersi[4]);
    bool validateMaxMinEdgeRatio(Point2f corners[4]);
    bool insideAnotherMarker(std::vector<vector<Point> >::iterator contourIt, std::vector<vector<Point> > othersContours);

    vector<vector<Point2f> > findMarkers(Image image);

private:
    bool validateArea(Image image, vector<Point> contour);



    //Point findFirst(Point from, Vec3b color);
    //void nextNeighbour(Point & foreground, Point & background, Vec3b color);
    Point neighbour(const Point & point, int angle);
    int mostDistantCorner(vector<Point> contour,  Vec4i edge, int si, int ei, float * dst = NULL);
};


#endif //ARTOOL_MarkerDetector_H
