//
// Created by danfergo on 21-12-2016.
//

#include "MarkerDetector.h"
#include "../util/Geometry.h"
#include <iostream>
#include <math.h>
#include <opencv/cv.hpp>

#define step(x)    ( x >= -0.01 && x <= 0.01 ? 0 : (x < 0 ? -1 : 1))

const double PI = atan(1) * 4;
const double halfC = PI / 180;


MarkerDetector::MarkerDetector()  {
}
/*
Image MarkerDetector::plotContours(Image image, Point origin) {

    if (!image.contains(origin)) {
        return image;
    }

    // image.plotCircle(origin);

    //contour = std::vector<Point>();

    // image.plotCircle(origin);

    Point firstBlack = findFirst(origin, Vec3b(0, 0, 0));
    if (image.contains(firstBlack)) {
        //image.plotCircle(firstBlack);
        Point foreground = firstBlack;


        Point background = neighbour(firstBlack, 180);

        //image.at3(foreground) = Vec3b(255, 0, 0);
        image.at3(background) = Vec3b(0, 255, 0);

        for (int i = 0; i < 1000; i++) {
            nextNeighbour(foreground, background, Vec3b(0, 0, 0));

            if (image.contains(foreground)) {


               image.at3(foreground) = Vec3b(255, 0, 0);
               image.at3(background) = Vec3b(0, 255, 0);
//                contour.push_back(foreground);

                if (foreground.x == firstBlack.x && foreground.y == firstBlack.y) {
                   // std::cout << "TADAAAA " << endl;
                    break;
                }
            } else {
                break;
            }
        }

    }


    return image;
}

Point MarkerDetector::findFirst(Point from, Vec3b color) {
    for (int i = from.y; i < image.height(); i++) {
        for (int j = from.x; j < image.width(); j++) {
            if (image.at3(i, j) == color) {
                return Point2i(j, i);
            }
        }
    }
    return Point(image.height() + 10, image.width() + 10);
}
*/
/*
void MarkerDetector::nextNeighbour(Point &foreground, Point &background, Vec3b color) {

    Point delta = background - foreground;
    delta.y = -delta.y;

    double angle = fmod((atan2(delta.y, delta.x) * 180 / PI), 360);

    Point nb = neighbour(foreground, angle);
    for (int i = 0; i < 8; i++) {

        if (image.at3(nb) == color) {
            foreground = nb;
            return;
        }
        background = nb;
        angle = fmod((angle + 45), 360);
        nb = neighbour(foreground, angle);
    }


    foreground = Point(-1, -1);
}
*/
Point MarkerDetector::neighbour(const Point &point, int angle) {
    double rad = halfC * angle;
    int dy = step(sin(rad));
    int dx = step(cos(rad));

    return point + Point(dx, -1 * dy);
}

bool MarkerDetector::calcCorners(vector<Point> contour, Point2f (& corners) [4], int (& cornersi) [4]) {
    if (contour.size() == 0) return false;

    Point A = contour[0];
    Point B = contour[contour.size() / 2];

    Vec4i ab(A.x, A.y, B.x, B.y);

    int Ci = mostDistantCorner(contour, ab, 0, (int) contour.size() / 2);

    int Di = mostDistantCorner(contour, ab, (int) (contour.size() / 2), (int) contour.size() -1);

    Point C = contour[Ci];
    Point D = contour[Di];

    Vec4i cd(C.x, C.y, D.x, D.y);

    int Ai = mostDistantCorner(contour,cd, Di, Ci);
    int Bi = mostDistantCorner(contour,cd, Ci, Di);

    A = contour[Ai];
    B = contour[Bi];



 //   image.plotLine(A,C,Scalar(0,255,0));
 //   image.plotLine(C,B,Scalar(0,255,255));
 //   image.plotLine(B,D,Scalar(0,255,0));
 //   image.plotLine(D,A,Scalar(0,255,0));

    corners[0] = A;
    corners[1] = C;
    corners[2] = B;
    corners[3] = D;

    cornersi[0] = Ai;
    cornersi[1] = Ci;
    cornersi[2] = Bi;
    cornersi[3] = Di;


    return true;
}

