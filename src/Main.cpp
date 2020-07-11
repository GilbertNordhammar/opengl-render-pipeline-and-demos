#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include "Shader/Shader.hpp"
#include "utils/file.h"
#include "utils/stb.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void clearBuffers();
GLFWwindow* createWindow(int width, int height);
void executeRenderLoop(GLFWwindow* window, Shader shader, unsigned int VBO);
unsigned int setVertexInput
(
    float vertices[],
    unsigned int verticesInBytes,
    unsigned int indices[],
    unsigned int indicesInBytes,
    float texCoords[],
    unsigned int texCoordsInBytes
);

int main() {
    glfwInit();
    
    const int windowWidth = 800;
    const int windowHeight = 600;

    GLFWwindow* window = createWindow(windowWidth, windowHeight);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader(fileUtils::getFullPath("shaders/TestShader/TestShader.vert").c_str(),
                     fileUtils::getFullPath("shaders/TestShader/TestShader.frag").c_str());

    float vertices[] = {
        // positions         // colors
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
    };
    auto VAO = setVertexInput(vertices, sizeof(vertices), indices, sizeof(indices), texCoords, sizeof(texCoords));

    executeRenderLoop(window, ourShader, VAO);

    glfwTerminate();
    return 0;
}


void executeRenderLoop(GLFWwindow* window, Shader shader, unsigned int VAO) {
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        clearBuffers();

        shader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // TODO: makes number of indices not hardcoded

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

unsigned int setVertexInput
(
    float vertices[],
    unsigned int verticesInBytes,
    unsigned int indices[],
    unsigned int indicesInBytes,
    float texCoords[],
    unsigned int texCoordsInBytes
) {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesInBytes, vertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesInBytes, indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
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
    glClear(GL_COLOR_BUFFER_BIT);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}