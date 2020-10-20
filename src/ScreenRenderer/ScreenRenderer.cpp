#include "ScreenRenderer.hpp"
#include "src/utils/fileUtils.h"
#include <string>
#include <optick.h>

const std::string NO_EFFECT_PP_FRAGMENT_PATH = fileUtils::getFullResourcesPath("shaders/post_processing/NoEffect.frag");

ScreenRenderer::ScreenRenderer(): 
    mNoEffectPP(NO_EFFECT_PP_FRAGMENT_PATH.c_str()),
    mScreenQuadVAO(CreateScreenQuadVAO()) {}

void ScreenRenderer::Draw(GLuint screenTexture) {
    Draw(screenTexture, &mNoEffectPP);
}

void ScreenRenderer::Draw(GLuint screenTexture, PostProcessEffect* ppEffect) {
    OPTICK_EVENT();

    glClear(GL_COLOR_BUFFER_BIT);

    ppEffect->UseShader(screenTexture);

    glBindVertexArray(mScreenQuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

GLuint ScreenRenderer::CreateScreenQuadVAO() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    unsigned int vbo;
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // NDC coordinates
    float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);

    return vao;
}