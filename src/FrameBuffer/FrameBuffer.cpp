#include "FrameBuffer.hpp"
#include <iostream>

FrameBuffer::FrameBuffer(
	ColorType colorAttachmentType,
	DepthStencilType depthStencilAttachmentType) :
		mColorType(colorAttachmentType),
		mDepthStencilType(depthStencilAttachmentType) {}

void FrameBuffer::Create(unsigned int width, unsigned int height)
{
	CleanUp();

	mWidth = width;
	mHeight = height;

	CreateFbo();
	CreateColorAttachment();
	CreateDepthStencilAttachment();

	CheckFrambufferStatus();
}

void FrameBuffer::CreateColorAttachment() {
	switch (mColorType)
	{
	case ColorType::Texture:
		mColorAttachment = CreateTextureAttachment(GL_COLOR_ATTACHMENT0, GL_RGB); // TODO: Switch to sized internal format if possible (gives error for GL_RGB8, but it should work according to khronos docs)
		break;
	case ColorType::Rbo:
		mColorAttachment = CreateRboAttachment(GL_COLOR_ATTACHMENT0, GL_RGB);
		break;
	}
}

void FrameBuffer::CreateDepthStencilAttachment() {
	switch (mDepthStencilType)
	{
	case DepthStencilType::DepthTexture:
		mDepthStencilAttachment = CreateTextureAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT);
		break;
	case DepthStencilType::DepthRbo:
		mDepthStencilAttachment = CreateRboAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT24);
		break;
	case DepthStencilType::DepthStencilRbo:
		mDepthStencilAttachment = CreateRboAttachment(GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8);
		break;
	}
}

void FrameBuffer::CreateFbo() {
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

GLuint FrameBuffer::CreateTextureAttachment(GLuint attachment, GLuint internalFormat) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, mWidth, mHeight, 0, internalFormat, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);

	return texture;
}

GLuint FrameBuffer::CreateRboAttachment(GLuint attachment, GLuint internalFormat) {
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, mWidth, mHeight); 
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, rbo);

	return rbo;
}

void FrameBuffer::CheckFrambufferStatus() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

void FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

void FrameBuffer::CleanUp() {
	glDeleteFramebuffers(1, &mFbo);
	mFbo = 0;

	glDeleteTextures(1, &mColorAttachment);
	glDeleteRenderbuffers(1, &mColorAttachment);
	mColorAttachment = 0;

	glDeleteBuffers(1, &mDepthStencilAttachment);
	glDeleteRenderbuffers(1, &mDepthStencilAttachment);
	mDepthStencilAttachment = 0;
}