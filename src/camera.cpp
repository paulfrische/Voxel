// math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <math.h>

#include "math_util.hpp"

// (modern) OpenGL
#include <GL/glew.h>

// windowing
#include <GLFW/glfw3.h>

// Camera class
#include "camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, float speed, float mouse_sensitivity, float fov, GLFWwindow* window, unsigned int shader)
{
    this->position = position;
    this->front = front;
    this->up = up;
    this->speed = speed;
    this->mouse_sensitivity = mouse_sensitivity;
    this->fov = fov;

    viewMatrixLocation = glGetUniformLocation(shader, "view");
    projectionMatrixLocation = glGetUniformLocation(shader, "projection");

    this->viewMatrix = glm::lookAt(position, position + front, up);
    this->projectionMatrix = glm::perspective(glm::radians(fov), 1920.0f / 1080.0f, 0.1f, 100.0f);

    pitch = 0;
    yaw = 0;
}

void Camera::Update()
{
    this->viewMatrix = glm::lookAt(position, position + front, up);
    this->projectionMatrix = glm::perspective(glm::radians(fov), 1920.0f / 1080.0f, 0.1f, 100.0f);

    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Camera::processInput(GLFWwindow* window, float delta)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * speed * delta;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * speed * delta;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(front, up)) * speed * delta;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(front, up)) * speed * delta;
}

glm::vec3 Camera::direction()
{
    return util::eulerToVector(pitch, yaw);
}

void Camera::mouse_callback(double xpos, double ypos)
{
    float xoffset = xpos - lastMouseX;
    float yoffset = xpos - lastMouseY;

    lastMouseX = xpos;
    lastMouseY = ypos;

    yaw += xoffset * mouse_sensitivity;
    pitch += yoffset * mouse_sensitivity;

    pitch = util::clamp<float>(-89.0f, 89.0f, pitch);

    front = direction();
}