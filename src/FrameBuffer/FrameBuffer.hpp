#pragma once
#include <glad/glad.h>

enum class ColorType { Texture, Rbo };
enum class DepthStencilType { 
	None, 
	DepthStencilRbo, 
	DepthTex,
	DepthRbo
};

class FrameBuffer
{
public:
	FrameBuffer(unsigned int width, unsigned int height, 
				ColorType colorAttachmentType, DepthStencilType depthStencilAttachmentType);

	FrameBuffer(const FrameBuffer& other);
	FrameBuffer(FrameBuffer&& other) noexcept;
	FrameBuffer& operator=(FrameBuffer other);
	~FrameBuffer();
	
	void Bind();
	void Resize(unsigned int width, unsigned int height);

	GLuint GetFbo() { return mFbo; }
	GLuint GetColorTex() { return mColorTex; }
	GLuint GetColorRbo() { return mColorRbo; }
	GLuint GetDepthTex() { return mDepthTex; }
	GLuint GetDepthRbo() { return mDepthRbo; }
	ColorType GetColorType() { return mColorType; }
	DepthStencilType GetDepthStencilType() { return mDepthStencilType; }
	unsigned int GetWidth() { return mWidth; }
	unsigned int GetHeight() { return mHeight; }

private:
	void Create();
	void CreateFbo();
	void CreateColorAttachment();
	void CreateDepthAndStencilAttachments();
	GLuint CreateTextureAttachment(GLuint attachment, GLuint internalFormat);
	GLuint CreateRboAttachment(GLuint attachment, GLuint internalFormat);
	void CheckFrambufferStatus();
	void Cleanup();
	void swap(FrameBuffer& first, FrameBuffer& second) noexcept;

	GLuint mFbo = 0;
	GLuint mColorTex = 0;
	GLuint mColorRbo = 0;
	GLuint mDepthTex = 0;
	GLuint mDepthRbo = 0;
	GLuint mDepthStencilRbo = 0;

	ColorType mColorType;
	DepthStencilType mDepthStencilType;

	unsigned int mWidth;
	unsigned int mHeight;
};

