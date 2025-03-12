#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>

// this function is called whenever the window is resized
inline void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

inline std::string loadShaderSource(const std::string &filepath)
{
  std::ifstream file(filepath);
  if (!file.is_open()) {
    std::cerr << "Failed to open shader file: " << filepath << std::endl;
    return std::string();
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

inline GLFWwindow *startup_opengl(const char *title, unsigned int width,
                                  unsigned int height)
{
  // Initialize GLFW
  // openGL doesn't actually make a window for us
  if (!glfwInit()) {
    std::cerr << "Failed to init GLFW\n";
    return nullptr;
  }

  // Configure GLFW for OpenGL 3.3 Core (No Immediate Mode)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Init Window Object
  GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create window\n";
    glfwTerminate();
    return nullptr;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  // GLEW is used to load OpenGL functions
  // It must be initialized after OpenGL context is created
  // and before any OpenGL function is called
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to init GLEW\n";
    glfwTerminate();
    return nullptr;
  }

  glViewport(0, 0, width, height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  return window;
}
