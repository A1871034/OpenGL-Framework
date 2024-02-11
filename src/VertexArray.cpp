#include "VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_rendered_id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_rendered_id);
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    bind();
    vb.bind();

    const auto elements = layout.get_elements();
    // Bellow shit goofy to avoid -Wint-to-pointer-cast warning on unsigned int -> const void* 
    unsigned int *offset;
    offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].norm, layout.get_stride(), (const void*) offset);
        glEnableVertexAttribArray(i);
        offset += elements[i].count;
    }
    
}

void VertexArray::bind() const {
    glBindVertexArray(m_rendered_id);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}
