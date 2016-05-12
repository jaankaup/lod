#ifndef CAMERA_H
#define CAMERA_H

#include <SDL2/sdl.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
    public:
        Camera(const glm::vec3& cameraPosition, const glm::vec3& targetPosition, const glm::vec3& upVector);
        ~Camera();
        glm::mat4 getMatrix() const;

        void translate(const glm::vec3& newPos);
        void rotateCamera(const float &xoffset, const float &yoffset);
        void update(const float &time);
        void handleKeyInput();
        void handleMouseInput(SDL_Event &mouseEvent);

        void setView(const glm::vec3& cameraTarget);
        void resetView();
        void toggleMode();
        void adjustSpeed(const float &adjust);
        void adjustSensitivity(const float &adjust);

    private:
        glm::mat4 view;

        glm::vec3 defaultPosition_;
        glm::vec3 position_;
        glm::vec3 front_ = glm::vec3(0.0f, 0.0f,  -1.0f);
        glm::vec3 up_    = glm::vec3(0.0f, 1.0f,  0.0f);
        glm::vec3 target_; // Kameran peruskohde

        float lastFrameTime = 0.0f;
        float deltaTime = 0.0f;

        float pitch = -45.0f;
        float yaw   = -90.0f;
        float lastMouseX = 0.0f;
        float lastMouseY = 0.0f;
        float camSpeed = 5.0;
        float camSensitivity = 0.6;

        enum class CameraMode { Free, Orbit, Floating };
        CameraMode camMode = CameraMode::Free;
};

#endif // CAMERA_H
