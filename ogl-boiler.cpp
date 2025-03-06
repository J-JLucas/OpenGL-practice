#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

int main()
{
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to init GLFW\n";
    return -1;
  }

  // Configure GLFW for OpenGL 3.3 Core (No Immidiate Mode)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Init Window Object
  GLFWwindow *window = glfwCreateWindow(800, 600, "Hello Triangle", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to create window\n";
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  // GLEW is used to load OpenGL functions
  // It must be initialized after OpenGL context is created
  // and before any OpenGL function is called
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to init GLEW\n";
    glfwTerminate();
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Main Render Loop
  while (!glfwWindowShouldClose(window)) {
    // terminate on ESC key press
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render commands here

    // check for events and swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
