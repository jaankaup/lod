#ifndef CAMERA_H
#define CAMERA_H

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
    public:
        Camera(const glm::vec3& cameraPosition, const glm::vec3& targetPosition, const glm::vec3& upVector);
        ~Camera();
        glm::mat4 getMatrix() const;
        void translate(const glm::vec3& posDelta);

    private:
        glm::vec3 position_; // missä on
        glm::vec3 target_; // minne katsoo
        glm::vec3 up_; // ylös päin vectori
};

#endif // CAMERA_H
