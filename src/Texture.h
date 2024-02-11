#pragma once

#include "core.h"

class Texture {
    unsigned int m_renderer_id;
    std::string m_filepath;
    unsigned char* m_local_buffer;
    int m_width, m_height, m_bits_per_pixel;
    public:
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int get_width() const {return m_width;} 
    inline int get_height() const {return m_height;} 
};