#define STB_IMAGE_IMPLEMENTATION // Must be defined before stb_image.h is included anywhere

/* 
    Optick Profiler config
    
    TOOD: 
        * Create VS Configuration 'Release_Profiling_Enabled'
        * Have USE_OPTICK == 1 for 'Debug' and 'Release_Profiling_Enabled'
        * Have USE_OPTICK == 0 for 'Release'
*/
#define USE_OPTICK (1)
#define OPTICK_ENABLE_TRACING (USE_OPTICK)
#define OPTICK_ENABLE_GPU (USE_OPTICK)

#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include "Shader/Shader.hpp"
#include "utils/fileUtils.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera/Camera.hpp"
#include "Model/Model.hpp"
#include "SceneObject/SceneObject.hpp"
#include "utils/array.h"
#include <vector>
#include "Mesh/Mesh.hpp"
#include "src/light/DirectionalLight/DirectionalLight.hpp"
#include "src/light/PointLight/PointLight.hpp"
#include "src/light/SpotLight/SpotLight.hpp"
#include "src/ScreenRenderer/ScreenRenderer.hpp"
#include "PostProcessEffect/PostProcessEffect.hpp"
#include "FrameBuffer/FrameBuffer.hpp"
#include "Skybox/Skybox.hpp"
#include "ShaderGlobals/ShaderGlobals.hpp"
#include "utils/random.hpp"
#include <optick.h>

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
    std::vector<SceneObject>& opaqueObjects,
    std::vector<SceneObject>& transparentObjects,
    std::vector<PointLight>& pointLights,
    DirectionalLight& dirLight,
    SpotLight& spotLight,
    Skybox& skybox
);

int windowWidth = 800;
int windowHeight = 600;
float lastX = (float)windowWidth / 2, lastY = (float) windowWidth / 2;
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
bool firstMouse = false;

Camera camera(glm::vec3(0.0f, 0.0f, 50.0f));

