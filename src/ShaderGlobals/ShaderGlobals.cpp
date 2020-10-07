#include "ShaderGlobals.hpp"

void ShaderGlobals::LinkGlobals(GLuint shaderObj) {
	mView.BindBlock(shaderObj);
}