#ifndef _INCLUDES_OPENGL_
#define _INCLUDES_OPENGL_

// Disable warning messages 4251
#pragma warning( disable : 4251 ) // glbinding issue

#define GLFW_INCLUDE_NONE

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include "GLFW\glfw3.h" // GLFW, OpenGL

extern void glfwInitAndCreateWindow(GLFWwindow * & window, int width, int height, std::string title);
extern void glfwErrorCallback(int error, const char* description);
extern void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

extern void glBindingInit();
extern void afterOpenglCallback(const glbinding::FunctionCall & call);
extern void beforOpenglCallback(const glbinding::FunctionCall & call);

extern gl::GLuint compile_shader(const char * shaderSource, gl::GLenum shaderType, bool inFile = false);
extern gl::GLuint create_shader_program(const char *path_vert_shader, const char *path_frag_shader, bool inFile = false);

#endif