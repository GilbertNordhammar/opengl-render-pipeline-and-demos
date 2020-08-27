#include "PostProcessEffect.hpp"
#include <string>
#include "src/utils/fileUtils.h"

const std::string VERTEX_PATH = fileUtils::getFullResourcesPath("shaders/post_processing/default.vert");

PostProcessEffect::PostProcessEffect(const std::string& fragmentPath) : mShader(VERTEX_PATH, fragmentPath) {}

void PostProcessEffect::UseShader(GLuint screenTexture) {
    mShader.use();
    mShader.setInt("screenTexture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
}