#include "FrameBuffer.hpp"
#include <iostream>

FrameBuffer::FrameBuffer(
	unsigned int width, unsigned int height,
	ColorType colorAttachmentType, DepthStencilType depthStencilAttachmentType) :
		mWidth(width),
		mHeight(height),
		mColorType(colorAttachmentType),
		mDepthStencilType(depthStencilAttachmentType) 
{
	Create();
}

FrameBuffer::FrameBuffer(const FrameBuffer& other) :
	FrameBuffer(other.mWidth, other.mHeight, other.mColorType, other.mDepthStencilType) {}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept :
	mWidth(other.mWidth), mHeight(other.mHeight), 
	mColorType(other.mColorType), mDepthStencilType(other.mDepthStencilType),
	mFbo(other.mFbo), mColorTex(other.mColorTex), mColorRbo(other.mColorRbo),
	mDepthTex(other.mDepthTex), mDepthRbo(other.mDepthRbo), mDepthStencilRbo(other.mDepthStencilRbo) {}

FrameBuffer& FrameBuffer::operator=(FrameBuffer other) {
	swap(*this, other);

	return *this;
}

void FrameBuffer::swap(FrameBuffer& first, FrameBuffer& second) noexcept {
	using std::swap;

	swap(first.mColorTex, second.mColorTex);
	swap(first.mColorRbo, second.mColorRbo);
	swap(first.mDepthTex, second.mDepthTex);
	swap(first.mDepthRbo, second.mDepthRbo);
	swap(first.mDepthStencilRbo, second.mDepthStencilRbo);

	swap(first.mColorType, second.mColorType);
	swap(first.mDepthStencilType, second.mDepthStencilType);

	swap(first.mWidth, second.mWidth);
	swap(first.mHeight, second.mHeight);


}

FrameBuffer::~FrameBuffer() {
	Cleanup();
}

void FrameBuffer::Create()
{
	CreateFbo();
	CreateColorAttachment();
	CreateDepthAndStencilAttachments();

	CheckFrambufferStatus();
}

void FrameBuffer::CreateColorAttachment() {
	switch (mColorType)
	{
	case ColorType::Texture:
		mColorTex = CreateTextureAttachment(GL_COLOR_ATTACHMENT0, GL_RGB); // TODO: Switch to sized internal format if possible (gives error for GL_RGB8, but it should work according to khronos docs)
		break;
	case ColorType::Rbo:
		mColorRbo = CreateRboAttachment(GL_COLOR_ATTACHMENT0, GL_RGB);
		break;
	}
}

void FrameBuffer::CreateDepthAndStencilAttachments() {
	switch (mDepthStencilType)
	{
	case DepthStencilType::DepthStencilRbo:
		mDepthStencilRbo = CreateRboAttachment(GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8);
		break;
	case DepthStencilType::DepthTex:
		mDepthTex = CreateTextureAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT);
		break;
	case DepthStencilType::DepthRbo:
		mDepthRbo = CreateRboAttachment(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT);
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
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "ERROR::FRAMEBUFFER: Framebuffer is not complete!" << std::endl;
		
		switch (status)
		{
		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cout << "DETAILED::ERROR: GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cout << "DETAILED::ERROR: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
			break;
		}
	}
		
}

void FrameBuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
}

void FrameBuffer::Resize(unsigned int width, unsigned int height) {
	Cleanup();
	mWidth = width;
	mHeight = height;
	Create();
}

void FrameBuffer::Cleanup() {
	glDeleteFramebuffers(1, &mFbo);
	mFbo = 0;
	
	glDeleteTextures(1, &mColorTex);
	mColorTex = 0;

	glDeleteRenderbuffers(1, &mColorRbo);
	mColorRbo = 0;

	glDeleteBuffers(1, &mDepthTex);
	mDepthTex = 0;

	glDeleteRenderbuffers(1, &mDepthRbo);
	mDepthRbo = 0;

	glDeleteRenderbuffers(1, &mDepthStencilRbo);
	mDepthStencilRbo = 0;
}