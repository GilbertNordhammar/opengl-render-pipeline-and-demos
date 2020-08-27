#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    GLuint GetProgramID() { return mProgramID; }
    
    Shader(
        const std::string vertexPath,
        const std::string fragmentPath,
        const std::string geometryPath = "") : 
            mVertexPath(vertexPath), 
            mFragmentPath(fragmentPath), 
            mGeometryPath(geometryPath) 
    {
        createAndLinkProgram();
        std::cout << "Created " << mProgramID << " " << this << std::endl; //
    }

    Shader(const Shader& other) : 
        Shader(other.mVertexPath, other.mFragmentPath, other.mGeometryPath) {
        std::cout << "Copied " << other.mProgramID << " " << &other << std::endl; //
    }

    Shader(Shader&& other) noexcept :
        mVertexPath(other.mVertexPath), 
        mFragmentPath(other.mFragmentPath), 
        mGeometryPath(other.mGeometryPath),
        mProgramID(other.mProgramID) {
        std::cout << "Moved " << other.mProgramID << " " << &other << std::endl; //
    }

    Shader& operator=(Shader other) {
        swap(*this, other);

        return *this;
    }

    void swap(Shader& first, Shader& second) noexcept {
        using std::swap;

        swap(first.mProgramID, second.mProgramID);
        swap(first.mVertexPath, second.mVertexPath);
        swap(first.mFragmentPath, second.mFragmentPath);
        swap(first.mGeometryPath, second.mGeometryPath);
    }

    ~Shader() {
        std::cout << "Destroyed " << mProgramID << " " << this << std::endl; //
        glDeleteProgram(mProgramID);
    }

    void createAndLinkProgram() {
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;

        try
        {
            vertexCode = retrieveShaderCode(mVertexPath.c_str());
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::VERTEX_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << "Shader file path: " << mVertexPath << std::endl;
        }

        try
        {
            fragmentCode = retrieveShaderCode(mFragmentPath.c_str());
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::FRAGMENT_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << "Shader file path: " << mFragmentPath << std::endl;
        }

        try
        {
            if (!mGeometryPath.empty())
            {
                geometryCode = retrieveShaderCode(mGeometryPath.c_str());
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::GEOMETRY_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << "Shader file path: " << mGeometryPath << std::endl;
        }

        unsigned int vertex = createAndCompileShader(vertexCode, GL_VERTEX_SHADER);
        unsigned int fragment = createAndCompileShader(fragmentCode, GL_FRAGMENT_SHADER);
        unsigned int geometry = 0;
        if (!mGeometryPath.empty()) {
            geometry = createAndCompileShader(fragmentCode, GL_GEOMETRY_SHADER);
        }

        mProgramID = glCreateProgram();
        glAttachShader(mProgramID, vertex);
        glAttachShader(mProgramID, fragment);
        if (geometry != 0)
            glAttachShader(mProgramID, geometry);

        glLinkProgram(mProgramID);
        checkLinkErrors(mProgramID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (!mGeometryPath.empty())
            glDeleteShader(geometry);
    }

    void use()
    {
        glUseProgram(mProgramID);
    }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(mProgramID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(mProgramID, name.c_str()), value);
    }

    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(mProgramID, name.c_str()), x, y);
    }

    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(mProgramID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(mProgramID, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(mProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    GLuint mProgramID; 
    std::string mVertexPath;
    std::string mFragmentPath;
    std::string mGeometryPath;

    unsigned int createAndCompileShader(std::string shaderCode, GLenum shaderType) {
        const char* gShaderCode = shaderCode.c_str();
        unsigned int shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &gShaderCode, NULL);
        glCompileShader(shaderID);
        checkCompileErrors(shaderID, shaderType);

        return shaderID;
    }

    std::string retrieveShaderCode(const char* shaderFileSrc) {
        std::ifstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        std::stringstream shaderStream;
        shaderFile.open(shaderFileSrc);
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();

        return shaderStream.str();
    }

    void checkCompileErrors(GLuint shader, GLenum shaderType)
    {
        GLint success;
        GLchar infoLog[1024];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR" << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }

    void checkLinkErrors(GLuint program)
    {
        GLint success;
        GLchar infoLog[1024];

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR" << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
};