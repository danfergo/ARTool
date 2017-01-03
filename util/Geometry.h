#ifndef DISPLAYIMAGE_GEOMETRY_H
#define DISPLAYIMAGE_GEOMETRY_H

#include <opencv2/core/core.hpp>

using namespace cv;

class Geometry {
public:
    static double slopeAngle(Vec4f edge);
    static Vec2f intersect(Vec4f edge1, Vec4f edge2);
    static float distance(Vec2f x, Vec2f y);
    static float distance(Vec4f ab, Point2f c, Point2f * intersection = NULL);
    static Vec2f midpoint(Vec4f edge);
    static float areaTriangle(Point2f a, Point2f b, Point2f c);

};


#endif //DISPLAYIMAGE_GEOMETRY_H
