//
// Created by danfergo on 02-01-2017.
//

#include "Camera.h"

Camera::Camera(int id): videoStream(id) {

}

Image Camera::nextFrame() {
    Mat mat;
    videoStream >> mat;
    return Image(mat);
}

Camera::~Camera() {
    videoStream.release();
}

void Camera::release() {
    videoStream.release();
}
