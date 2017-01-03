//
// Created by danfergo on 03-01-2017.
//

#ifndef ARTOOL_SHAPE_H
#define ARTOOL_SHAPE_H


#include <iostream>
#include <vector>
#include <opencv2/core/types.hpp>
#include <opencv/cv.h>

using namespace std;
using namespace cv;

class Shape {

protected:
    std::vector<Point3f> vertices;
    std::string identifier;
    std::string type;
public:
    Shape();
    Shape(std::string identifier,std::string type, std::vector<Point3f> vertices);
    virtual void draw();
    static Shape importFromFile(std::string filePath);
    string getIdentifier() const;
};


#endif //ARTOOL_SHAPE_H
