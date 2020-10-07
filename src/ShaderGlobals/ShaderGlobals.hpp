#pragma once
#include <glm/glm.hpp>
#include "src/OpenGLObjectGenerator/OpenGLObjectGenerator.hpp"
#include "SG_Containers/View/View.hpp"

class ShaderGlobals
{
public:
	static ShaderGlobals& Get() {
		static ShaderGlobals instance;
		return instance;
	}

	void LinkGlobals(GLuint shaderObj);

	SG_Containers::View mView = SG_Containers::View(1, "rp_uniformBlock_View");
private:
	ShaderGlobals() {};
};

