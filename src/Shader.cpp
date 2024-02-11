#include<GL/glew.h>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

#include "core.h"
#include "Shader.h"


int Shader::get_uniform_location(const std::string &name) {
    if (uniform_location_cache.find(name) != uniform_location_cache.end()) {
        return uniform_location_cache[name];
    }

    int location = glGetUniformLocation(m_render_id, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform \"" << name << "\" not found in shader" << std::endl;
    }

    uniform_location_cache[name] = location;
    
    return location;
}

Shader::Shader(const std::string &filepath) : m_filepath(filepath), m_render_id(0) {
    ShaderSource sources = parse_shader(filepath);
    m_render_id = create_shader(sources.VertexShader, sources.FragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(m_render_id);
}

unsigned int Shader::compile_shader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int compile_status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);

    if (compile_status == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*) alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
        std::cerr << message << std::endl;
        ASSERT("");
        glDeleteShader(id);
        return 0;        
    } 

    return id;
}

unsigned int Shader::create_shader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = compile_shader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compile_shader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShaderSource Shader::parse_shader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else if (type != ShaderType::NONE) {
            ss[(int) type] << line << "\n";
        }
    }
    return {ss[0].str(), ss[1].str()};
}

void Shader::bind() const {
    glUseProgram(m_render_id);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::set_uniform_1i(const std::string &name, int value) {
    glUniform1i(get_uniform_location(name), value);
}

void Shader::set_uniform_1f(const std::string &name, int value) {
    glUniform1f(get_uniform_location(name), value);
}

void Shader::set_uniform_3f(const std::string &name, float v0, float v1, float v2) {
    glUniform3f(get_uniform_location(name), v0, v1, v2);
}

void Shader::set_uniform_4f(const std::string &name, float v0, float v1, float v2, float v3) {
    glUniform4f(get_uniform_location(name), v0, v1, v2, v3);
}

void Shader::set_uniform_mat4f(const std::string &name, const glm::mat4& mat) {
    glUniformMatrix4fv(get_uniform_location(name), 1, false, &(mat[0][0]));
}
