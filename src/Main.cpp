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
#include "src/light/DirectionalLight/DirectionalLight.hpp"
#include "src/light/PointLight/PointLight.hpp"
#include "src/light/SpotLight/SpotLight.hpp"
#include "src/ScreenRenderer/ScreenRenderer.hpp"
#include "PostProcessEffect/PostProcessEffect.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* createWindow(int width, int height);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void setLightProperties(
    Shader& shader,
    std::vector<PointLight> pointLights,
    DirectionalLight dirLight,
    SpotLight spotLight
);
void DrawScene(
    std::vector<WorldObject> opaqueObjects,
    std::vector<WorldObject> transparentObjects,
    std::vector<PointLight> pointLights,
    DirectionalLight dirLight,
    SpotLight spotLight
);
void createFramebuffer(unsigned int& OUT_fbo, unsigned int& OUT_textureColorBuffer, bool useDepthAndStencil);

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
    glEnable(GL_CULL_FACE);

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

    std::vector<WorldObject> opaqueObjects = {
        WorldObject(backpackModel, phongShader, glm::vec3(0.0f,  2.0f,  0.0f)),
        WorldObject(backpackModel, phongShader, glm::vec3(0.0f, 0.0f, -8.0f)),
        WorldObject(sphere, pointLightShader,
            glm::vec3(0.7f,  0.2f,  20.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f)),
        WorldObject(sphere, pointLightShader,
            glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f)),
        WorldObject(sphere, pointLightShader,
            glm::vec3(-4.0f,  2.0f, -12.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f)),
        WorldObject(sphere, pointLightShader,
            glm::vec3(0.0f,  1.0f, 3.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f))
    };

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

    DirectionalLight directionalLight = DirectionalLight();
    directionalLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    directionalLight.phong.amibent = glm::vec3(0.05f, 0.05f, 0.05f);
    directionalLight.phong.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    directionalLight.phong.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    
    SpotLight spotLight = SpotLight();
    spotLight.innerCuttoff = glm::cos(glm::radians(12.5f));
    spotLight.outerCuttoff = glm::cos(glm::radians(15.0f));
    spotLight.phong.amibent = glm::vec3(0.0f, 0.0f, 0.0f);
    spotLight.phong.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLight.phong.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    spotLight.positional.constant = 1.0f;
    spotLight.positional.linear = 0.09f;
    spotLight.positional.quadratic = 0.032f;

    std::vector<PointLight> pointLights;
    for (int i = 0; i < 4; i++) {
        PointLight light;
        light.phong.amibent = glm::vec3(0.05f, 0.05f, 0.05f);
        light.phong.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
        light.phong.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        light.positional.constant = 1.0f;
        light.positional.linear = 0.09f;
        light.positional.quadratic = 0.032f;
        pointLights.push_back(light);
    }
    pointLights[0].positional.position = glm::vec3(0.7f, 0.2f, 20.0f);
    pointLights[1].positional.position = glm::vec3(2.3f, -3.3f, -4.0f);
    pointLights[2].positional.position = glm::vec3(-4.0f, 2.0f, -12.0f);
    pointLights[3].positional.position = glm::vec3(0.0f, 1.0f, 3.0f);

    unsigned int fboScene, texColBufferScene;
    createFramebuffer(fboScene, texColBufferScene, true);
   
    unsigned int fboPostProcess1, texColBufferPostProcess1;
    createFramebuffer(fboPostProcess1, texColBufferPostProcess1, false);

    unsigned int fboPostProcess2, texColBufferPostProcess2;
    createFramebuffer(fboPostProcess2, texColBufferPostProcess2, false);

    auto screenRenderer = ScreenRenderer();

    auto ppGrayscale = PostProcessEffect(fileUtils::getFullResourcesPath("shaders/post_processing/Grayscale.frag").c_str());
    auto ppInvertColors = PostProcessEffect(fileUtils::getFullResourcesPath("shaders/post_processing/InvertColors.frag").c_str());

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glBindFramebuffer(GL_FRAMEBUFFER, fboScene);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        DrawScene(
            opaqueObjects, 
            transparentObjects, 
            pointLights, directionalLight, spotLight);
        
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);

        glBindFramebuffer(GL_FRAMEBUFFER, fboPostProcess1);
        screenRenderer.Draw(texColBufferScene, &ppGrayscale);

        glBindFramebuffer(GL_FRAMEBUFFER, fboPostProcess2);
        screenRenderer.Draw(texColBufferPostProcess1, &ppInvertColors);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        screenRenderer.Draw(texColBufferPostProcess2);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteFramebuffers(1, &fboScene);

    glfwTerminate();
    return 0;
}