int MarkerDetector::mostDistantCorner(vector<Point> contour, Vec4i edge, int si, int ei , float * dst) {
    int mDC = si;
    float maxDst = 0.0f;
    int i = si;
    do {
        Point2f intersection;
        float dst = Geometry::distance(edge, contour[i], &intersection);
        if (dst > maxDst) {
            maxDst = dst;
            mDC = i;
        }
    } while((i = (int) (++i % contour.size())) != ei);

    if(dst != NULL){
        *dst = maxDst;
    }
    return mDC;
}
/*
Point2f *MarkerDetector::getCorners() {
    return corners;
}

bool MarkerDetector::isMarkerDetected() {
    return markerDetected;
}

void MarkerDetector::setContour(std::vector<Point> contour) {
    this->contour = contour;
}*/

bool MarkerDetector::validateStraightEdges(vector<Point> contour, Point2f corners[4], int cornersi[4]) {
    for(int i = 0; i < 4; i++){
        int i2 = (i+1)%4;

        Vec4f side(corners[i2].x, corners[i2].y, corners[i].x, corners[i].y);
        float edgeWidth = Geometry::distance(corners[i], corners[i2]);

        float dist;
        mostDistantCorner(contour, side, cornersi[i], cornersi[i2], &dist);

        if(dist > 0.05f * edgeWidth){
            return false;
        }
    }

    return true;
}

bool MarkerDetector::validateMaxMinEdgeRatio(Point2f corners[4]) {
    double maxv = -1, minv = -1;

    for(int i = 0; i < 4; i++) {
        int i2 = (i+1)%4;
        double edgeWidth = Geometry::distance(corners[i], corners[i2]);
        minv = minv == -1 ? edgeWidth : min(edgeWidth, minv);
        maxv = maxv == -1 ? edgeWidth : max(edgeWidth, maxv);
    }

    return maxv != 0 && minv/maxv > 0.25f;
}

bool MarkerDetector::insideAnotherMarker(std::vector<vector<Point> >::iterator contourIt, std::vector<vector<Point> > othersContours) {
    for(vector<vector<Point> >::iterator it = othersContours.begin(); it!= othersContours.end(); it++){
        if(it != contourIt){
            for(vector<Point >::iterator itt = (*contourIt).begin(); itt!= (*contourIt).end(); itt++){
                if (pointPolygonTest((*it), *itt, false) > 0) {
                    return true;
                }
            }
        }

    }
    return false;
}



vector<MarkerDescriptor> MarkerDetector::findMarkers(Image image) {
    vector<vector<Point> > rawContours;
    vector<Vec4i> hierarchy;

    //Image imageForCanny = image.clone().changeColorSpace(CV_BGR2GRAY).gaussianBlur(7);

    findContours(image.mat, rawContours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0,0));

    vector<vector<Point2f> > markersCorners;
    vector<vector<Point> >  markersContours;

    for(vector<vector<Point> >::iterator it = rawContours.begin(); it!= rawContours.end(); it++){
        Point2f corners [4];
        int cornersi [4];
        if(validateArea(image, *it)
           && calcCorners((*it), corners, cornersi)
           && validateMaxMinEdgeRatio(corners)
           && validateStraightEdges(*it, corners, cornersi)){
            markersCorners.push_back(vector<Point2f>(std::begin(corners), std::end(corners)));
            markersContours.push_back(vector<Point>(std::begin(*it), std::end(*it)));

        }

    }

    vector<vector<Point> >::iterator it = markersContours.begin();
    vector<vector<Point2f> >::iterator it2 = markersCorners.begin();

    for(; it!= markersContours.end();) {
        if(insideAnotherMarker(it, markersContours)){
            it = markersContours.erase(it);
            it2 = markersCorners.erase(it2);
        }else{
            it++;
            it2++;
        }
    }

    vector<MarkerDescriptor> markers;
    for(vector<vector<Point2f> >::iterator it = markersCorners.begin(); it!= markersCorners.end(); it++) {
        markers.push_back(MarkerDescriptor(image,*it));
    }

        return markers;
}

bool MarkerDetector::validateArea(Image image, vector<Point> contour) {
    double totalArea =  image.width() * image.height();
    double area =  contourArea(contour);
    return area > 0.05f*totalArea && area < 0.95f*totalArea;
}

