#include "../../includes/3D/openGL.h"
#include "macro.h"
#include <iostream>
#include <fstream>
#include <cstring>

void glfwInitAndCreateWindow(GLFWwindow* & window, int width, int height, std::string title)
{
    // Try to initialise GLFW
    if (!glfwInit())
        exit(EXIT_FAILURE);
    else
    {
        glfwSetErrorCallback(glfwErrorCallback);

        glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

        // Try to initialise the window
        if (!(window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL)))
        {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        else
        {
            // Make the window context the current OpenGL context
            glfwMakeContextCurrent(window);
            // Set the key callback
            //glfwSetKeyCallback(window, glfwKeyCallback);
        }
    }
}

void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: (%d) %s\n", error, description);
    DBG_HALT;
}

void glBindingInit()
{
    // Initialize glbinding with the OpenGL function loader (same as before)
    glbinding::Binding::initialize(glfwGetProcAddress);

    // Set the global callback mask for the functions
    glbinding::setCallbackMask(glbinding::CallbackMask::After);

    // Define what the "after" callbacks do
    glbinding::setAfterCallback([](const glbinding::FunctionCall& call)
    {
        static thread_local bool inCallback = false;
        if (!inCallback && strcmp(call.function->name(), "glGetError") != 0)
        {
            inCallback = true;
            gl::GLenum error = gl::glGetError();
            inCallback = false;
            if (error != gl::GL_NO_ERROR)
            {
                std::cerr << "Function: " << call.function->name()
                    << " caused error: " << std::hex << error
                    << std::endl;

                DBG_HALT;
            }
        }
    });

#ifdef ENABLE_OPENGL_LOGS
    // Optional: Set a before-callback for logging or debugging parameters
    glbinding::setCallbackMask(glbinding::CallbackMask::Before | glbinding::CallbackMask::ParametersAndReturnValue);
    glbinding::setBeforeCallback([](const glbinding::FunctionCall &call) {
        std::cout << "Calling: " << call.function->name() << std::endl;
    });
#endif

    // Basic OpenGL state setup
    gl::glEnable(gl::GL_PROGRAM_POINT_SIZE);
}

void beforeOpenglCallback(const glbinding::FunctionCall& call)
{
    std::cout << call.function->name() << std::endl;
}

void afterOpenglCallback(const glbinding::FunctionCall& call)
{
    gl::GLenum error = gl::glGetError();
    if (error != gl::GL_NO_ERROR)
    {
#ifndef ENABLE_OPENGL_LOGS
        std::cout << call.function->name() << std::endl;
#endif
        std::cout << "<> error: " << std::hex << (int)error << std::endl;
        DBG_HALT;
    }
}