void DrawScene(
    std::vector<WorldObject> opaqueObjects, 
    std::vector<WorldObject> transparentObjects,
    std::vector<PointLight> pointLights,
    DirectionalLight dirLight,
    SpotLight spotLight
) {
    glm::mat4 viewMatrix = camera.GetViewMatrix();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.GetFov()), (float)windowWidth / windowHeight, 0.1f, 100.0f);

    spotLight.positional.position = camera.Position;
    spotLight.direction = camera.GetFront();

    for (auto& obj : opaqueObjects) {
        obj.mShader.use();

        obj.mShader.setMat4("view", viewMatrix);
        obj.mShader.setMat4("projection", projectionMatrix);

        obj.mShader.setVec3("viewPos", camera.Position);
        obj.mShader.setFloat("material.shininess", 32.0f);

        setLightProperties(obj.mShader, pointLights, dirLight, spotLight);

        // For spot lights
        obj.mShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        obj.Draw();
    }

    std::map<float, WorldObject*, std::greater<float>> transparentObjSorted;
    for (auto& obj : transparentObjects) {
        float distance = glm::length(camera.Position - obj.mPosition);
        transparentObjSorted[distance] = &obj;
    }

    glDisable(GL_CULL_FACE); // temporarily turns culling of since we're rendering quads here
    for (auto& [key, obj] : transparentObjSorted) {
        obj->mShader.use();

        obj->mShader.setMat4("view", viewMatrix);
        obj->mShader.setMat4("projection", projectionMatrix);

        obj->mShader.setVec3("viewPos", camera.Position);
        obj->mShader.setFloat("material.shininess", 32.0f);

        setLightProperties(obj->mShader, pointLights, dirLight, spotLight);

        obj->Draw();
    }
    glEnable(GL_CULL_FACE);
}

void createFramebuffer(unsigned int& OUT_fbo, unsigned int& OUT_textureColorBuffer, bool useDepthAndStencil) {
    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    // create a color attachment texture
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
    
    if (useDepthAndStencil) {
        unsigned int rboDepthStencil;
        glGenRenderbuffers(1, &rboDepthStencil);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil); // now actually attach it
    }

    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    OUT_fbo = fbo;
    OUT_textureColorBuffer = textureColorbuffer;
}

void setLightProperties(
    Shader& shader, 
    std::vector<PointLight> pointLights, 
    DirectionalLight dirLight, 
    SpotLight spotLight
    ) 
{
    for (int i = 0; i < pointLights.size(); i++) {
        std::string pointLightSlot = "pointLights[" + std::to_string(i) + "]";
        shader.setVec3(pointLightSlot + ".position", pointLights[i].positional.position);
        shader.setVec3(pointLightSlot + ".ambient", pointLights[i].phong.amibent);
        shader.setVec3(pointLightSlot + ".diffuse", pointLights[i].phong.diffuse);
        shader.setVec3(pointLightSlot + ".specular", pointLights[i].phong.specular);
        shader.setFloat(pointLightSlot + ".constant", pointLights[i].positional.constant);
        shader.setFloat(pointLightSlot + ".linear", pointLights[i].positional.linear);
        shader.setFloat(pointLightSlot + ".quadratic", pointLights[i].positional.quadratic);
    }

    shader.setVec3("dirLight.direction", dirLight.direction);
    shader.setVec3("dirLight.ambient", dirLight.phong.amibent);
    shader.setVec3("dirLight.diffuse", dirLight.phong.diffuse);
    shader.setVec3("dirLight.specular", dirLight.phong.specular);
    
    shader.setVec3("spotLight.position", spotLight.positional.position);
    shader.setVec3("spotLight.direction", spotLight.direction);
    shader.setVec3("spotLight.ambient", spotLight.phong.amibent);
    shader.setVec3("spotLight.diffuse", spotLight.phong.diffuse);
    shader.setVec3("spotLight.specular", spotLight.phong.specular);
    shader.setFloat("spotLight.constant", spotLight.positional.constant);
    shader.setFloat("spotLight.linear", spotLight.positional.linear);
    shader.setFloat("spotLight.quadratic", spotLight.positional.quadratic);
    shader.setFloat("spotLight.cutOff", spotLight.innerCuttoff);
    shader.setFloat("spotLight.outerCutOff", spotLight.outerCuttoff);
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