#pragma once

#include <string>
#include <unordered_map>
#include "vendor/glm/glm.hpp"

struct ShaderSource {
    std::string VertexShader;
    std::string FragmentShader;
};

class Shader {
    unsigned int m_render_id;
    std::string m_filepath;
    std::unordered_map<std::string, unsigned int> uniform_location_cache;

    int get_uniform_location(const std::string& name);

    public:
    Shader(const std::string& filepath);
    ~Shader();

    unsigned int compile_shader(unsigned int type, const std::string& source);
    unsigned int create_shader(const std::string& vertexShader, const std::string& fragmentShader);
    ShaderSource parse_shader(const std::string& filepath);


    void bind() const;
    void unbind() const;

    void set_uniform_1i(const std::string& name, int value);
    void set_uniform_1f(const std::string& name, int value);
    void set_uniform_3f(const std::string& name, float v0, float v1, float v2);
    void set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3);
    void set_uniform_mat4f(const std::string& name, const glm::mat4& mat);
};