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
    GLuint GetProgramID() { return _programID; }

    Shader(
        const char* vertexPath, 
        const char* fragmentPath, 
        const char* geometryPath = nullptr,
        const std::string debugTag = "") : _debugTag(debugTag)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;

        try
        {
            vertexCode = retrieveShaderCode(vertexPath);
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::VERTEX_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << "Shader file path: " << vertexPath << std::endl;
        }

        try
        {
            fragmentCode = retrieveShaderCode(fragmentPath);
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::FRAGMENT_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << "Shader file path: " << fragmentPath << std::endl;
        }

        try
        {
            if (geometryPath != nullptr)
            {
                geometryCode = retrieveShaderCode(geometryPath);
            }
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::GEOMETRY_SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << "Shader file path: " << geometryPath << std::endl;
        }

        unsigned int vertex = createAndCompileShader(vertexCode, GL_VERTEX_SHADER);
        unsigned int fragment = createAndCompileShader(fragmentCode, GL_FRAGMENT_SHADER);
        unsigned int geometry = 0;
        if (geometryPath != nullptr) {
            geometry = createAndCompileShader(fragmentCode, GL_GEOMETRY_SHADER);
        }

        _programID = createAndLinkProgram(vertex, fragment, geometry);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
        if (geometryPath != nullptr)
            glDeleteShader(geometry);
    }

    void use()
    {
        glUseProgram(_programID);
    }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(_programID, name.c_str()), (int)value);
    }

    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(_programID, name.c_str()), value);
    }

    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(_programID, name.c_str()), value);
    }

    void setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]);
    }
    void setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(_programID, name.c_str()), x, y);
    }

    void setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]);
    }
    void setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(_programID, name.c_str()), x, y, z);
    }

    void setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(_programID, name.c_str()), 1, &value[0]);
    }
    void setVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(_programID, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(_programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    GLuint _programID;
    std::string _debugTag = nullptr;

    unsigned int createAndLinkProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader) 
    {
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        if (geometryShader != 0)
            glAttachShader(programID, geometryShader);
        
        glLinkProgram(programID);
        checkLinkErrors(programID);

        return programID;
    }

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
                << "Debug tag: " << _debugTag << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
};