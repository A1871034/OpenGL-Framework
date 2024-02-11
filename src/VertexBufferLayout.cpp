#include "VertexBufferLayout.h"

#include <GL/glew.h>

template <typename T>
void VertexBufferLayout::push(unsigned int count) {ASSERT("Pusshing unsupported type to VertexBufferLayout");}

template<>
void VertexBufferLayout::push<float>(unsigned int count) {
    m_elements.push_back(VertexBufferElement {GL_FLOAT, count, false});
    m_stride += VertexBufferElement::get_type_size(GL_FLOAT)*count;
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count) {
    m_elements.push_back(VertexBufferElement {GL_UNSIGNED_INT, count, false});
    m_stride += VertexBufferElement::get_type_size(GL_UNSIGNED_INT)*count;
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count) {
    m_elements.push_back(VertexBufferElement {GL_UNSIGNED_BYTE, count, false});
    m_stride += VertexBufferElement::get_type_size(GL_UNSIGNED_BYTE)*count;
}

unsigned int VertexBufferElement::get_type_size(unsigned int type) {
    switch (type) {
        case GL_FLOAT:
            return sizeof(GLfloat);
        case GL_UNSIGNED_INT:
            return sizeof(GLuint);
        case GL_UNSIGNED_BYTE:
            return sizeof(GLubyte);
        
        default:
            ASSERT("Getting size of unsupported type");
    }
    return 0;
}