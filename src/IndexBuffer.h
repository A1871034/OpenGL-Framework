#pragma once

#include <GL/glew.h>

class IndexBuffer {
    unsigned int m_renderer_id;
    unsigned int m_count;
    public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void bind() const;
    void unbind() const;

    unsigned int get_count() const {return m_count;}
};