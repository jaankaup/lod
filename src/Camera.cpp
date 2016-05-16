#include "Camera.h"


// Konstruktori
Camera::Camera(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& upVector)
{

    defaultPosition_ = cameraPosition;
    position_ = cameraPosition;
    target_ = cameraTarget;
    up_ = upVector;

    rotateCamera(0,0);
}

// Destruktori
Camera::~Camera()
{
}


/**
 * Palauttaa kameran matriisin
 * @return Matrix
 */
glm::mat4 Camera::getMatrix() const
{
    return view;
}

/**
 * Palauttaa kameran sijainnin
 * @return position
 */
glm::vec3 Camera::getPosition() const
{
    return position_;
}


/**
 * Asettaa kameran katsomaan annettua kohdetta
 * @param cameraTarget kameran kohdevektori
 */
void Camera::setView(const glm::vec3& cameraTarget)
{
    view = glm::lookAt(position_, cameraTarget, up_);
}


/**
 * Palauttaa kameran sijainnin alkuarvoihin
 */
void Camera::resetView()
{
    position_ = defaultPosition_;
    pitch     =  -45.0;
    yaw       =  -90.0;
    rotateCamera(0,0);
}


/**
 * Vaihtaa kameratyyppi�
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


/***
 * Kameran suunnan muutos hiirikoordinaattien perusteella.
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


/**
 * Muuttaa kameran liikkumisnopeutta parametrin verran.
 * Minimi 1.0, maksimi 10.0
 * @param adjust muutos
 */
void Camera::adjustSpeed(const float &adjust)
{
    camSpeed += adjust;
    camSpeed = glm::clamp(camSpeed, 1.0f, 10.0f);
    //cout << "Camera speed: " << camSpeed << endl;
    logInfo.log("Camera speed: %", camSpeed);
}


/**
 * Muuttaa kameran k��ntymisnopeutta parametrin verran.
 * Minimi 0.1, maksimi 1.0
 * @param adjust muutos
 */
void Camera::adjustSensitivity(const float &adjust)
{
    camSensitivity += adjust;
    camSensitivity = glm::clamp(camSensitivity, 0.1f, 1.0f);
    //cout << "Camera sensitivity: "  << camSensitivity << endl;
    logInfo.log("Camera sensitivity: %", camSensitivity);
}


/**
 * Kameran p�ivitys
 * @param time aikakerroin, (currentTick / lastTick), v�litet��n program-luokasta
 */
void Camera::update(const float &time)
{
    deltaTime = time;
    movementInterp = (SDL_GetTicks() * 0.0005f) * deltaTime;

    if (camMode == CameraMode::Free)
    {
        // WASD-kamera
        translate(position_);
        setView(position_ + front_);
    }
    else if (camMode == CameraMode::Floating)
    {
        // Liikuttelee kameraa edestakaisin
        float coordX = 80.0f + 80.0f * sin(movementInterp); // Vaihteluv�li 0-160
        float coordY = 80.0f + 40.0f * cos(movementInterp); // Vaihteluv�li 40-120
        float coordZ = 80.0f + 80.0f * sin(movementInterp) * cos(movementInterp); // Vaihteluv�li 40-120
        translate(glm::vec3(coordX, coordY, coordZ));
        setView(target_);
    }
    else if (camMode == CameraMode::Orbit)
    {
        // Py�ritt�� kameraa kohteen ymp�ri
        float coordX = 0.0f + 160.0f * sin(movementInterp);
        float coordY = 120.0f; // Korkeus
        float coordZ = 0.0f + 160.0f * cos(movementInterp);
        translate(glm::vec3(coordX, coordY, coordZ));
        setView(target_);
    }

    handleKeyInput();
}


/**
 * N�pp�imist�tapahtumien k�sittely
 */
void Camera::handleKeyInput()
{
    /* Pyydet��n SDL:n n�pp�intila */
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    /* Shift hidastaa liikkumisnopeutta */
    float speedMultiplier = camSpeed;
    if(keystate[SDL_SCANCODE_LSHIFT])
        speedMultiplier *= 0.1f;

    /* WASD-n�pp�imet */
    if(keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
        position_ += front_ * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
        position_ -= front_ * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
        position_ -= glm::normalize(glm::cross(front_, up_)) * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
        position_ += glm::normalize(glm::cross(front_, up_)) * speedMultiplier * deltaTime;

    /* Yl�s ja alas liikkuminen */
    if(keystate[SDL_SCANCODE_V])
        position_ += glm::normalize(up_) * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_C])
        position_ -= glm::normalize(up_) * speedMultiplier * deltaTime;
}


/**
 * Hiiritapahtumien k�sittely. Program v�litt�� tapahtumat.
 */
void Camera::handleMouseInput(SDL_Event &inputEvent)
{
    switch (inputEvent.type)
    {
    case (SDL_MOUSEBUTTONDOWN): /* Klikkaus. Sijainti talteen sulavampaa liikett� varten */
        lastMouseX = inputEvent.motion.x;
        lastMouseY = inputEvent.motion.y;
    case (SDL_MOUSEMOTION):
        switch (inputEvent.motion.state)
        {
        case (SDL_BUTTON_LMASK): /* Hiiren vasen nappi pohjassa */
            rotateCamera((inputEvent.motion.x - lastMouseX) * camSensitivity * deltaTime,
                         (lastMouseY - inputEvent.motion.y) * camSensitivity * deltaTime);
            /* Hiiren sijainti talteen muutoksen laskemista varten */
            lastMouseX = inputEvent.motion.x;
            lastMouseY = inputEvent.motion.y;
            break;
        }
    }
}
