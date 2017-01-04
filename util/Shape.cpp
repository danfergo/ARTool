//
// Created by danfergo on 03-01-2017.
//

#include <fstream>
#include <opencv/cv.hpp>
#include "Shape.h"
#include "Util.h"
#include "Image.h"

Shape::Shape(std::string identifier, std::string type, std::vector<Point3f> vertices) : identifier(identifier),
                                                                                        type(type), vertices(vertices) {

}

Shape Shape::importFromFile(std::string filename) {

    ifstream infile(("shapes/" + filename));

    vector<string> record;
    string s;
    while (getline(infile, s)) {
        record.push_back(s);
    }

    // read vertices coordinates from each line
    vector<Point3f> vertices;
    for (int i = 1; i < record.size(); i++) {
        vector<string> tmp = Util::split(record[i], ' ');

        istringstream buffer(tmp[0]);
        int x;
        buffer >> x;

        istringstream buffer2(tmp[1]);
        int y;
        buffer2 >> y;

        istringstream buffer3(tmp[2]);
        int z;
        buffer3 >> z;


        vertices.push_back(Point3f(x, y, z));
    }

    return Shape(Util::split(filename, '.')[0], record[0], vertices);
}

string Shape::getIdentifier() const {
    return identifier;
}

Shape::Shape() : identifier("") {

}

void
Shape::draw(Image &image, const Mat &rvec, const Mat &tvec, const Mat &intrinsicCoeffs, const Mat &distortionCoeffs) {
    if (type.length() >= 7 && type.substr(0,7).compare("pyramid") == 0 && vertices.size() == 5) {

        vector<Point3f> basePoints = {vertices[0],vertices[1],vertices[2],vertices[3]};
        vector<Point2f> baseProjection;

        projectPoints(basePoints, rvec,tvec, intrinsicCoeffs, distortionCoeffs, baseProjection);
        image.plotPolygon(baseProjection, Scalar(0,0,255));

        for(int i = 0 ; i < 4; i++){
            vector<Point3f> edgePoints = {vertices[i], vertices[4]};
            vector<Point2f> edgeProjection;
            projectPoints(edgePoints, rvec,tvec, intrinsicCoeffs, distortionCoeffs, edgeProjection);
            image.plotPolygon(edgeProjection, Scalar(0,0,255));
        }
    } else if (type.length() >= 4 && type.substr(0,4).compare("cube") == 0 && vertices.size() == 8) {

        for (int i = 0; i < 2; i++) {
            vector<Point3f> basePoints = {vertices[(i * 4) + 0], vertices[(i * 4) + 1], vertices[(i * 4) + 2],
                                          vertices[(i * 4) + 3]};
            vector<Point2f> baseProjection;

            projectPoints(basePoints, rvec, tvec, intrinsicCoeffs, distortionCoeffs, baseProjection);
            image.plotPolygon(baseProjection, Scalar(0, 255, 0));
        }


        for (int i = 0; i < 4; i++) {
            vector<Point3f> edgePoints = {vertices[i], vertices[4 + i]};
            vector<Point2f> edgeProjection;
            projectPoints(edgePoints, rvec, tvec, intrinsicCoeffs, distortionCoeffs, edgeProjection);
            image.plotPolygon(edgeProjection, Scalar(0, 255, 0));
        }


    }

}


