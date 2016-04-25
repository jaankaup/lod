#include "global.h"
#include "Camera.h"

Camera::Camera(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& upVector)
{
    position_ = cameraPosition;
    target_ = cameraTarget;
    up_ = upVector;
}

Camera::~Camera()
{
    //dtor
	//Juha oli Taalla
}

glm::mat4 Camera::getMatrix() const
{
    return glm::lookAt(position_, // the position of your camera, in world space
                       target_,   // where you want to look at, in world space
                       up_);      // probably glm::vec3(0,1,0), but (0,-1,0) would make you looking upside-down, which can be great
}

void Camera::translate(const glm::vec3& posDelta)
{
    position_ = position_ + posDelta;
    // logInfo.log("(%,%,%)", position_.x, position_.y,position_.z);
}
