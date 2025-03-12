#include "common.h"
int main()
{
  unsigned int w = 800;
  unsigned int h = 600;
  GLFWwindow *window = startup_opengl("MyProgramName", w, h);
  if (!window) {
    return -1;
  }

  // Define our triangle geometry
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

  float triangle_colors[] = {
      1.0f, 0.0f, 0.0f, // red
      0.0f, 1.0f, 0.0f, // green
      0.0f, 0.0f, 1.0f  // blue
  };

  /* SHADERS */
  // Shaders are written in GLSL (OpenGL Shading Language)

  // Load shader source files
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
  unsigned int VBO_points;
  glGenBuffers(1, &VBO_points);

  // bind VBO buffer to GL_ARRAY_BUFFER target (vertex array type)
  glBindBuffer(GL_ARRAY_BUFFER, VBO_points);

  // copy vertex data to bound GL_ARRAY_BUFFER (our VBO buffer)
  // 3 draw modes:
  // GL_STATIC_DRAW: the data is set once and used many times; static objects
  // GL_DYNAMIC_DRAW: the data will be changed a lot and used often; moving
  // objects GL_STREAM_DRAW: the data is set only once and only used a few
  // times; usecase? idk
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_verts), triangle_verts,
               GL_STATIC_DRAW);

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
  glEnableVertexAttribArray(0); // tell shader to turn on location 0 attribute

  unsigned int VBO_colors;
  glGenBuffers(1, &VBO_colors);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_colors), triangle_colors,
               GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1); // tell shader to turn on location 1 attribute

  // Main Render Loop
  while (!glfwWindowShouldClose(window)) {

    // terminate on ESC key press
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }

    // Clear the screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw to double buffer
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // glDrawArrays(GL_LINE_LOOP, 0, 3);

    // check for events and swap buffers, rendering triangle to screen
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
