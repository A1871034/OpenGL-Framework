#pragma once

#include <GL/glew.h>
#include <map>
#include <unordered_set>
#include <iostream>

/* ------ Debug Settings ------ */

// Will assert if Severity is this or worse
#define DEBUG_ASSERT_SEVERITY GL_DEBUG_SEVERITY_LOW 
// Do we hide OpenGL callbacks with severity of notifications
#define HIDE_DEBUG_NOTIFICATIONS true 

/* ----------- End ----------- */

// Assertion to break while debugging
#define SIGTRAP 5
#define ASSERT(x) std::cerr << x; raise(SIGTRAP);

// Debug info enum lookup
extern std::map<GLenum, std::string> GLENUM_LOOKUP;

// Checks if GLenum is in the GLENUM_LOOKUP table and returns its value there or it -> hex.str()
std::string GLenum_to_hex_or_str(GLenum value);

// Will store encountered errors to prevent console spam (key = id+message)
extern std::unordered_set<std::string> seen_errors;

// Callback for glDebugMessageCallback
void GLAPIENTRY GLErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar* message, const void *userParam);

// Callback for glfwSetErrorCallback
void GLFWErrorCallback(int error, const char* message);