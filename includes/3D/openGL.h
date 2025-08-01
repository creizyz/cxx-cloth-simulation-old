#pragma once

#define GLFW_INCLUDE_NONE

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <GLFW/glfw3.h> // GLFW, OpenGL

extern void glfwInitAndCreateWindow(GLFWwindow* & window, int width, int height, std::string title);
extern void glfwErrorCallback(int error, const char* description);
extern void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

extern void glBindingInit();
extern void afterOpenglCallback(const glbinding::FunctionCall& call);
extern void beforeOpenglCallback(const glbinding::FunctionCall& call);
