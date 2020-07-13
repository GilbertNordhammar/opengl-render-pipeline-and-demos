#include "Camera.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

Camera::Camera(
    glm::vec3 position, 
    glm::vec3 up, float yaw, 
    float pitch) : 
        _front(glm::vec3(0.0f, 0.0f, -1.0f)), 
        _movementSpeed(DEFAULT_MOVEMENT_SPEED), 
        _mouseSensitivity(DEFAULT_CAMERA_SENSITIVITY), 
        _fov(DEFAULT_FOV)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

Camera::Camera(
    float posX, 
    float posY, 
    float posZ, 
    float upX, 
    float upY, 
    float upZ, 
    float yaw, 
    float pitch) : 
        _front(glm::vec3(0.0f, 0.0f, -1.0f)),
        _movementSpeed(DEFAULT_MOVEMENT_SPEED), 
        _mouseSensitivity(DEFAULT_CAMERA_SENSITIVITY), 
        _fov(DEFAULT_FOV)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    UpdateCameraVectors();
}

void Camera::Move(CameraMovement direction, float deltaTime)
{
    float velocity = _movementSpeed * deltaTime;
    if (direction == CameraMovement::FORWARD)
        Position += _front * velocity;
    if (direction == CameraMovement::BACKWARD)
        Position -= _front * velocity;
    if (direction == CameraMovement::LEFT)
        Position -= _right * velocity;
    if (direction == CameraMovement::RIGHT)
        Position += _right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= _mouseSensitivity;
    yoffset *= _mouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;
    
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }
    
    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    _fov -= (float)yoffset;
    if (_fov < 1.0f)
        _fov = 1.0f;
    if (_fov > 45.0f)
        _fov = 45.0f;
}

void Camera::UpdateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    _front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    _right = glm::normalize(glm::cross(_front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    _up = glm::normalize(glm::cross(_right, _front));
}