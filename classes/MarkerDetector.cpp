//
// Created by danfergo on 21-12-2016.
//

#include "MarkerDetector.h"
#include "Geometry.h"
#include <iostream>
#include <math.h>

#define step(x)    ( x >= -0.01 && x <= 0.01 ? 0 : (x < 0 ? -1 : 1))

const double PI = atan(1) * 4;
const double halfC = PI / 180;


MarkerDetector::MarkerDetector(Image &img) : image(img) {
}

Image MarkerDetector::plotContours(Point origin) {

    if (!image.contains(origin)) {
        return image;
    }

    // image.plotCircle(origin);

    contour = std::vector<Point>();

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
                contour.push_back(foreground);

                if (foreground.x == firstBlack.x && foreground.y == firstBlack.y) {
                    std::cout << "TADAAAA " << endl;
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

Point MarkerDetector::neighbour(const Point &point, int angle) {
    double rad = halfC * angle;
    int dy = step(sin(rad));
    int dx = step(cos(rad));

    return point + Point(dx, -1 * dy);
}

Image MarkerDetector::plotMedians() {
    if (contour.size() == 0) return image;

    Point A = contour[0];
    Point B = contour[contour.size() / 2];

    Vec4i ab(A.x, A.y, B.x, B.y);

    int Ci = mostDistantCorner(ab, 0, (int) contour.size() / 2);

    int Di = mostDistantCorner(ab, (int) (contour.size() / 2), (int) contour.size() -1);

    Point C = contour[Ci];
    Point D = contour[Di];

    Vec4i cd(C.x, C.y, D.x, D.y);

    int Ai = mostDistantCorner(cd, Di, Ci);
    int Bi = mostDistantCorner(cd, Ci, Di);

    A = contour[Ai];
    B = contour[Bi];

    image.plotLine(C, D);
    image.plotLine(A, B);

    image.plotLine(A,C,Scalar(0,255,0));
    image.plotLine(C,B,Scalar(0,255,255));
    image.plotLine(B,D,Scalar(0,255,0));
    image.plotLine(D,A,Scalar(0,255,0));

    corners[0] = A;
    corners[1] = C;
    corners[2] = B;
    corners[3] = D;
    markerDetected = true;

    return image;
}

int MarkerDetector::mostDistantCorner(Vec4i edge, int si, int ei) {
    int mDC = si;
    float maxDst = 0.0f;
    int i = si;
    do {

        float dst = Geometry::distance(edge, contour[i]);
        if (dst > maxDst) {
            maxDst = dst;
            mDC = i;
        }
    } while((i = (int) (++i % contour.size())) != ei);
    return mDC;
}

Point2f *MarkerDetector::getCorners() {
    return corners;
}

bool MarkerDetector::isMarkerDetected() {
    return markerDetected;
}

