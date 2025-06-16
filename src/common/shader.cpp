#include <common/shader.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader()
{
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

Shader::Shader(const char* vertex, const char* fragment, bool isFromShaderfile)
{
    std::string vShaderCode;
    std::string fShaderCode;

    if(isFromShaderfile)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertex);
            fShaderFile.open(fragment);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        vShaderCode = std::string(vertexCode);
        fShaderCode = std::string(fragmentCode);
    }
    else
    {
        vShaderCode = vertex;
        fShaderCode = fragment;
    }

    const char* vCode = vShaderCode.c_str();
    const char* fCode = fShaderCode.c_str();

    // 2. compile shaders
    unsigned int v, f;
    int success;
    char infoLog[512];

    // vertex Shader
    v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, &vCode, NULL);
    glCompileShader(v);
    // print compile errors if any
    glGetShaderiv(v, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(v, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // fragment Shader
    f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, &fCode, NULL);
    glCompileShader(f);
    // print compile errors if any
    glGetShaderiv(f, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(f, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, v);
    glAttachShader(ID, f);
    glLinkProgram(ID);
    // print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete shaders
    glDeleteShader(v);
    glDeleteShader(f);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), int(value));
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID,name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& vec) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vec));
}
