//
// Created by danfergo on 02-01-2017.
//

#ifndef ARTOOL_CAMERA_H
#define ARTOOL_CAMERA_H

#include <opencv/cv.h>
#include <opencv2/videoio.hpp>
#include "Image.h"

using namespace cv;

class Camera {

private:
    VideoCapture videoStream;

public:
    Camera(int id);
    ~Camera();
    void release();
    Image nextFrame();
};


#endif //ARTOOL_CAMERA_H
