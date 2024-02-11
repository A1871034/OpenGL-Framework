#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "core.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"


int main(void)
{
    GLFWwindow* window;

    // Enable GLFW Debugging Output
    glfwSetErrorCallback(GLFWErrorCallback);

    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    // Use GLFW Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(2560, 1440, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Synchronises with vsync ie monitor refresh rate, enable vsync
    glfwSwapInterval(1);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    /* Initialize GLEW and ensure it is working*/
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }
    fprintf(stdout, "Status: Using GLEW %s and OpenGL %s\n", glewGetString(GLEW_VERSION), glGetString(GL_VERSION)); 

    // Enable OpenGL Debugging Output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback((GLDEBUGPROC) GLErrorCallback, nullptr);

    float triangle_positions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 0.75f,
        -0.5f,  0.5f, 0.0f, 0.75f,
         0.0f, 0.75f, 0.5f, 1.0f
    };

    unsigned int indicies[] = {
        0, 1, 2,
        0, 2, 3,
        3, 4, 2,        
    };

    // Set blend mode for alpha channel
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Create Vertex Array object
    VertexArray va;
    va.bind();

    // Vertex Buffer
    VertexBuffer vb(&triangle_positions, 5 * 4 * sizeof(float), GL_STATIC_DRAW);
    // Links this vertex buffer with the vertex array object and describes attributes
    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);
    va.AddBuffer(vb, layout);

    // Index Buffer
    // is also stored state in the vertex array object
    IndexBuffer ib(indicies, 9);

    // Projection matrix
    glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f);

    // Load and bind Shaders
    Shader shader("res/shaders/basic.glsl");

    Texture texture("res/textures/img.png");
    texture.bind();
    shader.bind();
    shader.set_uniform_1i("u_Texture", 0);
    shader.set_uniform_mat4f("u_MVP", proj);
    
    // Renderer
    Renderer renderer;

    // Info for colours going to uniform 
    float shader_colour[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float colour_increment[4] = {0.05f, 0.025f, 0.0125f, 0.0f};

    // Controllable
    float target_colour[3] = {1.0f, 1.0f, 1.0f};
    float distance_ratios_texOverride[3] = {0};

    float f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();

        // ImGui Stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.bind();
        shader.set_uniform_4f("u_Color", shader_colour[0], shader_colour[1], shader_colour[2], shader_colour[3]);
        
        renderer.Draw(va, ib, shader);

        for (int i = 0; i < 4; i++) {
            if (shader_colour[i] >= 1.0f || shader_colour[i] <= 0.0f) {
                colour_increment[i] = -colour_increment[i];
            }
            shader_colour[i] += colour_increment[i];
        }

        // ImGui Gui
        {
            ImGui::SliderFloat("Colour max distance", &(distance_ratios_texOverride[0]), 0.0f, 1.0f);
            ImGui::SliderFloat("Colour max ratio difference", &(distance_ratios_texOverride[1]), 0.0f, 1.0f);
            ImGui::SliderFloat("Alpha overide", &(distance_ratios_texOverride[2]), 0.0f, 1.0f);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }

        // ImGui controlled values
        shader.set_uniform_3f("target_colour" , target_colour[0], target_colour[1], target_colour[2]);
        shader.set_uniform_3f("distance_ratios_texOverride" , distance_ratios_texOverride[0], distance_ratios_texOverride[1], distance_ratios_texOverride[2]);

        // ImGui Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // ImGui Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}