#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void *data, unsigned int size, GLenum usage) {
    glGenBuffers(1, &m_renderer_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
    glBufferData(GL_ARRAY_BUFFER, 8 * size, data, usage);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_renderer_id);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
