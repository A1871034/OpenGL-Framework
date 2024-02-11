#pragma once

#include <GL/glew.h>

class VertexBuffer {
    unsigned int m_renderer_id;
    public:
    VertexBuffer(const void* data, unsigned int size, GLenum usage);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;

};