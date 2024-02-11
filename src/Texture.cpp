#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string &path)
    : m_renderer_id(0), m_filepath(path), m_local_buffer(nullptr),
    m_width(0), m_height(0), m_bits_per_pixel(0) 
{
    // Load image
    stbi_set_flip_vertically_on_load(1);
    m_local_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bits_per_pixel, 4);

    // Get a texture buffer and bind
    glGenTextures(1, &m_renderer_id);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);

    // Texture Parameters (must be set)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Set texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer);
    unbind();

    if (m_local_buffer) {
        stbi_image_free(m_local_buffer);
    }
}

Texture::~Texture() {
    glDeleteTextures(1, &m_renderer_id);
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_renderer_id);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
