#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    Shader() {
        ID = 0;
    };
    
    void init() {
        ID = glCreateProgram();
    }
    
    void use()
    {
        glUseProgram(ID);
    }

    int addShader(std::string const &path, GLenum eShaderType) {
     
        std::string shaderCode;
        std::ifstream shaderFile;

        // ensure ifstream objects can throw exceptions:
        shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            shaderFile.open(path.c_str());
            std::stringstream shaderStream;
            
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();

            shaderCode = shaderStream.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        
        const char* cShaderCode = shaderCode.c_str();
        unsigned int shaderID;
        
        // vertex shader
        shaderID = glCreateShader(eShaderType);
        glShaderSource(shaderID, 1, &cShaderCode, NULL);
        glCompileShader(shaderID);
        checkCompileErrors(shaderID, "SHADER");
        
        glAttachShader(ID, shaderID);
        glDeleteShader(shaderID);
        
        return 0;
    }
    
    int link() {
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        
        return 0;
    }
    
private:
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif
