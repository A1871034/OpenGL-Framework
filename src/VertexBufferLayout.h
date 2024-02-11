#pragma once

#include <vector>

#include "core.h"

struct VertexBufferElement {
    unsigned int type;
    unsigned int count;
    bool norm;

    static unsigned int get_type_size(unsigned int type);
};

class VertexBufferLayout {
    std::vector<VertexBufferElement> m_elements;
    unsigned int m_stride;
    public:
    VertexBufferLayout() : m_stride(0) {}

    template <typename T>
    void push(unsigned int count);

    inline const std::vector<VertexBufferElement>& get_elements() const {return m_elements;}
    inline const unsigned int get_stride() const {return m_stride;}
};