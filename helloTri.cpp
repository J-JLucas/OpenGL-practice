#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

std::string loadShaderSource(const std::string &filepath)
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

int main()
{
  // Initialize GLFW
  // openGL doesn't actually make a window for us
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

  // define triangle geometry
  // vertices are defined in Normalized Device Coordinates (NDC)
  // NDC is a coordinate system where the visible range is from -1 to 1,
  // centered at 0,0 OpenGL will transform these coordinates to screen
  // coordinates OpenGL expects vertices to be defined in counter-clockwise
  // order OpenGL uses right-handed coordinate system
  float triangle_verts[] = {
      -0.5f, -0.5f, 0.0f, // bottom left
      0.5f,  -0.5f, 0.0f, // bottom right
      0.0f,  0.5f,  0.0f  // top
  };

  // Create Vertex Buffer Object (VBO)
  // VBO is a buffer in GPU memory that stores vertex data
  // VBOs are used to store vertex attributes (position, color, normal, etc)
  // different data types requires different VBO macros
  // The more data you send at once, the better the performance
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  // bind VBO buffer to GL_ARRAY_BUFFER target (vertex array type)
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // copy vertex data to bound GL_ARRAY_BUFFER (our VBO buffer)
  // 3 draw modes:
  // GL_STATIC_DRAW: the data is set once and used many times; static objects
  // GL_DYNAMIC_DRAW: the data will be changed a lot and used often; moving
  // objects GL_STREAM_DRAW: the data is set only once and only used a few
  // times; usecase? idk
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verts), triangle_verts,
               GL_STATIC_DRAW);

  /* SHADERS */
  // Shaders are written in GLSL (OpenGL Shading Language)

  // Load shader sources
  std::string vertexShaderSource = loadShaderSource("../shaders/basic.vert");
  std::string fragmentShaderSource = loadShaderSource("../shaders/basic.frag");

  const char *vertexShaderSourceC = vertexShaderSource.c_str();
  const char *fragmentShaderSourceC = fragmentShaderSource.c_str();

  // Vertex Shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
  glCompileShader(vertexShader);

  // Fragment Shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, NULL);
  glCompileShader(fragmentShader);

  // Shader Program
  // A shader program is a collection of shaders that are linked together
  unsigned int shaderProgram = glCreateProgram();

  // link compiled shaders to shader program
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // tell OpenGL to use our shader program
  glUseProgram(shaderProgram);

  // doesn't really delete them,
  // just marks them for deletion once the shaderprogram is finished with them
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

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
