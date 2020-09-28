#include "OpenGLObjectGenerator.hpp"

OpenGLObjectArray OpenGLObjectGenerator::GenBuffers(unsigned int count) {
    auto funcGenerate = [](unsigned int nObjects, GLuint* firstObject) {
        glGenBuffers(nObjects, firstObject);
    };
    auto funcDelete = [](unsigned int nObjects, GLuint* firstObject) {
        glDeleteBuffers(nObjects, firstObject);
    };

    return OpenGLObjectArray(count, funcGenerate, funcDelete);
}

OpenGLObjectArray OpenGLObjectGenerator::GenVertexArrays(unsigned int count) {
    auto funcGenerate = [](unsigned int nObjects, GLuint* firstObject) {
        glGenVertexArrays(nObjects, firstObject);
    };
    auto funcDelete = [](unsigned int nObjects, GLuint* firstObject) {
        glDeleteVertexArrays(nObjects, firstObject);
    };

    return OpenGLObjectArray(count, funcGenerate, funcDelete);
}

OpenGLObjectArray OpenGLObjectGenerator::GenTexture(unsigned int count) {
    auto funcGenerate = [](unsigned int nObjects, GLuint* firstObject) {
        glGenTextures(nObjects, firstObject);
    };
    auto funcDelete = [](unsigned int nObjects, GLuint* firstObject) {
        glDeleteBuffers(nObjects, firstObject);
    };

    return OpenGLObjectArray(count, funcGenerate, funcDelete);
}