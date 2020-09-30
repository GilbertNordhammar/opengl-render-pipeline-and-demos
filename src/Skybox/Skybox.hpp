#pragma once
#include <glad/glad.h>
#include "src/Shader/Shader.hpp"
#include "src/CubeMap/Cubemap.hpp"
#include "src/utils/fileUtils.h"
#include <glm/glm.hpp>

class Skybox
{
public:
	Skybox(Cubemap cubemap);
	void Draw(const glm::mat4& view, const glm::mat4& projection);
 	
private:
	static GLuint CreateSkyboxVAO();

	GLuint mSkyboxVAO;
	Shader mShader = Shader(
		fileUtils::getFullResourcesPath("shaders/Skybox/Skybox.vert"),
		fileUtils::getFullResourcesPath("shaders/Skybox/Skybox.frag"));
	Cubemap mCubemap;
};

