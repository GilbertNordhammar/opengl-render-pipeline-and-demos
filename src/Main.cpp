#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include "Shader/Shader.hpp"
#include "utils/fileUtils.h"
#include <stb.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera/Camera.hpp"
#include "Model/Model.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void clearBuffers();
GLFWwindow* createWindow(int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

int windowWidth = 800;
int windowHeight = 600;
float lastX = (float)windowWidth / 2, lastY = (float) windowWidth / 2;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
bool firstMouse = false;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main() {
    glfwInit();
    
    GLFWwindow* window = createWindow(windowWidth, windowHeight);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader lightSourceShader(
        fileUtils::getFullResourcesPath("shaders/LightSource/LightSource.vert").c_str(),
        fileUtils::getFullResourcesPath("shaders/LightSource/LightSource.frag").c_str()
    );

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  20.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  1.0f, 3.0f)
    };

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    Shader ourModelShader(
        fileUtils::getFullResourcesPath("shaders/StandardPhong/StandardPhong.vert").c_str(),
        fileUtils::getFullResourcesPath("shaders/StandardPhong/StandardPhong.frag").c_str()
    );
    auto ourModel = Model(fileUtils::getFullResourcesPath("models/backpack/backpack.obj"));
    
    Shader pointLightShader(
        fileUtils::getFullResourcesPath("shaders/LightSource/LightSource.vert").c_str(),
        fileUtils::getFullResourcesPath("shaders/LightSource/LightSource.frag").c_str()
    );
    auto pointLight = Model(fileUtils::getFullResourcesPath("models/primitives/sphere/sphere.obj"));

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        clearBuffers();

        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.GetFov()), (float)windowWidth / windowHeight, 0.1f, 100.0f);

        ourModelShader.use();

        ourModelShader.setMat4("view", viewMatrix);
        ourModelShader.setMat4("projection", projectionMatrix);

        ourModelShader.setVec3("viewPos", camera.Position);

        // Setting material properties
        ourModelShader.setFloat("material.shininess", 32.0f);

        // Directional light
        ourModelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        ourModelShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        ourModelShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        ourModelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

        // point light 1
        ourModelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        ourModelShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        ourModelShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        ourModelShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourModelShader.setFloat("pointLights[0].constant", 1.0f);
        ourModelShader.setFloat("pointLights[0].linear", 0.09);
        ourModelShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        ourModelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        ourModelShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        ourModelShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        ourModelShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        ourModelShader.setFloat("pointLights[1].constant", 1.0f);
        ourModelShader.setFloat("pointLights[1].linear", 0.09);
        ourModelShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        ourModelShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        ourModelShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        ourModelShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        ourModelShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        ourModelShader.setFloat("pointLights[2].constant", 1.0f);
        ourModelShader.setFloat("pointLights[2].linear", 0.09);
        ourModelShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        ourModelShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        ourModelShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        ourModelShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        ourModelShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        ourModelShader.setFloat("pointLights[3].constant", 1.0f);
        ourModelShader.setFloat("pointLights[3].linear", 0.09);
        ourModelShader.setFloat("pointLights[3].quadratic", 0.032);

        // spotLight
        ourModelShader.setVec3("spotLight.position", camera.Position);
        ourModelShader.setVec3("spotLight.direction", camera.GetFront());
        ourModelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        ourModelShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        ourModelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        ourModelShader.setFloat("spotLight.constant", 1.0f);
        ourModelShader.setFloat("spotLight.linear", 0.09);
        ourModelShader.setFloat("spotLight.quadratic", 0.032);
        ourModelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        ourModelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourModelShader.setMat4("model", modelMatrix);

        ourModel.Draw(ourModelShader);

        // Drawing spot light cubes
        lightSourceShader.use();
        lightSourceShader.setMat4("projection", projectionMatrix);
        lightSourceShader.setMat4("view", viewMatrix);
        lightSourceShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, pointLightPositions[i]);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
            lightSourceShader.setMat4("model", modelMatrix);
            pointLight.Draw(lightSourceShader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

GLFWwindow* createWindow(int width, int height) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    return glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
}

void clearBuffers() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.Move(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.Move(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.Move(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.Move(CameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.Move(CameraMovement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.Move(CameraMovement::DOWN, deltaTime);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.Zoom(yoffset);
}