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
#include "WorldObject/WorldObject.hpp"
#include "utils/array.h"
#include "effects/drawWithOutline/drawWithOutline.hpp"
#include <vector>
#include "Mesh/Mesh.hpp"

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

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_CULL_FACE);

    Shader unlitShader(
        fileUtils::getFullResourcesPath("shaders/StandardUnlit/StandardUnlit.vert").c_str(),
        fileUtils::getFullResourcesPath("shaders/StandardUnlit/StandardUnlit.frag").c_str()
    );
    Shader phongShader(
        fileUtils::getFullResourcesPath("shaders/StandardPhong/StandardPhong.vert").c_str(),
        fileUtils::getFullResourcesPath("shaders/StandardPhong/StandardPhong.frag").c_str()
    );
    Shader pointLightShader(
        fileUtils::getFullResourcesPath("shaders/LightSource/LightSource.vert").c_str(),
        fileUtils::getFullResourcesPath("shaders/LightSource/LightSource.frag").c_str()
    );

    auto backpackModel = Model(fileUtils::getFullResourcesPath("models/backpack/backpack.obj"));
    auto sphere = Model(fileUtils::getFullResourcesPath("models/primitives/sphere/sphere.obj"));

    std::vector<WorldObject> backpacks {
        WorldObject(backpackModel, phongShader, glm::vec3(0.0f,  2.0f,  0.0f)),
        WorldObject(backpackModel, phongShader, glm::vec3(0.0f, 0.0f, -8.0f))
    };

    WorldObject pointLights[] = {
        WorldObject(sphere, pointLightShader, 
            glm::vec3(0.7f,  0.2f,  20.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f)),
        WorldObject(sphere, pointLightShader, 
            glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f)),
        WorldObject(sphere, pointLightShader, 
            glm::vec3(-4.0f,  2.0f, -12.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f)),
        WorldObject(sphere, pointLightShader, 
            glm::vec3(0.0f,  1.0f, 3.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f))
    };

    std::vector<glm::vec3> windowVertPositions = {
        glm::vec3(-1.0f, 0.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 0.0f, -1.0f)
    };
    std::vector<glm::vec2> windowTexCoords = {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f)
    };
    std::vector<Vertex> windowVertices;
    for (int i = 0; i < windowVertPositions.size(); i++) {
        auto vert = Vertex();
        vert.position = windowVertPositions[i];
        vert.texCoords = windowTexCoords[i];
        vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
        windowVertices.push_back(vert);
    }
    std::vector<unsigned int> windowIndices = { 
        0, 1, 2,
        1, 3, 2
    };
    std::vector<Texture> windowTextures = {
        Texture(fileUtils::getFullResourcesPath("textures/red-window.png"), aiTextureType::aiTextureType_DIFFUSE)
    };
    std::vector<Mesh> windowMeshes = { Mesh(windowVertices, windowIndices, windowTextures) };
    auto windowModel = Model(windowMeshes);

    std::vector<WorldObject> transparentObjects = {
        WorldObject(windowModel, phongShader, glm::vec3(-1.5f, 0.0f, -0.48f)),
        WorldObject(windowModel, phongShader, glm::vec3(1.5f, 0.0f, 0.51f)),
        WorldObject(windowModel, phongShader, glm::vec3(0.0f, 0.0f, 0.7f)),
        WorldObject(windowModel, phongShader, glm::vec3(-0.3f, 0.0f, -2.3f)),
        WorldObject(windowModel, phongShader, glm::vec3(0.5f, 0.0f, -0.6f))
    };
 
    for (auto& obj : transparentObjects) {
        obj.mRotation = glm::vec3(90.0f, 0.0f, 0.0f);
    }

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        clearBuffers();

        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.GetFov()), (float)windowWidth / windowHeight, 0.1f, 100.0f);

        phongShader.use();

        phongShader.setMat4("view", viewMatrix);
        phongShader.setMat4("projection", projectionMatrix);

        phongShader.setVec3("viewPos", camera.Position);

        // Setting material properties
        phongShader.setFloat("material.shininess", 32.0f);

        // Directional light
        phongShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        phongShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        phongShader.setVec3("dirLight.diffuse", 0.8f, 0.8f, 0.8f);
        phongShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

        // point light 1
        phongShader.setVec3("pointLights[0].position", pointLights[0].mPosition);
        phongShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        phongShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        phongShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        phongShader.setFloat("pointLights[0].constant", 1.0f);
        phongShader.setFloat("pointLights[0].linear", 0.09);
        phongShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        phongShader.setVec3("pointLights[1].position", pointLights[1].mPosition);
        phongShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        phongShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        phongShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        phongShader.setFloat("pointLights[1].constant", 1.0f);
        phongShader.setFloat("pointLights[1].linear", 0.09);
        phongShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        phongShader.setVec3("pointLights[2].position", pointLights[2].mPosition);
        phongShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        phongShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        phongShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        phongShader.setFloat("pointLights[2].constant", 1.0f);
        phongShader.setFloat("pointLights[2].linear", 0.09);
        phongShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        phongShader.setVec3("pointLights[3].position", pointLights[3].mPosition);
        phongShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        phongShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        phongShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        phongShader.setFloat("pointLights[3].constant", 1.0f);
        phongShader.setFloat("pointLights[3].linear", 0.09);
        phongShader.setFloat("pointLights[3].quadratic", 0.032);

        // spotLight
        phongShader.setVec3("spotLight.position", camera.Position);
        phongShader.setVec3("spotLight.direction", camera.GetFront());
        phongShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        phongShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        phongShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        phongShader.setFloat("spotLight.constant", 1.0f);
        phongShader.setFloat("spotLight.linear", 0.09);
        phongShader.setFloat("spotLight.quadratic", 0.032);
        phongShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        phongShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        for (auto backpack: backpacks) {
            backpack.Draw();
        }

        // Drawing spot light spheres
        pointLightShader.use();
        pointLightShader.setMat4("projection", projectionMatrix);
        pointLightShader.setMat4("view", viewMatrix);
        pointLightShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        int numbLights = arrayUtils::length(pointLights);
        for (unsigned int i = 0; i < numbLights; i++)
        {
            pointLights[i].Draw();
        }

        std::map<float, WorldObject*, std::greater<float>> transparentObjSorted;
        for (auto& obj : transparentObjects) {
            float distance = glm::length(camera.Position - obj.mPosition);
            transparentObjSorted[distance] = &obj;
        }

        glDisable(GL_CULL_FACE); // temporarily turns culling of since we're rendering quads here
        for (auto& [key, val] : transparentObjSorted) {
            val->Draw();
        }
        glEnable(GL_CULL_FACE);

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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