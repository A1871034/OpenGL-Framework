#include "core.h"

#include <sstream>

std::map<GLenum, std::string> GLENUM_LOOKUP = {
    {GL_DEBUG_SOURCE_APPLICATION, "GL_DEBUG_SOURCE_APPLICATION"},
    {GL_DEBUG_SOURCE_THIRD_PARTY, "GL_DEBUG_SOURCE_THIRD_PARTY"},
    {GL_DEBUG_TYPE_ERROR, "GL_DEBUG_TYPE_ERROR"},
    {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR"},
    {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR"},
    {GL_DEBUG_TYPE_PORTABILITY, "GL_DEBUG_TYPE_PORTABILITY"},
    {GL_DEBUG_TYPE_PERFORMANCE, "GL_DEBUG_TYPE_PERFORMANCE"},
    {GL_DEBUG_TYPE_MARKER, "GL_DEBUG_TYPE_MARKER"},
    {GL_DEBUG_TYPE_PUSH_GROUP, "GL_DEBUG_TYPE_PUSH_GROUP"},
    {GL_DEBUG_TYPE_POP_GROUP, "GL_DEBUG_TYPE_POP_GROUP"},
    {GL_DEBUG_TYPE_OTHER, "GL_DEBUG_TYPE_OTHER"},
    {GL_DEBUG_SEVERITY_LOW, "GL_DEBUG_SEVERITY_LOW"},
    {GL_DEBUG_SEVERITY_MEDIUM, "GL_DEBUG_SEVERITY_MEDIUM"},
    {GL_DEBUG_SEVERITY_HIGH, "GL_DEBUG_SEVERITY_HIGH"},
    {GL_DEBUG_SEVERITY_NOTIFICATION, "GL_DEBUG_SEVERITY_NOTIFICATION"}
};

std::string GLenum_to_hex_or_str(GLenum value) {
    if (GLENUM_LOOKUP.find(value) != GLENUM_LOOKUP.end()) {
        return GLENUM_LOOKUP[value];
    }

    std::stringstream stream;
    stream << "0x" << std::hex << value;
    return stream.str();
}

std::unordered_set<std::string> seen_errors;
void GLAPIENTRY GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar* message, const void *userParam) {
    // Hide notifications based on flag
    if (HIDE_DEBUG_NOTIFICATIONS and severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }

    // Don't display duplicates
    if (seen_errors.empty() || seen_errors.find(std::to_string(id) + message) == seen_errors.end()) {
        seen_errors.insert(std::to_string(id) + message);
    } else {
        // Assert here?
        return;
    }
    
    std::cerr << "----- GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "OpenGL ERROR" : "" ) << std::endl;
    std::cerr << "Message: " <<  message << std::endl;
    std::cerr << "Type: " <<  GLenum_to_hex_or_str(type);
    std::cerr << " | Severity: " <<  GLenum_to_hex_or_str(severity);
    std::cerr << " | ID: 0x" << std::hex << id << std::endl;
    std::cerr << "-----------------" << std::endl;

    if (severity <= DEBUG_ASSERT_SEVERITY and severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        ASSERT("");
    }
}

void GLFWErrorCallback(int error, const char* message) {
    std::cerr << "------ GLFW CALLBACK:" << std::endl;
    std::cerr << "Message: " << message << std::endl;
    std::cerr << "Error Code: " << std::hex << error << std::endl;

    ASSERT("");

}