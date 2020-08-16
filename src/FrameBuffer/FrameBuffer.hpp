#pragma once
#include <glad/glad.h>

enum class ColorType { Texture, Rbo };
enum class DepthStencilType { None, DepthTexture, DepthRbo, DepthStencilRbo };

class FrameBuffer
{
public:
	FrameBuffer(ColorType colorAttachmentType, DepthStencilType depthStencilAttachmentType);

	/*Binds new fbo and creates attachments. Deletes any existing fbo and attachments*/
	void Create(unsigned int width, unsigned int height);
	void Bind();
	/*Deletes fbo and attachments*/
	void CleanUp();

	GLuint GetFbo() { return mFbo; }
	GLuint GetColorAttachment() { return mColorAttachment; }
	GLuint GetDepthStencilAttachment() { return mDepthStencilAttachment; }
	ColorType GetColorType() { return mColorType; }
	DepthStencilType GetDepthStencilType() { return mDepthStencilType; }
	unsigned int GetWidth() { return mWidth; }
	unsigned int GetHeight() { return mHeight; }

private:
	void CreateFbo();
	void CreateColorAttachment();
	void CreateDepthStencilAttachment();
	GLuint CreateTextureAttachment(GLuint attachment, GLuint internalFormat);
	GLuint CreateRboAttachment(GLuint attachment, GLuint internalFormat);
	void CheckFrambufferStatus();
	

	GLuint mFbo = 0;
	GLuint mColorAttachment = 0;
	GLuint mDepthStencilAttachment = 0;

	const ColorType mColorType;
	const DepthStencilType mDepthStencilType;

	unsigned int mWidth;
	unsigned int mHeight;
};

