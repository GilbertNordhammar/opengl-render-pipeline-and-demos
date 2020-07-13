#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default camera values
const float DEFAULT_YAW = -90.0f;
const float DEFAULT_PITCH =  0.0f;
const float DEFAULT_MOVEMENT_SPEED = 2.5f;
const float DEFAULT_CAMERA_SENSITIVITY = 0.1f;
const float DEFAULT_FOV = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 WorldUp;
    float Yaw;
    float Pitch;
    
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
        float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH);
    Camera(
        float posX, 
        float posY, 
        float posZ, 
        float upX, 
        float upY, 
        float upZ, 
        float yaw, 
        float pitch);
    
    void Move(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void Zoom(float yoffset);
    
    void SetFov(float value) { _fov = std::fmax(0, std::fmin(90, value)); }
    float GetFov() { return _fov; }

    void SetMovementSpeed(float value) { _movementSpeed = std::fmax(0, value); }
    float GetMovementSpeed() { return _movementSpeed; }

    void SetMouseSensitivity(float value) { _mouseSensitivity = std::fmax(0, value); }
    float GetMouseSensitivity() { return _mouseSensitivity; }

    glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + _front, _up); }
    glm::vec3 GetFront() { return _front; }
    glm::vec3 GetUp() { return _up; }
    glm::vec3 GetRight() { return _right; }

private:
    float _fov;
    glm::vec3 _front;
    glm::vec3 _up;
    glm::vec3 _right;
    float _movementSpeed;
    float _mouseSensitivity;
    void UpdateCameraVectors();
};