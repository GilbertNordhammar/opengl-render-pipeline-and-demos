#include "GLObjectGenerator.hpp"

GLObjectArray GLObjectGenerator::GenBuffers(unsigned int count) {
    auto funcGenerate = [](unsigned int nObjects, GLuint* firstObject) {
        glGenBuffers(nObjects, firstObject);
    };
    auto funcDelete = [](unsigned int nObjects, GLuint* firstObject) {
        glDeleteBuffers(nObjects, firstObject);
    };

    return GLObjectArray(count, funcGenerate, funcDelete);
}

GLObjectArray GLObjectGenerator::GenVertexArrays(unsigned int count) {
    auto funcGenerate = [](unsigned int nObjects, GLuint* firstObject) {
        glGenVertexArrays(nObjects, firstObject);
    };
    auto funcDelete = [](unsigned int nObjects, GLuint* firstObject) {
        glDeleteVertexArrays(nObjects, firstObject);
    };

    return GLObjectArray(count, funcGenerate, funcDelete);
}

GLObjectArray GLObjectGenerator::GenTexture(unsigned int count) {
    auto funcGenerate = [](unsigned int nObjects, GLuint* firstObject) {
        glGenTextures(nObjects, firstObject);
    };
    auto funcDelete = [](unsigned int nObjects, GLuint* firstObject) {
        glDeleteBuffers(nObjects, firstObject);
    };

    return GLObjectArray(count, funcGenerate, funcDelete);
}