FrameBuffer* sceneFrameBuffer;
FrameBuffer* ppEffect1FrameBuffer;
FrameBuffer* ppEffect2FrameBuffer;

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

    glEnable(GL_CULL_FACE);

    auto phongShader = std::make_shared<Shader>(
        fileUtils::getFullResourcesPath("shaders/StandardPhong/StandardPhong.vert"),
        fileUtils::getFullResourcesPath("shaders/StandardPhong/StandardPhong.frag")
    );
    auto pointLightShader = std::make_shared<Shader>(
        fileUtils::getFullResourcesPath("shaders/LightSource/LightSource.vert"),
        fileUtils::getFullResourcesPath("shaders/LightSource/LightSource.frag")
    );

    auto unlitShader = std::make_shared<Shader>(
        fileUtils::getFullResourcesPath("shaders/StandardUnlit/StandardUnlit.vert"),
        fileUtils::getFullResourcesPath("shaders/StandardUnlit/StandardUnlit.frag")
    );

    auto backpackModel = std::make_shared<Model>(fileUtils::getFullResourcesPath("models/backpack/backpack.obj"));
    auto sphere = std::make_shared<Model>(fileUtils::getFullResourcesPath("models/primitives/sphere/sphere.obj"));

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
    std::vector<std::shared_ptr<Texture2D>> windowTextures = {
        Texture2D::Generate(fileUtils::getFullResourcesPath("textures/red-window.png"), aiTextureType::aiTextureType_DIFFUSE)
    };

    const int NUMB_OPAQUE_OBJECTS = 100;
    auto randomPositions = randomUtils::uniformRandomVec3(-12.0f, 12.0f, NUMB_OPAQUE_OBJECTS);
    std::vector<SceneObject> opaqueObjects;
    opaqueObjects.reserve(NUMB_OPAQUE_OBJECTS);
    for (auto& pos : randomPositions) {
        opaqueObjects.emplace_back(SceneObject(backpackModel, phongShader, pos));
    }

    /*std::vector<SceneObject> opaqueObjects = {
        SceneObject(backpackModel, phongShader, glm::vec3(0.0f,  2.0f,  0.0f)),
        SceneObject(backpackModel, phongShader, glm::vec3(0.0f, 0.0f, -8.0f)),
        SceneObject(backpackModel, phongShader, glm::vec3(0.0f, 5.0f, -8.0f)),
        SceneObject(backpackModel, phongShader, glm::vec3(3.0f, -2.0f, -8.0f)),
        SceneObject(backpackModel, phongShader, glm::vec3(5.0f, 0.0f, -1.0f)),
        SceneObject(sphere, pointLightShader,
            glm::vec3(0.7f,  0.2f,  20.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f)),
        SceneObject(sphere, pointLightShader,
            glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f)),
        SceneObject(sphere, pointLightShader,
            glm::vec3(-4.0f,  2.0f, -12.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f)),
        SceneObject(sphere, pointLightShader,
            glm::vec3(0.0f,  1.0f, 3.0f), glm::vec3(0.0,  0.0f,  0.0f), glm::vec3(0.2f))
    };*/

    auto windowModel = std::make_shared<Model>(std::vector<Mesh> { Mesh(windowVertices, windowIndices, windowTextures) });
    std::vector<SceneObject> transparentObjects = {
        SceneObject(windowModel, phongShader, glm::vec3(-1.5f, 0.0f, -0.48f)),
        SceneObject(windowModel, phongShader, glm::vec3(1.5f, 0.0f, 0.51f)),
        SceneObject(windowModel, phongShader, glm::vec3(0.0f, 0.0f, 0.7f)),
        SceneObject(windowModel, phongShader, glm::vec3(-0.3f, 0.0f, -2.3f)),
        SceneObject(windowModel, phongShader, glm::vec3(0.5f, 0.0f, -0.6f))
    };
 
    for (auto& obj : transparentObjects) {
        obj.mRotation = glm::vec3(90.0f, 0.0f, 0.0f);
    }

    auto cubemapFolder = fileUtils::getFullResourcesPath("skybox-textures/mountains-lake");
    std::vector<std::string> cubemapFaces = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };
    auto skyBox = Skybox(Cubemap(cubemapFolder, cubemapFaces));

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

    auto screenRenderer = ScreenRenderer();

    auto ppGrayscale = PostProcessEffect(fileUtils::getFullResourcesPath("shaders/post_processing/Grayscale.frag"));
    auto ppInvertColors = PostProcessEffect(fileUtils::getFullResourcesPath("shaders/post_processing/InvertColors.frag"));

    sceneFrameBuffer = new FrameBuffer(windowWidth, windowHeight, ColorType::Texture, DepthStencilType::DepthRbo);
    ppEffect1FrameBuffer = new FrameBuffer(windowWidth, windowHeight, ColorType::Texture, DepthStencilType::None);
    ppEffect2FrameBuffer = new FrameBuffer(windowWidth, windowHeight, ColorType::Texture, DepthStencilType::None);

    while (!glfwWindowShouldClose(window))
    {
        OPTICK_FRAME("Render loop");

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        sceneFrameBuffer->Bind();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        DrawScene(
            opaqueObjects,
            transparentObjects,
            pointLights, directionalLight, spotLight,
            skyBox);

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_BLEND);

        // With post processing
        /*ppEffect1FrameBuffer->Bind();
        screenRenderer.Draw(sceneFrameBuffer->GetColorTex(), &ppGrayscale);

        ppEffect2FrameBuffer->Bind();
        screenRenderer.Draw(ppEffect1FrameBuffer->GetColorTex(), &ppInvertColors);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        screenRenderer.Draw(ppEffect2FrameBuffer->GetColorTex());*/

        // No post processing
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        screenRenderer.Draw(sceneFrameBuffer->GetColorTex());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete sceneFrameBuffer;
    delete ppEffect1FrameBuffer;
    delete ppEffect2FrameBuffer;

    glfwTerminate();
    return 0;
}

