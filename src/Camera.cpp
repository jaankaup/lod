#include "global.h"
#include "Camera.h"
#include <iostream>
#include <cmath>
using std::cout;
using std::endl;

Camera::Camera(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& upVector)
{
    defaultPosition_ = cameraPosition;
    position_ = cameraPosition;
    target_ = cameraTarget;
    up_ = upVector;

    rotateCamera(0,0);
}

Camera::~Camera() {}


/**
 * Palauttaa kameran matriisin
 * @return Matrix
 */
glm::mat4 Camera::getMatrix() const
{
    return view;
}


/**
 * Asettaa kameran katsomaan annettua kohdetta
 */
void Camera::setView(const glm::vec3& cameraTarget)
{
    view = glm::lookAt(position_, cameraTarget, up_);
}

void Camera::resetView()
{
    position_ = defaultPosition_;
    pitch     =  -45.0;
    yaw       =  -90.0;
    rotateCamera(0,0);
}


/**
 * Vaihtaa kameratyyppiä
 */
void Camera::toggleMode()
{
    switch (camMode)
    {
        case (CameraMode::Free): camMode = CameraMode::Orbit; break;
        case (CameraMode::Orbit): camMode = CameraMode::Floating; break;
        case (CameraMode::Floating): camMode = CameraMode::Free; resetView(); break;
    }
}


/**
 * Kameran sijainnin muutos
 * @param newPos uusi sijainti
 */
void Camera::translate(const glm::vec3& newPos)
{
    position_ = newPos;
}


/**
 * Kameran suunnan muutos window-koordinaattien perusteella
 * @param xoffset x-muutos
 * @param xoffset y-muutos
 */
void Camera::rotateCamera(const float &xoffset, const float &yoffset)
{
    yaw   += xoffset;
    pitch += yoffset;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    front_.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front_.y = sin(glm::radians(pitch));
    front_.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front_ = glm::normalize(front_);
}


void Camera::adjustSpeed(const float &adjust)
{
    camSpeed += adjust;
    camSpeed = glm::clamp(camSpeed, 1.0f, 15.0f);
    //cout << "Camera speed: " << camSpeed << endl;
    logDebug.log("Camera speed: %", camSpeed);
}

void Camera::adjustSensitivity(const float &adjust)
{
    camSensitivity += adjust;
    camSensitivity = glm::clamp(camSensitivity, 0.1f, 1.0f);
    //cout << "Camera sensitivity: "  << camSensitivity << endl;
    logDebug.log("Camera sensitivity: %", camSensitivity);
}


/**
 * Kameran tilanteen päivitys
 * @param time ajanhetki, välitetään program-luokasta
 */
void Camera::update(const float &time)
{
    deltaTime = time;

    // print frametime
    //cout << (SDL_GetTicks() - (float)lastFrameTime) << " - ";
    //lastFrameTime = SDL_GetTicks();

    // Väliaikainen korjaus
    lastFrameTime = (SDL_GetTicks() / 2000.0f) * deltaTime;

    if (camMode == CameraMode::Free)
    {
        translate(position_);
        setView(position_ + front_);
    }
    else if (camMode == CameraMode::Floating)
    {
        // HUOM! GetTicks
        float ranX = 80.0f + 60.0f * sin(lastFrameTime); // Vaihteluväli 20-140
        float ranY = 80.0f + 60.0f * cos(lastFrameTime); // Vaihteluväli 20-140
        float ranZ = 80.0f + 60.0f * sin(lastFrameTime) * cos(lastFrameTime); // Vaihteluväli 50-110
        translate(glm::vec3(ranX,ranY,ranZ));
        setView(target_);
    }
    else if (camMode == CameraMode::Orbit)
    {
        // HUOM! GetTicks
        float ranX = 0.0f + 180.0f * sin(lastFrameTime);
        float ranZ = 0.0f + 180.0f * cos(lastFrameTime);
        translate(glm::vec3(ranX,120.0f,ranZ));
        setView(target_);
    }

    handleKeyInput();
}


/**
 * Näppäimistötapahtumien käsittely
 */
void Camera::handleKeyInput()
{
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    if(keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
    {
        if(keystate[SDL_SCANCODE_LSHIFT])
            position_ += front_ * (camSpeed * 0.1f) * deltaTime;
        else
            position_ += front_ * camSpeed * deltaTime;
    }
    if(keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
    {
        if(keystate[SDL_SCANCODE_LSHIFT])
            position_ -= front_ * (camSpeed * 0.1f) * deltaTime;
        else
            position_ -= front_ * camSpeed * deltaTime;
    }
    if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
    {
        if(keystate[SDL_SCANCODE_LSHIFT])
            position_ -= glm::normalize(glm::cross(front_, up_)) * (camSpeed * 0.1f) * deltaTime;
        else
            position_ -= glm::normalize(glm::cross(front_, up_)) * camSpeed * deltaTime;
    }
    if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
    {
        if(keystate[SDL_SCANCODE_LSHIFT])
            position_ += glm::normalize(glm::cross(front_, up_)) * (camSpeed * 0.1f) * deltaTime;
        else
            position_ += glm::normalize(glm::cross(front_, up_)) * camSpeed * deltaTime;
    }
    if(keystate[SDL_SCANCODE_V] || keystate[SDL_SCANCODE_D])
    {
        if(keystate[SDL_SCANCODE_LSHIFT])
            position_ += glm::normalize(up_) * (camSpeed * 0.1f) * deltaTime;
        else
            position_ += glm::normalize(up_) * camSpeed * deltaTime;
    }
    if(keystate[SDL_SCANCODE_C] || keystate[SDL_SCANCODE_D])
    {
        if(keystate[SDL_SCANCODE_LSHIFT])
            position_ -= glm::normalize(up_) * (camSpeed * 0.1f) * deltaTime;
        else
            position_ -= glm::normalize(up_) * camSpeed * deltaTime;
    }
}


/**
 * Hiiritapahtumien käsittely. Program välittää tapahtumat.
 */
void Camera::handleMouseInput(SDL_Event &inputEvent)
{
    switch (inputEvent.type)
    {
    case (SDL_MOUSEBUTTONDOWN): /* Klikkaus. Sijainti talteen sulavampaa liikettä varten. */
        lastMouseX = inputEvent.motion.x;
        lastMouseY = inputEvent.motion.y;
    case (SDL_MOUSEMOTION):
        switch (inputEvent.motion.state)
        {
        case (SDL_BUTTON_LMASK): /* Vasemman hiiren napin vetäminen */
            rotateCamera((inputEvent.motion.x - lastMouseX) * camSensitivity * deltaTime,
                         (lastMouseY - inputEvent.motion.y) * camSensitivity * deltaTime);
            // Sijainti talteen
            lastMouseX = inputEvent.motion.x;
            lastMouseY = inputEvent.motion.y;
            break;
        }
    }
}
