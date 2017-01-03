//
// Created by danfergo on 03-01-2017.
//

#include <fstream>
#include "Shape.h"
#include "Util.h"

Shape::Shape(std::string identifier, std::string type, std::vector<Point3f> vertices) : identifier(identifier), type(type), vertices(vertices) {

}

Shape Shape::importFromFile(std::string filename) {

    ifstream infile;
    infile.open(("shapes/" + filename).c_str());


    // read file line by line
    vector<string> record;
    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);


        while (ss) {
            string s;
            if (!getline(ss, s)) break;
            record.push_back(s);
            istringstream ss(s);
        }
    }
    if (!infile.eof()) {
        cerr << "Fooey!\n";
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



        vertices.push_back(Point3f(x,y,z));
    }

    return Shape(Util::split(filename, '.')[0], record[0], vertices);
}

string Shape::getIdentifier() const {
    return identifier;
}

Shape::Shape(): identifier(""){

}

void Shape::draw() {

}


