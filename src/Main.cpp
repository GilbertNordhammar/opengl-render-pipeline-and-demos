#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void clearBuffers();
GLFWwindow* createWindow(int width, int height);
void executeRenderLoop(GLFWwindow* window, unsigned int shaderProgram, unsigned int VBO);
void render();
unsigned int setVertexInput
(
    float vertices[],
    unsigned int verticesInBytes,
    unsigned int indices[],
    unsigned int indicesInBytes
);

enum class ShaderType { Vertex, Fragment };
unsigned int compileShader(const char* shaderSource, ShaderType shaderType);
unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader);

const char* _vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* _fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main() {
    glfwInit();
    std::cout << "hej" << std::endl;
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

    auto vertexShader = compileShader(_vertexShaderSource, ShaderType::Vertex);
    auto fragmentShader = compileShader(_fragmentShaderSource, ShaderType::Fragment);
    auto shaderProgram = createShaderProgram(vertexShader, fragmentShader);

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    auto VAO = setVertexInput(vertices, sizeof(vertices), indices, sizeof(indices));

    executeRenderLoop(window, shaderProgram, VAO);

    // look at this later (optional)
    //cleanup(shaderProgram, VAO, VBO);
    glfwTerminate();
    return 0;
}


void executeRenderLoop(GLFWwindow* window, unsigned int shaderProgram, unsigned int VAO) {
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        clearBuffers();

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // TODO: makes number of indices not hardcoded

        //render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

//currently not used
void cleanup(unsigned int shaderProgram, unsigned int VAO, unsigned int VBO) {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

void render() {

}

unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR! Linking of shader program failed\n" << infoLog << std::endl;
    }

    return shaderProgram;
}

unsigned int compileShader(const char* shaderSource, ShaderType shaderType) {
    unsigned int shaderObject = shaderType == ShaderType::Vertex ? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderObject, 1, &shaderSource, NULL);
    glCompileShader(shaderObject);

    int success;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shaderObject, 512, NULL, infoLog);

        std::string errorMessage = shaderType == ShaderType::Vertex
            ? "ERROR! Vertex shader failed to compile\n"
            : "ERROR! Fragment shader failed to compile\n";
        std::cout << errorMessage << infoLog << std::endl;
    }

    return shaderObject;
}

unsigned int setVertexInput
(
    float vertices[],
    unsigned int verticesInBytes,
    unsigned int indices[],
    unsigned int indicesInBytes
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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