void DrawScene(
    std::vector<SceneObject>& opaqueObjects, 
    std::vector<SceneObject>& transparentObjects,
    std::vector<PointLight>& pointLights,
    DirectionalLight& dirLight,
    SpotLight& spotLight,
    Skybox& skybox
) {
    ShaderGlobals::Get().mView.SetViewMatrix(camera.GetViewMatrix());
    ShaderGlobals::Get().mView.SetProjectionMatrix(
        glm::perspective(glm::radians(camera.GetFov()), (float)windowWidth / windowHeight, 0.1f, 100.0f));
    ShaderGlobals::Get().mView.SetViewPosWS(camera.Position);

    glm::mat4 viewMatrix = camera.GetViewMatrix();
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.GetFov()), (float)windowWidth / windowHeight, 0.1f, 100.0f);

    spotLight.positional.position = camera.Position;
    spotLight.direction = camera.GetFront();

    for (auto& obj : opaqueObjects) {
        obj.mShader->Use();

        obj.mShader->SetFloat("material.shininess", 32.0f);

        setLightProperties(*obj.mShader, pointLights, dirLight, spotLight);

        // For spot lights
        obj.mShader->SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

        obj.Draw();
    }

    std::map<float, SceneObject*, std::greater<float>> transparentObjSorted;
    for (auto& obj : transparentObjects) {
        float distance = glm::length(camera.Position - obj.mPosition);
        transparentObjSorted[distance] = &obj;
    }
    
    skybox.Draw(viewMatrix, projectionMatrix);

    glDisable(GL_CULL_FACE); // temporarily turns off culling since we're rendering quads here
    for (auto it = transparentObjSorted.begin(); it != transparentObjSorted.end(); it++) {
        auto obj = it->second;
        obj->mShader->Use();

        obj->mShader->SetFloat("material.shininess", 32.0f);

        setLightProperties(*obj->mShader, pointLights, dirLight, spotLight);

        obj->Draw();
    }
    glEnable(GL_CULL_FACE);
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
        shader.SetVec3(pointLightSlot + ".position", pointLights[i].positional.position);
        shader.SetVec3(pointLightSlot + ".ambient", pointLights[i].phong.amibent);
        shader.SetVec3(pointLightSlot + ".diffuse", pointLights[i].phong.diffuse);
        shader.SetVec3(pointLightSlot + ".specular", pointLights[i].phong.specular);
        shader.SetFloat(pointLightSlot + ".constant", pointLights[i].positional.constant);
        shader.SetFloat(pointLightSlot + ".linear", pointLights[i].positional.linear);
        shader.SetFloat(pointLightSlot + ".quadratic", pointLights[i].positional.quadratic);
    }

    shader.SetVec3("dirLight.direction", dirLight.direction);
    shader.SetVec3("dirLight.ambient", dirLight.phong.amibent);
    shader.SetVec3("dirLight.diffuse", dirLight.phong.diffuse);
    shader.SetVec3("dirLight.specular", dirLight.phong.specular);
    
    shader.SetVec3("spotLight.position", spotLight.positional.position);
    shader.SetVec3("spotLight.direction", spotLight.direction);
    shader.SetVec3("spotLight.ambient", spotLight.phong.amibent);
    shader.SetVec3("spotLight.diffuse", spotLight.phong.diffuse);
    shader.SetVec3("spotLight.specular", spotLight.phong.specular);
    shader.SetFloat("spotLight.constant", spotLight.positional.constant);
    shader.SetFloat("spotLight.linear", spotLight.positional.linear);
    shader.SetFloat("spotLight.quadratic", spotLight.positional.quadratic);
    shader.SetFloat("spotLight.cutOff", spotLight.innerCuttoff);
    shader.SetFloat("spotLight.outerCutOff", spotLight.outerCuttoff);
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

    sceneFrameBuffer->Resize(width, height);
    ppEffect1FrameBuffer->Resize(width, height);
    ppEffect2FrameBuffer->Resize(width, height);
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