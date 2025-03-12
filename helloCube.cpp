#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Include GLM for matrix transformations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

  // Configure GLFW for OpenGL 3.3 Core (No Immediate Mode)
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Init Window Object
  GLFWwindow *window = glfwCreateWindow(800, 600, "3d Cube", NULL, NULL);
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

  glEnable(GL_DEPTH_TEST);

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Define our triangle geometry
  // vertices are defined in Normalized Device Coordinates (NDC)
  // NDC is a coordinate system where the visible range is from -1 to 1,
  // centered at 0,0 OpenGL will transform these coordinates to screen
  // coordinates OpenGL expects vertices to be defined in counter-clockwise
  // order OpenGL uses right-handed coordinate system
  float cubeVerts[] = {
      // Front face
      -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f,
      0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,

      // Back face
      0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
      -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f,

      // Left face
      -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,

      // Right face
      0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f,
      0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f,

      // Top face
      -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f,
      0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,

      // Bottom face
      -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
      -0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f};

  /* SHADERS */
  // Shaders are written in GLSL (OpenGL Shading Language)

  // Load shader source files
  std::string vertexShaderSource = loadShaderSource("../shaders/cube.vert");
  std::string fragmentShaderSource = loadShaderSource("../shaders/cube.frag");

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

  // 1)
  // Vertex Array Object (VAO)
  // used to hold active vertix attribute configurations supplied by VBO's
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // 2)
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
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

  // 3)
  // tell OpenGL how to interpret vertex data
  // (this can be done in the render loop for dynamic changes)
  // 0: attribute location (defined in the vertex shader)
  // 3: number of components per vertex attribute ie. vec3 = 3
  // GL_FLOAT: data type
  // GL_FALSE: normalize data?
  // 3 * sizeof(float): stride (space between consecutive vertex attributes)
  // (void*)0: offset of the first component
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  unsigned int mvpLocation = glGetUniformLocation(shaderProgram, "uMVP");

  // Main Render Loop
  while (!glfwWindowShouldClose(window)) {

    // terminate on ESC key press
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    // Compute transformation matrices
    float timeValue = glfwGetTime();
    glm::mat4 model =
        glm::rotate(glm::mat4(1.0f), timeValue, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    glm::mat4 mvp = projection * view * model;

    // Pass the MVP matrix to the vertex shader
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw to double buffer
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // glDrawArrays(GL_LINE_LOOP, 0, 3);

    // check for events and swap buffers, rendering triangle to screen
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
