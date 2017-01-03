//
// Created by danfergo on 02-11-2016.
//

#include "Geometry.h"

double Geometry::slopeAngle(Vec4f points) {
    return fastAtan2(points[2] - points[0], points[3] - points[1]);
}

Vec2f Geometry::intersect(Vec4f edge1, Vec4f edge2) {
    float x1 = edge1[0], y1 = edge1[1], x2 = edge1[2], y2 = edge1[3];
    float x3 = edge2[0], y3 = edge2[1], x4 = edge2[2], y4 = edge2[3];

    float x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4))
              / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    float y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4))
              / ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));

    return cv::Vec2i(x, y);
}

float Geometry::distance(Vec2f x, Vec2f y) {
    return sqrt(pow(y[0] - x[0], 2) + pow(y[1] - x[1], 2));
}

float Geometry::distance(Vec4f ab, Point2f c, Point2f * intersection) {
    Point2f A = Point2f(ab[0], ab[1]);
    Point2f B = Point2f(ab[2], ab[3]);
    Point2f AB = B - A;
    Point2f c2 = c + Point2f(AB.y, -AB.x);
    Vec4f c2c(c.x, c.y, c2.x, c2.y);
    if(intersection != NULL){
        *intersection = intersect(ab, c2c);
        return distance(*intersection, c);
    }
    return distance(intersect(ab, c2c), c);
}


Vec2f Geometry::midpoint(Vec4f e) {
    return cv::Vec2f((e[0] + e[2]) / 2, (e[1] + e[3]) / 2);
}

