//
// Created by danfergo on 03-01-2017.
//

#include "Database.h"

MarkerDescriptor Database::findMarker(std::string identifier) {
    for (std::vector<MarkerDescriptor>::iterator it = markers.begin(); it != markers.end(); ++it) {
        if (it->getIdentifier() == identifier) {
            return *it;
        }
    }
    MarkerDescriptor marker;
    return marker;
}


Shape Database::findShape(std::string identifier) {

    for (std::vector<Shape>::iterator it = shapes.begin(); it != shapes.end(); it++) {
        if (it->getIdentifier() == identifier) {
            return *it;
        }
    }

    Shape shape;
    return shape;
}

void Database::addAssociation(MarkerDescriptor markerDescriptor, Shape shape) {
    markers.push_back(markerDescriptor);
    shapes.push_back(shape);
}

MarkerDescriptor Database::findSimilar(const MarkerDescriptor &descriptor, int & rotations) {

    for (std::vector<MarkerDescriptor>::iterator it = markers.begin(); it != markers.end(); ++it) {
       if((*it).matches(descriptor,rotations)){
           return *it;

       };
    }
    MarkerDescriptor marker;
    return marker;
}

vector<Shape> Database::findAllShapesForMarker(MarkerDescriptor descriptor) {
    vector<Shape> returnVector;

    for (std::vector<MarkerDescriptor>::iterator it = markers.begin(); it != markers.end(); ++it) {
        if (it->getIdentifier() == descriptor.getIdentifier()) {
            returnVector.push_back(shapes[it - markers.begin()]);
        }
    }

    return returnVector;
}


