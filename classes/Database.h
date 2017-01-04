//
// Created by danfergo on 03-01-2017.
//

#ifndef ARTOOL_DATABASE_H
#define ARTOOL_DATABASE_H


#include <map>
#include "MarkerDescriptor.h"
#include "../util/Shape.h"

class Database {

private:
public:
    vector<MarkerDescriptor> markers;
    vector<Shape> shapes;
    MarkerDescriptor findMarker(std::string identifier);
    Shape findShape(std::string identifier);
    vector<Shape> findAllShapesForMarker(MarkerDescriptor descriptor);
    void addAssociation(MarkerDescriptor markerDescriptor, Shape shape);


    MarkerDescriptor findSimilar(const MarkerDescriptor &descriptor, int & rotations);
};


#endif //ARTOOL_DATABASE_H
